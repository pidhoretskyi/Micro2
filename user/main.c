//Vitalii Pidhoretskyi
//PZ-41
//Variant 6 HAL

#include "stm32f4xx.h"  
#include "stm32f4xx_hal.h"  
#include "stm32f4xx_rcc.h"


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

uint32_t msTicks;

void initDelay()
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
}
 
void DelayMs(uint32_t ms)
{
  msTicks = ms;  
	while(msTicks);
}

void GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOE_CLK_ENABLE();
	GPIO_InitStruct.Pin=LCD_PIN_RS | LCD_PIN_RW | LCD_PIN_EN | LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST; //50 MHz
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);
	
}

void LCD_Pulse(){
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_EN , GPIO_PIN_SET);
	DelayMs(100);
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_EN , GPIO_PIN_RESET);
	DelayMs(100);
}

void WriteByte(uint8_t data){
	HAL_GPIO_WritePin(LCD_PORT, ((data & 0x0F)) << LCD_PIN_OFFSET, GPIO_PIN_SET);
	LCD_Pulse();
	HAL_GPIO_WritePin(LCD_PORT, (0x0F) << LCD_PIN_OFFSET, GPIO_PIN_SET);
	DelayMs(100);
	
	HAL_GPIO_WritePin(LCD_PORT, (((data<<4) & 0x0F)) << LCD_PIN_OFFSET, GPIO_PIN_SET);
	LCD_Pulse();
	HAL_GPIO_WritePin(LCD_PORT, (0x0F) << LCD_PIN_OFFSET, GPIO_PIN_SET);
	DelayMs(100);
}

void WriteValue(uint32_t value){	
	for(int i=3; i>=0; i--)
		WriteByte((uint8_t)(value >> 8*i));
}

void WriteStr(char *str){
	char *c = str;
	while((c != 0) && (*c != 0)){
		WriteByte((uint8_t)*c);
		c++;
	}
}

void LCD_Init(){
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, GPIO_PIN_RESET);
	DelayMs(150);
	WriteByte(0x20);
	DelayMs(150);
	WriteByte(0x28);
	DelayMs(150);
	WriteByte(0x28);
	DelayMs(150);
	WriteByte(0x0f);
	DelayMs(150);
	WriteByte(0x01);
	DelayMs(150);
	WriteByte(0x06);
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, GPIO_PIN_SET);	
}

void LCD_Clear(void){
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, GPIO_PIN_RESET);
	WriteByte(0x01);
	WriteByte(0x02);
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, GPIO_PIN_SET);	
}

void Cursor(uint8_t row, uint8_t col){
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, GPIO_PIN_RESET);
	uint8_t address =0;
	if(row==1)
		address = 0x40;
	address |= col;
	
	WriteByte(0x80 | address);
	
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, GPIO_PIN_SET);	
}

void Button_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOA_CLK_ENABLE();

  /* Configure GPIO pin: PA0 (USER) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint32_t rand(uint32_t seed){
		seed = (8253729 * seed + 2396403);
    return seed  % 32768;
}



int main()
{
	initDelay();
	HAL_Init();                               /* Initialize the HAL Library     */
	Button_Init();
	GPIO_Init();
	LCD_Init();
	for(uint32_t i=0; 1; i++){
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){ //If button is pressed
		LCD_Clear();
		WriteValue(rand(i));
	}
}
}
