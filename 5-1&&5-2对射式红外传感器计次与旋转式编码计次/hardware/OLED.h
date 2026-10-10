#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(unsigned char Line, unsigned char Column, char Char);
void OLED_ShowString(unsigned char Line, unsigned char Column, char *String);
void OLED_ShowNum(unsigned char Line, unsigned char  Column, unsigned int Number, unsigned char  Length);
void OLED_ShowSignedNum(unsigned char  Line, unsigned char  Column, int  Number, unsigned char  Length);
void OLED_ShowHexNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);
void OLED_ShowBinNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);
void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint8_t Index) ;

#endif



