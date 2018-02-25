/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   lcd_ctl.h
 * Author: Josepablo Cruz Baas
 * Comments:
 * Revision history: 
 */
#ifndef     LCD_CTL_H
#define     LCD_CTL_H
#include    <pic18f45k50.h>
#include    "../pic_configuration.h"
//<editor-fold defaultstate="collapsed" desc="LCD_LIB">
//  UNION DE 8 BITS
typedef union{
    struct{
    unsigned bit0               :1;
    unsigned bit1               :1;
    unsigned bit2               :1;
    unsigned bit3               :1;
    unsigned bit4               :1;
    unsigned bit5               :1;
    unsigned bit6               :1;
    unsigned bit7               :1;};
    struct{
        unsigned
        unsigned                :7;
        unsigned Busy           :1;
    };
}LCD_DATA_bits;
typedef union{
    struct{
        unsigned RS             :1;
        unsigned RW             :1;
        unsigned E              :1;
        unsigned                :4;
        unsigned NC             :1;
    };
    struct{
        unsigned CTL            :3;
        unsigned 
        unsigned P_NC           :5;
    };
}LCD_CTL;
//</editor-fold>

extern volatile LCD_CTL         lcd_ctl         @   0xF8D; //LATE
extern volatile LCD_DATA_bits   lcd_data_bits   @   0xF8C; //LATD
#define     LCD_DATA_P          PORTD
#define     LCD_DATA_L          LATD
#define     LCD_DATA_TRIS       TRISD
#define     LCD_CTL_TRIS        TRISE
#define     LCD_CTL_P           PORTE
#define     LCD_CTL_L           LATE
#define     LCD_SPECIFIC_CONFIG()   {ANSELD = 0x00;ANSELE = 0x00;}

#define     LCD_PROPERTY_LENGHT 16

#define     lcd_display_off         0x00
#define     lcd_display_on          0x04
#define     lcd_blink_cursor        0x01
#define     lcd_no_blink_cursor     0x00
#define     lcd_no_cursor           0x02
#define     lcd_cursor              0x00

enum{LCD_ALIGN_CENTER,LCD_ALIGN_RIGTH,LCD_ALIGN_LEFT};
/**
 * STROBE
 * Conmuta el Enable para enviar el dato al modulo lcd 
 */
    #define STROBE(){lcd_ctl.E = 1;asm("NOP");lcd_ctl.E=0;}
/**
 * writeByte
 * Escribe en el puerto de datos el byte especificado
 * esperando el busy 
 */
    void lcd_writeByte(unsigned char);
/**
 * init_lcd
 * Configura el puerto e inicializa el modulo lcd 2x16
 */
    #define init_lcd(){ LCD_SPECIFIC_CONFIG();\
                        LCD_DATA_P = 0;\
                        LCD_DATA_TRIS = 0x00;\
                        LCD_CTL_P = 0;\
                        LCD_CTL_TRIS = 0;\
                        LCD_DATA_L = 0x00;\
                        delay(20);\
                        lcd_ctl.CTL = 0;\
                        LCD_DATA_L=0x30;\
                        STROBE();\
                        delay(5);\
                        LCD_DATA_L=0x30;\
                        STROBE();\
                        delay_us(200);\
                        LCD_DATA_L=0x30;\
                        STROBE();\
                        delay_us(200);\
                        LCD_DATA_L=0x38;\
                        STROBE();\
                        lcd_writeByte(0x38);\
                        lcd_writeByte(0x0F);\
                        lcd_writeByte(0x01);\
                        lcd_writeByte(0x06);\
                      }
/**
 * putch
 * Escribe un caracter en el modulo lcd 2x16
 * @param C - caracter a imprimir
 */
    #define lcd_putch(C){lcd_ctl.CTL=0x0;lcd_ctl.RS=1;lcd_writeByte(C&0x7F);__delay_us(50);}
/**
 * goto
 * Permite apuntar a una posición dentro del lcd 2x16
 * @param N - Numero de caracter
 * 0x00 - 0x27 line 1
 * 0x40 - 0x67 line 2
 */
    #define lcd_goto(N){lcd_ctl.CTL=0x0;lcd_writeByte( 0x80 + (N&0x7F));}
    #define lcd_display_on_off_ctl(CMD){lcd_ctl.CTL = 0;lcd_writeByte(0x08 | CMD);}
    #define lcd_go_home(){lcd_ctl.CTL = 0;lcd_writeByte(0x02);__delay_ms(2);}
    #define lcd_clear(){lcd_ctl.CTL = 0;lcd_writeByte(0x01);__delay_ms(2);}
/**
 * puts
 * Permite escribir una cadena de caracteres en el lcd 2x16
 * @param const char* - Apuntador al primer caracter de la cadena
 */ 
    void lcd_puts(const char*);
    /**
     * lcd_puts_text_align
     * Imprime una cadena de caracteres alineada
     * con una longitud de hasta el valor de LCD_PROPERTY_LENGHT.
     * @param const char*   Cadena de caracteres
     * @param unsigned char 
     */
    void lcd_puts_text_align(const char*,unsigned char);
/**
 *  Mensajes predeterminados para el systema
 * 
 */
    #define MSG_WELCOME "¡BIENVENIDO!"
#endif
