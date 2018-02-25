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
 * File:   pwm_ctl.h
 * Author: Josepablo Cruz Baas
 * Comments:
 * Revision history: 
 */
#ifndef     PWM_CTL_H
#define     PWM_CTL_H
#include    "../pic_configuration.h"
// // // // // // // // // // //
//                            //
//  PWM 1                     //
//                            //
// // // // // // // // // // //


/**
 * init_pwm1
 * Inicializa el modulo CCP1 en modo PWM para el pin RC2
 * Con una resolución de 10 bits
 */
#define init_pwm1()  {PR2=0xFF;T2CON=1;PORTCbits.CCP1=0;TRISCbits.RC2=0;}
//
//  Numero máximo que se puede registrar como salida PWM en el modulo CCP1
//
#define PWM1_D_MAX   1023
/**
 * pwm1_duty_cycle
 * Modifica el ciclo de trabajo de la señal pwm en el modulo CCP1
 * @param DC    Ciclo de trabajo que va de 0 a PWM1_D_MAX, salida del PWM1
 */
#define pwm1_duty_cycle(DC)    {CCPR1L=(DC>>2);CCP1CONbits.DC1B=DC&0x3;\
                              T2CONbits.TMR2ON=1;CCP1CONbits.CCP1M=12;}
/**
 * pwm1_off
 * Apaga el modulo CCP1
 */
#define pwm1_off()  {CCP1CON=0x00;}
/**
 * get_duty_cycle1
 * Convierte un parametro en procentaje del 0 al 100 en un valor
 * valido como ciclo de trabajo para el modulo PWM1
 * @param unsigned int  valor en porcentaje del 0 al 100
 * @return unsigned int valor en ciclo de trabajo del 0 al PWM1_D_MAX
 */
unsigned int get_duty_cycle1(unsigned int);

// // // // // // // // // // //
//                            //
//  PWM 2                     //
//                            //
// // // // // // // // // // //

/**
 * init_pwm2
 * Inicializa el modulo CCP2 en modo PWM para el pin RC1
 * Con una resolución de 10 bits
 */
#define init_pwm2()  {PR2=0xFF;T2CON=1;PORTCbits.CCP2=0;TRISCbits.RC1=0;}
//
//  Numero máximo que se puede registrar como salida PWM en el modulo CCP2
//
#define PWM2_D_MAX   1023
/**
 * pwm2_duty_cycle
 * Modifica el ciclo de trabajo de la señal pwm en el modulo CCP2
 * @param DC    Ciclo de trabajo que va de 0 a PWM1_D_MAX, salida del PWM2
 */
#define pwm2_duty_cycle(DC)    {CCPR2L=(DC>>2);CCP2CONbits.DC2B=DC&0x3;\
                              T2CONbits.TMR2ON=1;CCP2CONbits.CCP2M=12;}
/**
 * pwm2_off
 * Apaga el modulo CCP2
 */
#define pwm2_off()  {CCP1CON=0x00;}
/**
 * get_duty_cycle1
 * Convierte un parametro en procentaje del 0 al 100 en un valor
 * valido como ciclo de trabajo para el modulo PWM2
 * @param unsigned int  valor en porcentaje del 0 al 100
 * @return unsigned int valor en ciclo de trabajo del 0 al PWM2_D_MAX
 */
unsigned int get_duty_cycle2(unsigned int);

#endif
