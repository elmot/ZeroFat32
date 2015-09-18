//----------------------------------------------------------------------
//EASTRISING TECHNOLOGY CO,.LTD.//
// Module    : ER-OLED0.96-1  0.96" 
// Lanuage   : C51 Code
// Create    : JAVEN
// Date      : 2010-06-18
// Drive IC  : SSD1306
// INTERFACE : I2C
// MCU 		 : AT89LV52
// VDD		 : 3.3V   VBAT: 3.6V    
// SA0 connected to VSS. Slave address:White:0x70 Read 0x71
//----------------------------------------------------------------------

#include "stm32f0xx_hal.h"
#include "oled.h"


uint8_t Contrast_level=0x8F;

#define Write_Address 0x78/*slave addresses with write*/
#define Read_Address 0x79/*slave addresses with read*/

#define Start_column	0x00
#define Start_page		0x00
#define	StartLine_set	0x00

#define sendCommand(...) { const uint8_t data [] =  {__VA_ARGS__}; HAL_I2C_Master_Transmit( &hi2c2, Write_Address, (uint8_t *) data, sizeof( data),20000); };

void Write_symbol(unsigned char const *n, uint8_t station_dot);
void adj_Contrast(void);
void Delay(uint16_t n);
void Set_Page_Address(unsigned char add);
void Set_Column_Address(unsigned char add);
void Set_Contrast_Control_Register(unsigned char mod);

//unsigned char ReceiveByte(void);

unsigned char const VAADIN_PICTURE[]=
{
0x40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0x40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0x40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0x40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,248,248,248,248,224,0,0,112,248,248,248,240,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,7,7,7,255,255,255,255,255,248,0,0,0,0,0,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,
0x40,30,31,63,255,255,254,248,224,0,192,248,254,255,255,127,31,159,244,252,252,254,255,63,31,31,31,63,255,254,252,248,240,0,224,248,252,254,254,63,31,31,31,63,127,254,254,252,240,192,0,240,248,252,254,255,63,31,31,31,63,255,255,255,255,255,255,0,0,252,254,255,255,254,0,4,30,255,255,255,254,254,63,31,31,31,31,63,255,254,252,248,224,0,0,0,0,0,0,0,0,0,0,31,255,255,255,255,255,224,224,224,0,15,31,31,31,63,62,60,60,252,248,248,248,240,240,240,224,
0x40,0,0,0,1,7,63,127,255,255,255,255,63,15,1,0,0,1,15,63,63,127,127,252,248,248,252,254,127,127,255,255,255,120,119,31,63,127,127,126,252,248,248,252,127,127,255,255,255,255,120,7,31,63,127,127,126,252,248,252,252,127,127,255,255,255,255,120,0,63,127,255,255,255,120,112,0,127,255,255,255,127,0,0,0,0,0,0,127,255,255,255,255,120,32,0,0,0,0,0,0,0,0,254,255,255,255,255,255,1,1,1,0,60,126,126,126,63,31,15,15,15,7,7,7,3,3,3,1,
0x40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,120,120,120,127,127,127,127,63,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0x40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

};

extern I2C_HandleTypeDef hi2c2;

void Write_symbol(uint8_t const *n, uint8_t station_dot)
{
	Set_Page_Address(Start_page);
  Set_Column_Address(Start_column + station_dot * 8);	
	HAL_I2C_Master_Transmit(&hi2c2, Write_Address, (uint8_t*)n, 9 , 20000);			

	Set_Page_Address(Start_page + 1);
  Set_Column_Address(Start_column+station_dot * 8);	

	HAL_I2C_Master_Transmit(&hi2c2, Write_Address, (uint8_t*) &n[9], 9 , 20000);			

}

void Delay(uint16_t n)
{
	unsigned long long nn= n;
	while(nn)
 {
	  nn--;
	 for(int i = 0; i <100;i++)
	 __nop();
 }
	return;
}


// Set page address 0~15
void Set_Page_Address(unsigned char add)
{
	sendCommand(0x80, 0xb0|add);
  __nop();
	return;
}

void Set_Column_Address(unsigned char add)
{	
	sendCommand(0x80,(0x10|(add>>4)),0x80,(0x0f&add));
}



void Set_Contrast_Control_Register(unsigned char mod)
{
	sendCommand(0x80, 0x81, 0x80,mod);
	return;
}


void OLED_Initial(void)
{
	Delay(2000);

	sendCommand( 0x80,0xae, 0x80,        //--turn off oled panel
		0xd5, 0x80, 0x80, 0x80, //--set display clock divide ratio/oscillator frequency
		0xa8, 0x80, 0x3f, 0x80, //--set multiplex ratio(1 to 64) 1/64 duty
		0xd3, 0x80, 0x00, 0x80, //-set display offset - no offset
		0x8d, 0x80, 0x14, 0x80,	//--set Charge Pump enable/disable - disable
		0x40, 0x80, 						//--set start line address
		0xa6, 0x80,							//--set normal display
		0xa4, 0x80,							//Disable Entire Display On
		0xa1, 0x80,							//--set segment re-map 128 to 0
		0xC8, 0x80,							//--Set COM Output Scan Direction 64 to 0
		0xda, 0x80, 0x12, 0x80,	//--set com pins hardware configuration
		0x81, 0x80, Contrast_level, 0x80, //--set contrast control register
		0xd9, 0x80, 0xf1, 0x80, //--set pre-charge period
		0xdb, 0x80, 0x40,				//--set vcomh
		0x80,0xaf								//--turn on oled panel
	);
}


void OLED_Display_Picture(unsigned const char pic[])
{
	for(int i = 0; i < 0x08; i++)
	{
		Set_Page_Address(i);
    Set_Column_Address(0x00);
		HAL_I2C_Master_Transmit(&hi2c2, Write_Address,(uint8_t*) &pic[i*0x81],0x81,20000);
	}
    return;
}

void run_oled(void)
{
	Delay(100);

    OLED_Initial();

    OLED_Display_Picture(VAADIN_PICTURE);
//		sendCommand(0x80,0xa7,0);//--set Inverse Display	
	while(1)
	{
		sendCommand(0x80,0xa7,0);//--set Inverse Display	
		Delay(6500);
		sendCommand(0x80,0xa6,0);//--set Normal Display	
		Delay(6500);
	}
}
