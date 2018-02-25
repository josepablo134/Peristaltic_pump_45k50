/*
 * File:   lcd_pwm.c
 * Author: josepablocruzbaas
 *
 * Created on June 18, 2017, 9:42 PM
 */
#include "lcd_ctl.h"

void lcd_puts(const char* str){
    while(*str){
        lcd_putch(*str++);
    }
}

void lcd_puts_text_align(const char* str,unsigned char align){
    unsigned char lenght=0;
    const char    *temp_ptr = str;
    //Averiguar la longitud de la candena
    while(*temp_ptr){lenght++; *temp_ptr++;}
    switch(align){
        case LCD_ALIGN_CENTER:
            lenght = LCD_PROPERTY_LENGHT - lenght;
            lenght /= 2;
            for(;lenght>0;lenght--)
                lcd_putch(' ');
            break;
        case LCD_ALIGN_RIGTH:
            lenght = LCD_PROPERTY_LENGHT - lenght;
            for(;lenght>0;lenght--)
                lcd_putch(' ');
            break;
        case LCD_ALIGN_LEFT:
            break;
        default :
            return;
    }
    lcd_puts(str);
}

void lcd_writeByte(unsigned char b){
//     unsigned char ctl = lcd_ctl.CTL;
//     lcd_ctl.CTL = 0;
//     lcd_ctl.RW = 1;
//     while(lcd_data_bits.Busy){}
//     lcd_ctl.CTL = ctl;
     delay_us(200);
     LCD_DATA_L = b & 0xFF;   //  Escribir byte en el puerto
     STROBE();  //  Enviamos el dato!
     LCD_DATA_L = 0;
     lcd_ctl.CTL = 0;
     delay_us(200);
 }