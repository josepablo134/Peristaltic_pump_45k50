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
 * File:   keypad_ctl.h
 * Author: Josepablo Cruz Baas
 * Comments:
 * Revision history: 
 */
#include "../pic_configuration.h"
#ifndef     KEYPAD_CTL_H
#define     KEYPAD_CTL_H

//
//  APP MODIFICATION IN KEYPAD_GETCH
//
#include "app.h"
#include "pwm_ctl.h"
#define init_beep(){pwm2_duty_cycle( get_duty_cycle2(5) );}
#define end_beep() {pwm2_duty_cycle(0);}



//<editor-fold defaultstate="collapsed" desc="KEY_LIB">
//  UNION DE 8 BITS
typedef union{
    struct{
    unsigned                    :3;
    unsigned output             :1;
    unsigned bit0               :1;
    unsigned bit1               :1;
    unsigned bit2               :1;
    unsigned bit3               :1;
    };
}KB_BITS;
//</editor-fold>
extern volatile KB_BITS kb_bits   @   0xF81;//PORTB
#define KEYBOARD_P PORTB
#define KEYBOARD_L LATB
unsigned const char keymask[4][4]=\
{{'1','2','3','A'},\
{'4','5','6','B'},\
{'7','8','9','C'},\
{'*','0','#','D'}};
/**
 * Configuración especifica del microcontrolador para activar
 * el puerto y desactivar funciones innecesarias
 */
#define     KP_SPECIFIC_CONFIG(){ANSELB=0x0;}
/**
 * Inicializa el puerto, desactiva el comparador analogico
 * y activa los pull ups del puerto.
 */
#define init_keypad_input(){KP_SPECIFIC_CONFIG();TRISB=0xF0;PORTB=0x00;\
                                                LATB=0x00;INTCON2bits.RBPU=0;\
                                                WPUB=0xF0;}
/**
 * Quitar el rebote producido
 * al precionar una tecla.
 */
#define debounce(){delay(32);}
/**
 * Espera la lectura de una tecla por medio del puerto.
 * @return unsigned char la tecla presionada
 */
unsigned char keypad_getch();
/**
 * Inenta realizar una lectura sin esperar
 * @return 
 */
unsigned char keypad_read();
#endif
