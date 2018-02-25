
#include "adc_ctl.h"
#include <pic18f45k50.h>

/*
 * File:   adc_ctl.c
 * Author: josepablocruzbaas
 *
 * Created on June 18, 2017, 9:41 PM
 */
unsigned short read_adc(){
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO){}
    return (ADRESH<<8)+(ADRESL&0xFF);
}