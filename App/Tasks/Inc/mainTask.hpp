#ifdef __cplusplus

#ifndef __APP_TASK_MAIN_H__
#define __APP_TASK_MAIN_H__

#include "main.h"

// Include the drivers
#include "ILI9341.hpp"

#define FRAME_BYTE_SIZE             (TFT_LCD::ILI9341::LCD_HEIGHT * TFT_LCD::ILI9341::LCD_WIDTH * TFT_LCD::ILI9341::PIXEL_BYTE_COUNT)

#define FRAME_BUFFER_ADDRESS        ((uint32_t)0xD0000000)
#define FRAME_BACK_BUFFER_ADDRESS   (FRAME_BUFFER_ADDRESS + FRAME_BYTE_SIZE)

#endif // __APP_TASK_MAIN_H__

#endif // __cplusplus