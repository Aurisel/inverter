#ifndef OLED_H
#define OLED_H

#include "stdint.h"


namespace oled
{

    enum status
    {
        IDLE,
        BUSY
    };

    enum result
    {
        FAIL,
        SUCCESS
    };

    namespace base
    {
        class interface
        {
            protected:
                void *buffer;
                status _status;
                uint32_t buffer_size;
            public:
                bool isBusy();
                virtual void transferComplete();
                virtual result sendCommand(uint8_t command) = 0;
                virtual result sendData(uint8_t *buffer, uint32_t size) = 0;
        };

        class canvas
        {
            friend class device;
            protected:
                void *buffer;
                uint32_t buffer_size;
            public:
                void clear();
                void *getBuffer();
        };

        class device
        {
            protected:
                virtual void selectDevice() = 0;
                virtual void releaseDevice() = 0;
                virtual void resetDevice() = 0;
                virtual result initDevice() = 0;
                interface *_interface;
            public:
                result flush(canvas& canvas);
                void flushComplete();
        };
    }
}





#endif
