#ifndef OLED_DEVICE_SSD1306_HPP
#define OLED_DEVICE_SSD1306_HPP

#include "oled.hpp"


namespace oled
{
    class device_ssd1306:public base::device
    {
        protected:
            result initDevice();
    };

}

#endif
