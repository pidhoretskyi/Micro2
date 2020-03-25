#ifndef LCD_H
#define LCD_H

#define 		LCD_PORT							GPIOE
#define     LCD_PIN_RS            GPIO_PIN_7  
#define     LCD_PIN_RW            GPIO_PIN_10
#define     LCD_PIN_EN            GPIO_PIN_11          
#define     LCD_PIN_D7            GPIO_PIN_15         
#define     LCD_PIN_D6            GPIO_PIN_14          
#define     LCD_PIN_D5            GPIO_PIN_13         
#define     LCD_PIN_D4            GPIO_PIN_12         
#define     LCD_PIN_MASK  				((LCD_PIN_RS | LCD_PIN_EN | LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4))
#define 		LCD_PIN_OFFSET  			12



void GPIO_Init(void);
void WriteByte(uint8_t data);
void WriteValue(uint32_t value);
void LCD_Init();

#endif