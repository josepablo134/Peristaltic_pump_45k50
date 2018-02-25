/*
 * File:   pwm_ctl.c
 * Author: josepablocruzbaas
 *
 * Created on June 18, 2017, 9:41 PM
 */
#include "pwm_ctl.h"

unsigned int get_duty_cycle1(unsigned int porcent){
                    return (PWM1_D_MAX*porcent)/100;
            }
unsigned int get_duty_cycle2(unsigned int porcent){
                    return (PWM1_D_MAX*porcent)/100;
            }