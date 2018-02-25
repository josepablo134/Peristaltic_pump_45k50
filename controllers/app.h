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
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
#include "../pic_configuration.h"
#include "lcd_ctl.h"
#include "keypad_ctl.h"
#include "adc_ctl.h"
#include "one_wire_ctl.h"
#include "pwm_ctl.h"
#include "EEPROM_ctl.h"
#include "timer0_ctl.h"

#ifndef APP_H
#define	APP_H
    #define TEMP_RANGE_MAX  2
    #define TEMP_RANGE_MIN  2
    #define PUMP(FORCE){pwm1_duty_cycle(FORCE);}

    #define ENABLE_PUMP(){APP_STATE_REG |= 0x02;}
    #define DISABLE_PUMP(){APP_STATE_REG &= 0x0D;}

    #define ENABLE_EVENTS(){GIE = 1;}
    #define DISABLE_EVENTS(){GIE = 0;}
    
    #define ENABLE_ADC_INT() { APP_STATE_REG |= 0x4; }
    #define DISABLE_ADC_INT(){ APP_STATE_REG &= 0xB; }

    #define init_beep(){pwm2_duty_cycle( get_duty_cycle2(5) );}
    #define end_beep() {pwm2_duty_cycle(0);}
    #define clear_keypad_buffer(){for(unsigned char i=0;i<6;i++){char_buffer[i]=0;}}
    typedef union{
        struct{
            unsigned SEC         :6;
            unsigned MIN         :6;
            unsigned HRS         :6;
        };
    }CLOCK;
    //
    //  Registros especificos
    //
    volatile unsigned char  APP_STATE_REG = 0;
    volatile unsigned char  VOLUME    = 5;
    volatile unsigned char  PUBLIC_TEMP = 0;
    volatile unsigned char  MAX_TEMP = 0;
    volatile unsigned char  MIN_TEMP = 0;
    volatile unsigned char  LAST_TEMP = 0;
    volatile CLOCK clock;
    unsigned char char_buffer[6] = {0,0,0,0,0,0};
    
    volatile unsigned char TMR0_COUNTER = 0;
    
    //
    //  CONTROLADOR DE INTERRUPCIONES
    //
    void interrupt ISR();
    
    
    //
    //  CONTROLADOR DE APLICACIONES
    //
    void init_app();
    
    void recovery_default_config();
    void save_default_config();
    
    unsigned char get_temp();
    
    void init_clock();
    void print_clock();
    void set_sec(unsigned char);
    void set_min(unsigned char);
    void set_hrs(unsigned char);
    
    void clock_event();
    
    void config_system();

    void get_temp_range();
    void get_volume();
    void get_time();
    
    
    void main_menu();
#endif
