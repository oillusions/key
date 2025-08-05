| [![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/oillusions/key)  |
|:---------------------------------------------------------------------------------------:|
---
## 概述
本项目基于STM32G0B1CBT6微控制器，实现了一个多功能嵌入式系统，包含显示控制、音频输出、LED驱动和用户输入等功能。系统使用8MHz外部晶振，通过PLL将主频提升至128MHz。

## 系统配置

### 硬件配置
| 项目              | 参数                  |
|-------------------|-----------------------|
| 主控芯片          | STM32G0B1CBT6        |
| 外部晶振          | 8MHz HSE             |
| 系统主频          | 128MHz (PLL倍频)      |
| 工作电压          | 3.3V                 |
| 调试接口          | SWD                  |

### 时钟配置
```C
// SystemClock_Config() 配置
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_HSI48;
RCC_OscInitStruct.HSEState = RCC_HSE_ON;
RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
RCC_OscInitStruct.PLL.PLLN = 32;
RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV8;
RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
```

### GPIO 映射表

### 显示接口 (ST7735S)
| 功能       | GPIO 引脚 | 信号方向 | 备注            |
|------------|-----------|----------|-----------------|
| LCD_CS     | PA5       | 输出     | 片选信号        |
| LCD_RES    | PA4       | 输出     | 复位信号        |
| LCD_DC     | PA3       | 输出     | 数据/命令选择   |
| LCD_SDA    | PA2       | 输出     | SPI数据线(MOSI) |
| LCD_CLK    | PA1       | 输出     | SPI时钟线(SCK)  |
| LCD_LED    | PA6       | 输出     | 背光控制        |

### 音频接口 (PCM5100A)
| 功能   | GPIO 引脚 | 信号方向 | 备注          |
|--------|-----------|----------|---------------|
| I2S_MCK| PB2       | 输出     | 主时钟        |
| I2S_CK | PB10      | 输出     | 位时钟(BCLK)  |
| I2S_SD | PB11      | 输出     | 数据线(DIN)  |
| I2S_WS | PB12      | 输出     | 字选择(LRCK) |

### 存储与输入 (ZD25WQ32 | 74HC165)
| 功能        | GPIO 引脚 | 信号方向 | 备注                 |
|-------------|-----------|----------|----------------------|
| FLASH_CS    | PD3       | 输出     | SPI Flash片选        |
| FLASH_CLK   | PB3       | 输出     | SPI时钟线(SCK)       |
| FLASH_MISO  | PB4       | 输入     | SPI数据输入(MISO)    |
| FLASH_MOSI  | PB5       | 输出     | SPI数据输出(MOSI)    |
| SR_IE       | PB6       | 输出     | 74HC165输入使能      |
| SR_CE       | PB7       | 输出     | 74HC165时钟使能      |

### 用户接口
| 功能      | GPIO 引脚 | 信号方向 | 备注               |
|-----------|-----------|----------|--------------------|
| LED       | PC13      | 输出     | 状态指示灯         |
| ENC1_A    | PA8       | 输入     | 编码器1通道A      |
| ENC1_B    | PA9       | 输入     | 编码器1通道B      |
| ENC1_C    | PB14      | 输入     | 编码器1按钮       |
| ENC2_A    | PC6       | 输入     | 编码器2通道A      |
| ENC2_B    | PC7       | 输入     | 编码器2通道B      |
| ENC2_C    | PB15      | 输入     | 编码器2按钮       |

### LED控制
| 功能     | GPIO 引脚 | 信号方向 | 备注               |
|----------|-----------|----------|--------------------|
| WS2812   | PA0       | 输出     | TIM2_CH1 PWM输出  |

## 片内外设映射

| 外设 | 功能                  | 使用场景             | 配置参数                     |
|------|-----------------------|----------------------|------------------------------|
| SPI1 | LCD控制               | ST7735S驱动         | 8位数据，波特率分频8        |
| SPI2 | I2S音频输出           | PCM5100A DAC         | 32位数据格式，Philips标准    |
| SPI3 | 存储和输入扩展        | Flash & 74HC165      | 8位数据，波特率分频8        |
| TIM1 | 编码器接口1           | 旋转编码器1          | 编码器模式，256周期         |
| TIM2 | PWM输出               | WS2812控制           | 160周期，PWM模式            |
| TIM3 | 编码器接口2           | 旋转编码器2          | 编码器模式，10000周期       |
| DMA  | 外设数据传输          | SPI/I2S/TIM数据加速 | 多通道配置，优先级管理      |
| USB  | USB设备               | 通信接口             | Full Speed模式              |

## 片外外设映射

### ST7735S LCD显示
- 分辨率: 80x160
- 接口: SPI
- 驱动代码: `panel_st7735s.cpp`
- 关键函数:
```CPP
void Panel_ST7735S::init() {
    RES(0);
    HAL_Delay(100);
    RES(1);
    HAL_Delay(100);
    Affairs(0x11);
    HAL_Delay(120);
    // ... 初始化序列 ...
    Affairs(0x29);
}
```
```CPP
void Panel_ST7735S::draw(uint8_t *data) {
    Affairs(0x2A, 4, (uint8_t[]){0, 1, 0, 160});
    Affairs(0x2B, 4, (uint8_t[]){0, 26, 0, 105});
    Affairs(0x2C, ((getWidth()* getHeight())* 2), data);
}
```

### PCM5100A 音频DAC
- 接口: I2S
- 音频格式: 32位，48kHz
- 配置:
```C
hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
hi2s2.Init.DataFormat = I2S_DATAFORMAT_32B;
hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_48K;
```

### WS2812 LED控制
- 数量: 8个
- 控制方式: PWM+DMA
- 数据格式: GRB 24位
- 关键函数:
```CPP
void setLEDColor(size_t pos, uint32_t color) {
    color = rgb_to_grb(color);
    uint32_t mask = 0x800000;
    for (uint8_t i = 0; i < 24; i++) {
        buffer[(pos * 24)+ i]= ((color << i)& mask)? WS2812_1 : WS2812_0;
    }
} 
```

### 74HC165 输入扩展
- 按键数量: 8个
- 接口: SPI
- 扫描流程:
```CPP
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
```

## 软件架构

### 主程序流程
```CPP
int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI3_Init();
  MX_USB_DRD_FS_PCD_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_I2S2_Init();
  MX_SPI1_Init();
  
  init();  // 初始化LED数据缓冲区[预填充低亮度白色合重置等待数据], 启动PWM和编码器
  run();   // 主循环
}
```

### 关键模块
1. **显示驱动** (`panel_st7735s.cpp`)
    - SPI事务处理
    - 显示屏初始化序列
    - 画面刷新优化

2. **输入处理** (`program.cpp`)
    - 旋转编码器解码
    - 按键扫描
    - LED颜色变换算法

3. **音频处理**
    - I2S数据流传输
    - DMA双缓冲管理

## 编译与烧录
1. 使用STM32CubeIDE导入项目
2. 配置使用ST-LINK/V2调试器
3. 编译目标: Release
4. 烧录配置:
    - Reset and Run
    - Enable flash erase

## 资源使用
- Flash: 98% (64KB中的62.8KB)
- RAM: 78% (20KB中的15.6KB)
- 外设利用率:
    - SPI: 100% (3个SPI接口全使用)
    - TIM: 75% (4个定时器使用3个)
    - DMA: 6通道全使用