#include "oled_canvas_char.hpp"
#include "string.h"
#include "oled_font.h"
#include "string.h"


void oled_canvas_char::clearLine(uint16_t line)
{
    uint32_t size_per_line = this -> buffer_size / line;
    memset(this -> buffer,0,size_per_line);
}

void oled_canvas_char::showChar(uint16_t line,uint16_t column,char character)
{
    if(line == 0 || line > this -> totalLine || column == 0 || column > this -> totalColumn)
        return;
    uint32_t size_per_half_line = this -> buffer_size / totalLine / 2;
    uint32_t size_per_half_column = size_per_half_line / totalColumn / 2;
    *((uint32_t *)(((char *)this -> buffer) + size_per_half_line * ((line - 1) * 2) + size_per_half_column * ((column - 1) * 2))) = OLED_F8x16_LE[character - ' '][0];
    *((uint32_t *)(((char *)this -> buffer) + size_per_half_line * ((line - 1) * 2) + size_per_half_column * ((column - 1) * 2 + 1))) = OLED_F8x16_LE[character - ' '][1];
    *((uint32_t *)(((char *)this -> buffer) + size_per_half_line * ((line - 1) * 2 + 1) + size_per_half_column * ((column - 1) * 2))) = OLED_F8x16_LE[character - ' '][2];
    *((uint32_t *)(((char *)this -> buffer) + size_per_half_line * ((line - 1) * 2 + 1) + size_per_half_column * ((column - 1) * 2 + 1))) = OLED_F8x16_LE[character - ' '][3];
}

void oled_canvas_char::showString(uint16_t line,uint16_t column,char *string)
{
    if(line == 0 || line > this -> totalLine || column == 0 || column > this -> totalColumn)
        return;
    for(uint16_t i = 0;string[i] != '\0' && ((column + i) <= this -> totalColumn);++i)
    {
        this -> showChar(line,(column + i),string[i]);
    }
}

oled_canvas_char_small::oled_canvas_char_small()
{
    this -> buffer = (void *)(this -> _buffer);
    this -> buffer_size = 1024;
    this -> totalColumn = 16;
    this -> totalLine = 4;
    memset(this -> buffer,0,this -> buffer_size);
}
