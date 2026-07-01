## Sheet1
| 广州市星翼电子科技有限公司（正点原子） | Unnamed: 1 | Unnamed: 2 | Unnamed: 3 | Unnamed: 4 | Unnamed: 5 |
| --- | --- | --- | --- | --- | --- |
| DNESP32S3 IO资源分配表 | NaN | NaN | NaN | NaN | NaN |
| 引脚编号 | GPIO | 连接资源 | NaN | 完全\n独立 | 连接关系说明 |
| 4 | IO4 | LCD\_DE | OV\_D0 | N | 1，\tRGBLCD的DE信号\n2，\t摄像头的D0信号 |
| 5 | IO5 | LCD\_CLK | OV\_D1 | N | 1，\tRGBLCD的CLK信号\n2，\t摄像头的D1信号 |
| 6 | IO6 | LCD\_B7 | OV\_D2 | N | 1，\tRGBLCD的B7信号\n2，\t摄像头的D2信号 |
| 7 | IO7 | LCD\_B6 | OV\_D3 | N | 1，\tRGBLCD的B6信号\n2，\t摄像头的D3信号 |
| 8 | IO15 | LCD\_B5 | OV\_D4 | N | 1，\tRGBLCD的B5信号\n2，\t摄像头的D4信号 |
| 9 | IO16 | LCD\_B4 | OV\_D5 | N | 1，\tRGBLCD的B4信号\n2，\t摄像头的D5信号 |
| 10 | IO17 | LCD\_B3 | OV\_D6 | N | 1，\tRGBLCD的B3信号\n2，\t摄像头的D6信号 |
| 11 | IO18 | LCD\_G7 | OV\_D7 | N | 1，\tRGBLCD的G7信号\n2，\t摄像头的D7信号 |
| 12 | IO8 | LCD\_G5、ADC\_IN | REMOTE\_OUT | N | 1，\tRGBLCD的G6信号\n2，\tADC输入信号\n3，\t红外发送信号 |
| 13 | IO19 | USB\_D- | USB\_D- | N | USB |
| 14 | IO20 | USB\_D+ | USB\_D+ | N | NaN |
| 15 | IO3 | LCD\_G5 | I2S\_MCLK | N | 1，\tRGBLCD的G5信号\n2，\t音频的MCLK信号 |
| 16 | IO46 | LCD\_G4 | I2S\_SCK | N | 1，\tRGBLCD的G4信号\n2，\t音频的SCK信号 |
| 17 | IO9 | LCD\_G3 | I2S\_LRCK | N | 1，\tRGBLCD的G3信号\n2，\t音频的LRCK信号 |
| 18 | IO10 | LCD\_G2 | I2S\_SDIN | N | 1，\tRGBLCD的G2信号\n2，\t音频的SDIN信号 |
| 19 | IO11 | SPI\_MOSI | SPI\_MOSI | N | SPI2口的MOSI信号 |
| 20 | IO12 | SPI\_SCK | SPI\_SCK | N | SPI2口的SCK信号 |
| 21 | IO13 | SPI\_MISO | SPI\_MISO | N | SPI2口的MISO信号 |
| 22 | IO14 | LCD\_R7 | I2S\_SDOUT | N | 1，\tRGBLCD的R7信号\n2，\t音频的SDOUT信号 |
| 23 | IO21 | LCD\_R6 | LCD\_CS | N | 1，\tRGBLCD的R6信号\n2，\tSPILCD的CS信号 |
| 24 | IO47 | LCD\_R5 | OV\_VSYNC | N | 1，\tRGBLCD的R5信号\n2，\t摄像头的VSYNC信号 |
| 25 | IO48 | LCD\_R4 | OV\_HREF | N | 1，\tRGBLCD的R4信号\n2，\t摄像头的HREF信号 |
| 26 | IO45 | LCD\_R3 | OV\_PCLK | N | 1，\tRGBLCD的R3信号\n2，\t摄像头的PCLK信号 |
| 27 | IO0 | IIC\_INT、BOOT | 1WIRE\_DQ | N | 1，\tXL9555的INT信号\n2，\tBOOT按键信号\n3，\tDHT11/DS18B20信号 |
| 28 | IO35 | NaN | NaN | Y | 不可用 |
| 29 | IO36 | NaN | NaN | Y | NaN |
| 30 | IO37 | NaN | NaN | Y | NaN |
| 31 | IO38 | CT\_SDA | OV\_SDA | N | 1，\t触摸IC的SDA信号\n2，\t摄像头的SDA信号 |
| 32 | IO39 | CT\_SCL | OV\_SCL | N | 1，\t触摸IC的SCL信号\n2，\t摄像头的SCL信号 |
| 33 | IO40 | CT\_INT | LCD\_DC | N | 1，\t触摸IC的INT信号\n2，\tSPILCD的DC信号 |
| 34 | IO41 | IIC\_SDA | IIC\_SDA | N | IIC0的SDA信号 |
| 35 | IO42 | IIC\_SCL | IIC\_SCL | N | IIC0的SCL信号 |
| 36 | RXD0 | U1RXD | U0RXD | N | UART0 |
| 37 | TXD0 | U1TXD | U0TXD | N | NaN |
| 38 | IO2 | TF\_CS | REMOTE\_IN | N | 1，\t触摸IC的CS信号\n2，\t红外发送信号 |
| 39 | IO1 | LED1 | LED0 | N | LED信号 |
| 引脚编号：对应DNESP32S3的引脚编号\n GPIO：DNESP32S3的IO口\n 完全独立：指该IO通过一定的方法，可以达到完全悬空的效果（即不接任何其他外设，且不接任何上拉/下拉电阻）\n连接关系说明：说明每个IO口与外设的连接关系\n 使用提示：介绍每个IO的特点和使用方法，方便大家掌握开发板每一个IO口的使用 | NaN | NaN | NaN | NaN | NaN |

## Sheet2
|
|  |

## Sheet3
|
|  |