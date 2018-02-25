/*
 * File:   main_sys.c
 * Author: josepablocruzbaas
 *
 * Created on June 18, 2017, 9:18 PM
 */


#include "pic_configuration.h"
#include "controllers/lcd_ctl.h"
#include "controllers/keypad_ctl.h"
#include "controllers/adc_ctl.h"
#include "controllers/one_wire_ctl.h"
#include "controllers/pwm_ctl.h"
#include "controllers/EEPROM_ctl.h"

#include "controllers/timer0_ctl.h"
#include "controllers/app.h"

#define DONE    while(1){}

void main(void){
    
    //
    //  INICIALIZACIÓN DE SERVICIOS DE HARDWARE
    //
    
    init_sys();//Inicializar sistema
    init_lcd();//Inicializar lcd
    lcd_display_on_off_ctl(lcd_display_on|lcd_no_cursor);
    
    init_keypad_input();//Inicializar el teclado
    init_adc();//Inicializar el ADC
    init_pwm2();//Inicializar el modulo pwm2
        pwm2_duty_cycle(0);//Apagar la salida del modulo
    init_pwm1();//Inicializar el modulo pwm1
        pwm1_duty_cycle(0);//Apagar la salida del modulo
    init_one_wire_protocol();//Inicializar comunicacion one wire
    
    config_tmr0_16bits();//Configurar el timer0
    
    init_app();//Inicializar aplicación de terapia
    DONE;
}

void lcd_put_temp(unsigned char temp){
    lcd_putch( HexToChar[ ((temp/100)%10) ] );
    lcd_putch( HexToChar[ ((temp/10)%10) ] );
    lcd_putch( HexToChar[ ((temp)%10) ] );
}