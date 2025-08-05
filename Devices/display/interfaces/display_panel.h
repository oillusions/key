#pragma once
#include <stdint.h>
#include <stddef.h>

class Panel {
    public:
        Panel(uint32_t width, uint32_t height): _width(width), _height(height)
        {};

        void virtual init() = 0;
        void virtual draw(uint8_t *data) = 0;

        uint32_t getWidth() {return _width;}
        uint32_t getHeight() {return _height;}

        virtual ~Panel() = default;
    private:
        uint32_t _width;
        uint32_t _height;
};


