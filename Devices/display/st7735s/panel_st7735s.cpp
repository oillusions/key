#include "panel_st7735s.h"
#include "main.h"
#include "spi.h"


#define CS(level) HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, (GPIO_PinState)level)
#define RES(level) HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, (GPIO_PinState)level)
#define DC(level) HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, (GPIO_PinState)level)

#define Panel_CS_Select 0
#define Panel_DC_Command 0
#define Panel_DC_Data 1


/**
 * @brief 发送数据-SPI
 * @param data_len: 数据长度
 * @param data: 数据指针
 * @retval none
 */
void Panel_ST7735S::sendData(size_t data_len, uint8_t* data)
{
    HAL_SPI_Transmit_DMA(&hspi1, data, data_len);
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
}

/**
 * @brief 读取数据-SPI
 * @param data_len: 数据长度
 * @param data: 数据缓冲区指针
 * @retval none
 */
void Panel_ST7735S::readData(uint8_t* buffer, size_t buffer_len)
{
    HAL_SPI_Receive_DMA(&hspi1, buffer, buffer_len);
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
}


/**
 * @brief 事务-单指令
 * @param instruction: 指令
 * @retval none
 */
void Panel_ST7735S::Affairs(uint8_t instruction)
{
    uint8_t cmd = static_cast<uint8_t>(instruction);
    CS(Panel_CS_Select);
    DC(Panel_DC_Command);
    sendData(1, &cmd);
    CS(!Panel_CS_Select);
}

/**
 * @brief 事务-指令+发送数据/参数
 * @param instruction: 指令
 * @param data_len: 数据长度
 * @param data: 数据指针
 * @retval none
 */
void Panel_ST7735S::Affairs(uint8_t instruction, size_t data_len, uint8_t* data)
{
    if (data_len == 0 || data == NULL) return;
    uint8_t cmd = static_cast<uint8_t>(instruction);
    CS(Panel_CS_Select);
    DC(Panel_DC_Command);
    sendData(1, &cmd);
    DC(Panel_DC_Data);
    sendData(data_len, data);
    CS(!Panel_CS_Select);
}

/**
 * @brief 事务-指令+读取数据
 * @param instruction: 指令
 * @param data_len: 读取大小
 * @param data: 数据缓冲区指针
 * @retval none
 */
void Panel_ST7735S::Affairs(uint8_t instruction, uint8_t* buffer, size_t buffer_len)
{
    if (buffer_len == 0 || buffer == NULL) return;
    uint8_t cmd = static_cast<uint8_t>(instruction);
    CS(Panel_CS_Select);
    DC(Panel_DC_Command);
    sendData(1, &cmd);
    DC(Panel_DC_Data);
    readData(buffer, buffer_len);
    CS(!Panel_CS_Select);
}

/**
 * @brief 事务-指令+发送参数+读取数据
 * @param instruction: 指令
 * @param param_len: 参数长度
 * @param buffer_len: 读取大小
 * @param param: 参数指针
 * @param buffer: 数据缓冲区指针
 * @retval none
 */
void Panel_ST7735S::Affairs(uint8_t instruction, size_t param_len, size_t buffer_len, uint8_t* param, uint8_t* buffer)
{
    if (param_len == 0 || param == NULL || buffer_len == 0 || buffer == NULL) return;
    uint8_t cmd = static_cast<uint8_t>(instruction);
    CS(Panel_CS_Select);
    DC(Panel_DC_Command);
    sendData(1, &cmd);
    DC(Panel_DC_Data);
    sendData(param_len, param);
    readData(buffer, buffer_len);
    CS(!Panel_CS_Select);
}

/**
 * @brief 事务-指令+发送参数+发送数据
 * @param instruction: 指令
 * @param param_len: 参数长度
 * @param param: 参数指针
 * @param buffer_len: 数据长度
 * @param data: 数据缓冲区指针
 * @retval none
 */
void Panel_ST7735S::Affairs(uint8_t instruction, size_t param_len, uint8_t* param, size_t data_len, uint8_t* data)
{
    if (param_len == 0 || param == NULL || data_len == 0 || data == NULL) return;
    uint8_t cmd = static_cast<uint8_t>(instruction);
    CS(Panel_CS_Select);
    DC(Panel_DC_Command);
    sendData(1, &cmd);
    DC(Panel_DC_Data);
    sendData(param_len, param);
    readData(data, data_len);
    CS(!Panel_CS_Select);
}

void Panel_ST7735S::config(uint8_t instruction, size_t data_len, uint8_t *data) {
	Affairs(instruction, data_len, data);
}


void Panel_ST7735S::init() {
    RES(0);
	HAL_Delay(100);
	RES(1);
	HAL_Delay(100);
	Affairs(0x11);
	HAL_Delay(120);
	Affairs(0xB1, 3, (uint8_t[]){0x05, 0x3C, 0x3C});
	Affairs(0xB2, 3, (uint8_t[]){0x05, 0x3C, 0x3C});
	Affairs(0xB2, 6, (uint8_t[]){0x05, 0x3C, 0x3C, 0x05, 0x3C, 0x3C});
	Affairs(0xB4, 1, (uint8_t[]){0x03});
	Affairs(0xC0, 3, (uint8_t[]){0xAB, 0x0B, 0x04});
	Affairs(0xC1, 1, (uint8_t[]){0xC5});
	Affairs(0xC2, 2, (uint8_t[]){0x0D, 0x00});
	Affairs(0xC3, 2, (uint8_t[]){0x8D, 0x6A}); 
	Affairs(0xC4, 2, (uint8_t[]){0x8D, 0xEE});
	Affairs(0xC4, 1, (uint8_t[]){0x0F});
	Affairs(0xE0, 16, (uint8_t[]){0x07, 0x0E, 0x08, 0x07, 0x10, 0x07, 0x02, 0x07, 0x09, 0x0F, 0x25, 0x36, 0x00, 0x08, 0x04, 0x10});
	Affairs(0xE1, 16, (uint8_t[]){0x0A, 0x0D, 0x08, 0x07, 0x0F, 0x07, 0x02, 0x07, 0x09, 0x0F, 0x25, 0x35, 0x00, 0x09, 0x04, 0x10});
	Affairs(0xFC, 1, (uint8_t[]){0x80});
	Affairs(0x3A, 1, (uint8_t[]){0x05});
	Affairs(0x36, 1, (uint8_t[]){0xA8});
	// Affairs(0x36, 1, (uint8_t[]){0x08});
	// Affairs(0x36, 1, (uint8_t[]){0xc8});
	// Affairs(0x36, 1, (uint8_t[]){0x78});
	Affairs(0x21);
	Affairs(0x29);
	Affairs(0x2A, 4, (uint8_t[]){0x00, 0x1A, 0x00, 0x69});
	Affairs(0x2B, 4, (uint8_t[]){0x00, 0x01, 0x00, 0xA0});
	Affairs(0x2C);
}

void Panel_ST7735S::draw(uint8_t *data) {
	Affairs(0x2A, 4, (uint8_t[]){0, 1, 0, 160});
	Affairs(0x2B, 4, (uint8_t[]){0, 26, 0, 105});
	Affairs(0x2C, ((getWidth()* getHeight())* 2), data);
}

void Panel_ST7735S::drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data) {
	Affairs(0x2A, 4, (uint8_t[]){0, (uint8_t)(x + 1), 0, (uint8_t)(x + width)});
	Affairs(0x2B, 4, (uint8_t[]){0, (uint8_t)(y + 26), 0, (uint8_t)( y + height + 26)});
	Affairs(0x2C, (width * height)* 2 , data);
}

Panel_ST7735S::~Panel_ST7735S() {

}
