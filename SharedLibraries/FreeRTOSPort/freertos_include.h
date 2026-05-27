#pragma once

#ifdef ESP32
    #include "freertos/FreeRTOS.h"
    #include "freertos/queue.h"
#else
    #include <Arduino_FreeRTOS.h>
#endif
