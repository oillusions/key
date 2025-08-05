#pragma once
#include "display_panel.h"

class Panel_ST7735S: public Panel {
    public:
        Panel_ST7735S(): Panel(80, 160) {};

        void init() override;
        void draw(uint8_t *data) override;
        void drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data);

        void config(uint8_t instruction, size_t data_len, uint8_t *data);

        ~Panel_ST7735S();
    private:
        void sendData(size_t data_len, uint8_t *data);
        void readData(uint8_t *buffer, size_t data_len);

        void Affairs(uint8_t instruction);
        void Affairs(uint8_t instruction, size_t data_len, uint8_t* data);
        void Affairs(uint8_t instruction, uint8_t* buffer, size_t buffer_len);
        void Affairs(uint8_t instruction, size_t data_len, size_t buffer_len, uint8_t* data, uint8_t* buffer);
        void Affairs(uint8_t instruction, size_t param_len, uint8_t *param,  size_t data_len, uint8_t *data);
};
