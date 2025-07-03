#include "oled.hpp"
#include "string.h"

void oled_base_interface::transferComplete()
{
    this -> status = interface_status::IDLE;
}


oled_result oled_base_device::flush(oled_base_canvas& canvas)
{
    this -> selectDevice();
    if(this -> interface -> sendData((uint8_t *)canvas.buffer, canvas.buffer_size) == oled_result::OLED_FAIL)
    {
        return oled_result::OLED_FAIL;
    }
    return oled_result::OLED_SUCCESS;
}

void oled_base_device::flushComplete()
{
    this -> interface -> transferComplete();
    this -> releaseDevice();
}

void oled_base_canvas::clear()
{
    memset(this -> buffer,0,this -> buffer_size);
}



