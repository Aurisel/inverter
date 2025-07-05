#include "oled.hpp"
#include "string.h"

void oled_base_interface::transferComplete()
{
    this -> status = oled_status::OLED_IDLE;
}

bool oled_base_interface::isBusy()
{
    if(this -> status == oled_status::OLED_BUSY)
        return true;
    else
        return false;
}


oled_result oled_base_device::flush(oled_base_canvas& canvas)
{
    if(this -> interface -> isBusy())
        return oled_result::OLED_FAIL;
    this -> selectDevice();
    return this -> interface -> sendData((uint8_t *)canvas.buffer, canvas.buffer_size);
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



