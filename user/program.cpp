#include "program.h"
#include "math.h"
#include "spi.h"
#include "tim.h"
#include "panel_st7735s.h"

#define LED(level) HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, (GPIO_PinState)level)
#define BACK(level) HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, (GPIO_PinState)level)

#define WS2812_0 53
#define WS2812_1 107
#define LED_NUM 8
#define WS2812_DELAY 280
#define LED_BUFFER_SIZE (24 * LED_NUM)+ WS2812_DELAY


uint8_t buffer[LED_BUFFER_SIZE] = {0x0000};

uint32_t rgb_to_grb(uint32_t color) {
		uint32_t out = 0;
	out |= ((color & 0x00FF0000)>> 8);
	out |= ((color & 0x0000FF00)<< 8);
	out |= (color & 0x000000FF);

	return out;
}

uint32_t rgba_to_rgb(uint32_t color) {
    uint16_t r, g, b, a;
    uint32_t out = 0;

    r = (uint16_t)((color & 0xFF000000)>> 24);
    g = (uint16_t)((color & 0x00FF0000)>> 16);
    b = (uint16_t)((color & 0x0000FF00)>> 8);
    a = (uint16_t)((color & 0x000000FF));

    out |= ((r * a)/ 255)<< 16;
    out |= ((g * a)/ 255)<< 8;
    out |= ((b * a)/ 255);

    return out;
}

void setLEDColor(size_t pos, uint32_t color) {
	color = rgb_to_grb(color);
	uint32_t mask = 0x800000;
	for (uint8_t i = 0; i < 24; i++) {
		buffer[(pos * 24)+ i]= ((color << i)& mask)? WS2812_1 : WS2812_0;
	}
}

void setLEDColor_rgba(size_t pos, uint32_t color) {
    setLEDColor(pos, rgba_to_rgb(color));
}

#define SR_CLK_ENABLE(level) HAL_GPIO_WritePin(SR_CE_GPIO_Port, SR_CE_Pin, (GPIO_PinState)level)
#define SR_IN_ENABLE(level) HAL_GPIO_WritePin(SR_IE_GPIO_Port, SR_IE_Pin, (GPIO_PinState)level)

uint8_t key_buffer = 0;

void Test() {
    SR_IN_ENABLE(0);
    HAL_Delay(1);
    SR_IN_ENABLE(1);

    
    SR_CLK_ENABLE(0);
    HAL_Delay(1);

    HAL_SPI_Receive_DMA(&hspi3, &key_buffer, 1);
    while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
    SR_CLK_ENABLE(1);
}


Panel *panel;

void init() {
    for (size_t i = LED_BUFFER_SIZE - WS2812_DELAY; i < LED_BUFFER_SIZE; i++) {
        buffer[i] = 0x00000000;
    }

    for (size_t i = 0; i < LED_NUM; i++) {
        setLEDColor_rgba(i, 0xFFFFFF10);
    }

    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)buffer, LED_BUFFER_SIZE);
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}

#define COLOR_NUM 7

uint32_t colors[COLOR_NUM] = {0xFF000000, 0xFFA50000, 0xFFFF0000, 0x00FF0000, 0x00FFFF00, 0x0000FF00, 0x80008000};

uint32_t color_lerp(float t, uint32_t color0, uint32_t color1) {
    uint32_t r0 = (color0 >> 24) & 0xFF;
    uint32_t g0 = (color0 >> 16) & 0xFF;
    uint32_t b0 = (color0 >> 8)  & 0xFF;
    uint32_t a0 = color0 & 0xFF;
    
    uint32_t r1 = (color1 >> 24) & 0xFF;
    uint32_t g1 = (color1 >> 16) & 0xFF;
    uint32_t b1 = (color1 >> 8)  & 0xFF;
    uint32_t a1 = color1 & 0xFF;
    
    t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;
    
    uint32_t r = (uint32_t)(r0 * (1.0f - t) + r1 * t + 0.5f);
    uint32_t g = (uint32_t)(g0 * (1.0f - t) + g1 * t + 0.5f);
    uint32_t b = (uint32_t)(b0 * (1.0f - t) + b1 * t + 0.5f);
    uint32_t a = (uint32_t)(a0 * (1.0f - t) + a1 * t + 0.5f);
    
    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;
    a = (a > 255) ? 255 : a;
    
    return (r << 24) | (g << 16) | (b << 8) | a;
}

uint32_t getColor(float x) {
    static float f = 1 / (float)COLOR_NUM;
    float xl = fmod(x, 1.0f);
    size_t pos = fmod((xl / f), (float)COLOR_NUM);
    float n = fmod(xl, f)* (float)COLOR_NUM;

    uint32_t color_prev = colors[pos];
    uint32_t color_next = colors[(pos + 1)% COLOR_NUM];

    return color_lerp(n, color_prev, color_next);
}

void run() {
	uint8_t a = 0;
	bool t = false;
    float ci = 0;
    float l = 0;

    static float f = 1.0f / (float)(LED_NUM * 8);
    
    while (true) {
        Test();
        if (HAL_GPIO_ReadPin(ENC1_C_GPIO_Port, ENC1_C_Pin)!= GPIO_PIN_SET) {
            __HAL_TIM_SET_COUNTER(&htim1, 0);
        }
        if (HAL_GPIO_ReadPin(ENC2_C_GPIO_Port, ENC2_C_Pin)!= GPIO_PIN_SET) {
            __HAL_TIM_SET_COUNTER(&htim3, 0);
        }

        a = (uint8_t)__HAL_TIM_GET_COUNTER(&htim1);
        l = 1.0f / (float)(__HAL_TIM_GET_COUNTER(&htim3)* 100);
    
        LED(!HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin));

        for (size_t i = 0; i < LED_NUM; i++) {
			setLEDColor_rgba(i, (((key_buffer >> i)& 0x01)== 1)? getColor((f *(float)i + ci)) | a : 0x0000FF00 | a);
            ci += l;
            if (ci >= 1) {
                ci = 0;
            }
		}
    }
}
