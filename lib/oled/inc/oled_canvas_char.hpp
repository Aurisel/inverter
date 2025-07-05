#ifndef OLED_CANVAS_CHAR_HPP
#define OLED_CANVAS_CHAR_HPP

#include "oled.hpp"

class oled_canvas_char : public oled_base_canvas
{
    protected:
        uint16_t totalLine;
        uint16_t totalColumn;
    public:
        void clearLine(uint16_t line);
        void showString(uint16_t line,uint16_t column,const char *string);
        void showChar(uint16_t line,uint16_t column,char character);
};

class oled_canvas_char_small : public oled_canvas_char
{
    protected:
        uint32_t _buffer[256];
    public:
        oled_canvas_char_small();
};





#endif

