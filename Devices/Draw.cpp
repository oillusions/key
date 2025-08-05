#include "Draw.h"

float lerp(float a, float b, float s) {
    return a +(b - a)* s;
}

Draw::Draw(LCD_GC9D01* screen_driver, uint8_t* buffer, size_t buffer_size)
{
    if (screen_driver == NULL || buffer == NULL) return;
    driver_instance = screen_driver;
    screen_size = screen_driver->GetScreenSize();

    this->buffer_size = buffer_size;
    this->buffer = buffer;

    SetStrokeWidth(1);
}

Draw::Draw(LCD_GC9D01* screen_driver)
{
    if (screen_driver == NULL) return;
    driver_instance = screen_driver;

    Screen_Size screen_size = driver_instance->GetScreenSize();

    buffer_size = (screen_size.width * screen_size.height)* 2;
    buffer = new uint8_t[buffer_size];

    SetStrokeWidth(1);
}

bool Draw::IsOutOfBounds(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    return (x > screen_size.width || y > screen_size.height ||
         width > screen_size.width || height > screen_size.height ||
          x + width > screen_size.width || y + height > screen_size.height);
}

void Draw::SetStrokeWidth(size_t stroke_width)
{
    this->stroke_width = stroke_width;
}

uint8_t* Draw::GetBuffer()
{
    return this->buffer;
}

void Draw::Drop(uint16_t x, uint16_t y, uint16_t color)
{
	size_t offset;

    x = screen_size.width - x - 1;

	offset = (((SCREEN_WIDTH * y)+ x)* 2);
	buffer[offset] = color >> 8;
	buffer[offset + 1] = color;
}

void Draw::Line(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color)
{
    int dx = x1 - x;
    int dy = y1 - y;
    int ux = ((dx > 0) << 1) - 1;
    int uy = ((dy > 0) << 1) - 1;
    int xl = x, yl = y, eps = 0; 
    dx = fabs(dx);
    dy = fabs(dy);
    if (dx > dy)
    {
        for (xl = x; xl!= x1; xl += ux)
        {
            Drop(xl,yl, color);
            eps += dy;
            if ((2*eps) >= dx)
            {
                yl += uy;
                eps -= dx;
            }
        }
    }
    else
    {
        for (yl = y; yl != y1; yl += uy)
        {
            Drop(xl,yl, color);
            eps += dx;
            if ((2*eps) >= dy)
            {
                xl += ux;
                eps -= dy;
            }
        }
    }
}

void Draw::Line(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color, uint16_t color1)
{
    int16_t dx = x1 - x;
    int16_t dy = y1 - y;
    int16_t ux = ((dx > 0) << 1) - 1;
    int16_t uy = ((dy > 0) << 1) - 1;
    int16_t xl = x, yl = y, eps = 0; 
    dx = fabs(dx);
    dy = fabs(dy);

    uint8_t r = (color & 0xF800) >> 11,
        g = (color & 0x07E0) >> 5,
        b = (color & 0x001F);

    uint8_t r1 = (color1 & 0xF800) >> 11,
        g1 = (color1 & 0x07E0) >> 5,
        b1 = (color1 & 0x001F);

    int8_t r_vector = r1 - r,
        g_vector = g1 - g,
        b_vector = b1 - b;

    if (dx > dy)
    {
        float pace = 1.0 /((x1 - x)/ ux), s = 0;
        uint16_t scolor;

        for (xl = x; xl != x1; xl += ux)
        {
            scolor = ((uint16_t)(r + (r_vector * s))) << 11;
            scolor |= ((uint16_t)(g + (g_vector * s))) << 5;
            scolor |= (uint16_t)(b + (b_vector * s));
            Drop(xl,yl, scolor);
            eps += dy;
            s += pace;
            if ((2*eps) >= dx)
            {
                yl += uy;
                eps -= dx;
            }
        }
    }
    else
    {
        float pace = 1.0 /((y1 - y)/ uy), s = 0;
        uint16_t scolor;
        for (yl = y; yl != y1; yl += uy)
        {
            scolor = ((uint16_t)(r + (r_vector * s))) << 11;
            scolor |= ((uint16_t)(g + (g_vector * s))) << 5;
            scolor |= (uint16_t)(b + (b_vector * s));
            Drop(xl,yl, scolor);
            eps += dx;
            s += pace;
            if ((2*eps) >= dy)
            {
                xl += ux;
                eps -= dy;
            }
        }
    }
}

Draw::~Draw()
{

}


