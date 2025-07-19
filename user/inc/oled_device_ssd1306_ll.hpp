#ifndef OLED_DEVICE_SSD1306_LL_HPP
#define OLED_DEVICE_SSD1306_LL_HPP

#include "oled_device_ssd1306.hpp"
#include "stm32g4xx_ll_gpio.h"

namespace oled
{
    class device_ssd1306_ll: public device_ssd1306
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
            device_ssd1306_ll(base::interface *interface,GPIO_TypeDef *cs_group,uint32_t cs_pin,GPIO_TypeDef *rst_group,uint32_t rst_pin);
    };

}

#endif
