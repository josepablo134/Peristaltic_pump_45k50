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
 * File:   adc_ctl.h
 * Author: Josepablo Cruz Baas
 * Comments:
 * Revision history: 
 */
#ifndef     ADC_CTL_H
#define     ADC_CTL_H
    #include "../pic_configuration.h"
/**
 *  Portconfiguration
 *  Channelselection
 *  ADCvoltagereferenceselection
 *  ADCconversionclocksource
 *  Interruptcontrol
 *  Resultsformatting
 * @return 
 */
    #define init_adc(){PORTAbits.AN0=0;TRISAbits.RA0=1;\
                       ANSELAbits.ANSA0=1;\
                       ADCON0 = 0x01;\
                       ADCON1bits.PVCFG = 0x00;\
                       ADCON1bits.NVCFG = 0x00;\
                       ADCON2bits.ADFM = 1;\
                       ADCON2bits.ACQT = 0b111;\
                       ADCON2bits.ADCS = 0b110;\
                        }
    unsigned short read_adc(); 
    #define read_adc_with_int(){ADCON0bits.GO = 1;}
#endif
