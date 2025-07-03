#ifndef OLED_DEVICE_SSD1306_HPP
#define OLED_DEVICE_SSD1306_HPP

#include "oled.hpp"
#include "stm32g4xx_ll_gpio.h"

class oled_device_ssd1306:public oled_base_device
{
    protected:
        GPIO_TypeDef *cs_group;
        uint32_t cs_pin;
        GPIO_TypeDef *rst_group;
        uint32_t rst_pin;
        void selectDevice();
        void releaseDevice();
        void resetDevice();
    public:
        oled_device_ssd1306(oled_base_interface *interface,GPIO_TypeDef *cs_group,uint32_t cs_pin,GPIO_TypeDef *rst_group,uint32_t rst_pin);
        oled_result initDevice();
};



#endif
