#ifndef OLED_CANVAS_CHAR_HPP
#define OLED_CANVAS_CHAR_HPP

#include "oled.hpp"

namespace oled
{
    class char_canvas : public base::canvas
    {
        protected:
            uint16_t totalLine;
            uint16_t totalColumn;
        public:
            void clearLine(uint16_t line);
            void showString(uint16_t line,uint16_t column,const char *string);
            void showConvertString(uint16_t line,uint16_t column,const char *string);
            void showChar(uint16_t line,uint16_t column,char character);
    };

    class char_canvas_small : public char_canvas
    {
        protected:
            uint32_t _buffer[256];
        public:
            char_canvas_small();
    };
}





#endif

