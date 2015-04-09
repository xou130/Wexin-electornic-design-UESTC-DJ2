//figure.TFT
#define LCD_W 240
#define LCD_H 320
void tft_write(uchar);
void addr_set(unsigned int, unsigned int, unsigned int, unsigned int);
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 mode);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len);
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 mode);
//refer sample lcd.c
//end of tft defines



void tft_write
{



}