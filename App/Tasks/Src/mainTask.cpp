#include "mainTask.hpp"

#include "cmsis_os.h"

#include "lvgl.h"

extern "C"
void mainTaskHandler(void *argument){

    for(;;){
        osDelay(1);
    }
}