#ifndef OLED_H
#define OLED_H

#include "stdint.h"

enum oled_status
{
    OLED_IDLE,
    OLED_BUSY
};

enum oled_result
{
    OLED_FAIL,
    OLED_SUCCESS
};

class oled_base_interface
{
    protected:
        void *buffer;
        oled_status status;
        uint32_t buffer_size;
    public:
        bool isBusy();
        virtual void transferComplete();
        virtual oled_result sendCommand(uint8_t command) = 0;
        virtual oled_result sendData(uint8_t *buffer, uint32_t size) = 0;
};

class oled_base_canvas
{
    friend class oled_base_device;
    protected:
        void *buffer;
        uint32_t buffer_size;
    public:
        void clear();
};

class oled_base_device
{
    protected:
        virtual void selectDevice() = 0;
        virtual void releaseDevice() = 0;
        virtual void resetDevice() = 0;
        virtual oled_result initDevice() = 0;
        oled_base_interface *interface;
    public:
        oled_result flush(oled_base_canvas& canvas);
        void flushComplete();
};







#endif
