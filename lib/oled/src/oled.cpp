#include "oled.hpp"
#include "string.h"

namespace oled::base
{

    void interface::transferComplete()
    {
        this -> _status = status::IDLE;
    }

    bool interface::isBusy()
    {
        if(this -> _status == status::BUSY)
            return true;
        else
            return false;
    }


    result device::flush(canvas& canvas)
    {
        if(this -> _interface -> isBusy())
            return result::FAIL;
        this -> selectDevice();
        return this -> _interface -> sendData((uint8_t *)canvas.buffer, canvas.buffer_size);
    }

    void device::flushComplete()
    {
        this -> _interface -> transferComplete();
        this -> releaseDevice();
    }

    void canvas::clear()
    {
        memset(this -> buffer,0,this -> buffer_size);
    }

    void* canvas::getBuffer()
    {
        return this -> buffer;
    }
}

