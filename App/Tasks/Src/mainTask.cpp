#include "mainTask.hpp"

#include "cmsis_os.h"

#include "lvgl.h"
#include "demos/lv_demos.h"

#include "spi.h"
#include "ltdc.h"
#include "dma2d.h"
#include "i2c.h"
#include <src/indev/lv_indev.h>
#include <widgets/lv_demo_widgets.h>

static lv_display_t* disp_ili9241 = nullptr;
static lv_indev_t* indev_stmpe811 = nullptr;


TFT_LCD::ILI9341 lcd(
    TFT_LCD::ILI9341_Config{
        .hspi = &hspi5,
        .CS = {CSX_GPIO_Port,CSX_Pin},              // CS
        .WR = {WRX_DCX_GPIO_Port,WRX_DCX_Pin},      // WR
        .RD = {RDX_GPIO_Port,RDX_Pin},              // RD
        .hltdc = &hltdc,
        .hdma2d = &hdma2d
    }
);

TFT_LCD::STMPE811 touchPanel(
    &hi2c3
);

void display_direct_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map){
    // In DIRECT mode, this callback only switches the final framebuffer address, so `area` is unused here.
    (void)area;

    // The flush callback can be called multiple times per frame, so switch the LTDC address only on the last flush.
    if(lv_display_flush_is_last(display)) {
        // Convert the LVGL-rendered pixel buffer pointer into an LTDC framebuffer address.
        const uint32_t fb_addr = reinterpret_cast<uint32_t>(px_map);

        // Update LTDC layer 0 framebuffer address without immediate reload; apply it at the next reload event.
        if(HAL_LTDC_SetAddress_NoReload(&hltdc, fb_addr, 0) != HAL_OK) {
            // Enter the system error handler if LTDC address update fails.
            Error_Handler();
        }

        // Reload LTDC registers during VBlank to reduce tearing.
        if(HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING) != HAL_OK) {
            // Enter the system error handler if LTDC reload fails.
            Error_Handler();
        }
    }

    // Notify LVGL that this flush is complete so the next rendering/flush step can continue.
    lv_display_flush_ready(display);
}


void lv_init_display(){
    // setting the display resolution
    disp_ili9241 = lv_display_create(TFT_LCD::ILI9341::LCD_WIDTH, TFT_LCD::ILI9341::LCD_HEIGHT);

    // setting method of displaying frame to display
    lv_display_set_buffers(disp_ili9241, 
        reinterpret_cast<void*>(FRAME_BUFFER_ADDRESS),
        reinterpret_cast<void*>(FRAME_BACK_BUFFER_ADDRESS), 
        FRAME_BYTE_SIZE, 
        lv_display_render_mode_t::LV_DISPLAY_RENDER_MODE_DIRECT
    );

    lv_display_set_flush_cb(disp_ili9241, display_direct_flush_cb);
}

void touch_read(lv_indev_t * indev, lv_indev_data_t * data){
    if(touchPanel.isTouched()) {
        touchPanel.getTouchedPoint((uint32_t*)&data->point.x, (uint32_t*)&data->point.y);

        data->point.x *= (TFT_LCD::ILI9341::LCD_WIDTH / (float)TP_WIDTH);
        data->point.y *= (TFT_LCD::ILI9341::LCD_HEIGHT / (float)TP_HEIGHT);

        data->point.x = TFT_LCD::ILI9341::LCD_WIDTH - data->point.x;
        data->point.y = TFT_LCD::ILI9341::LCD_HEIGHT - data->point.y;

        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void lv_init_indev(){
    indev_stmpe811 = lv_indev_create();
    lv_indev_set_type(indev_stmpe811, lv_indev_type_t::LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_stmpe811, touch_read);
}

extern "C"
void mainTaskHandler(void *argument){
    // Initializing the ILI9341 display via SPI protocol
    lcd.initalize(reinterpret_cast<uint16_t*>(FRAME_BUFFER_ADDRESS));
    touchPanel.initalize();

    // Initializing lvgl
    lv_init();
    lv_tick_set_cb(xTaskGetTickCount);

    lv_init_display();

    lv_init_indev();

    lv_demo_widgets();
    
    for(;;){
        lv_timer_handler();
        osDelay(2);
    }
}
