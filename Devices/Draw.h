#pragma once

#include "lcd_gc9d01.h"
#include <stdint.h>
#include <stddef.h>
#include <math.h>

// typedef union Color
// {
//     uint16_t color16;
//     uint8_t  byte[2];
// };



class Draw {

    private:
        LCD_GC9D01* driver_instance;
        uint8_t* buffer;
        size_t buffer_size;
        Screen_Size screen_size;
        size_t stroke_width;
    public:
        Draw(LCD_GC9D01* screen_driver);
        Draw(LCD_GC9D01* screen_driver, uint8_t* buffer, size_t buffer_size);
        bool IsOutOfBounds(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
        void SetStrokeWidth(size_t stroke_width);
        uint8_t *GetBuffer();
        void Drop(uint16_t x, uint16_t y, uint16_t color);
        void Line(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color);
        void Line(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color, uint16_t color1);
        ~Draw();


};


