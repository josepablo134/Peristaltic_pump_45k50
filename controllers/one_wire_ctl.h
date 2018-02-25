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
 * File:   1_wire_ctl.h
 * Author: Josepablo Cruz Baas
 * Comments:
 * Revision history: 
 */

//
//  Antes de importar esta libreria, es necesario definir 
//  los bits de entrada, salida y configuración con los nombres:
//  ONE_WIRE_INPUT_BIT, ONE_WIRE_OUTPUT_BIT, ONE_WIRE_CONFIG_BIT
//  respectivamente.
//
//Ejemplo:
//  #define ONE_WIRE_INPUT_BIT      PORTCbits.RC0
//  #define ONE_WIRE_OUTPUT_BIT     LATCbits.RC0
//  #define ONE_WIRE_CONFIG_BIT     TRISCbits.RC0
//Dependiendo del hardware, será necesario desactivar
//  los comparadores analógicos.
//
    #ifndef    ONE_WIRE_INPUT_BIT
        #warning    "Es necesario especificar el bit de entrada, ejemplo : PORTCbits.RC0"
        #define ONE_WIRE_INPUT_BIT          PORTCbits.RC0
    #endif
    #ifndef    ONE_WIRE_OUTPUT_BIT
        #warning    "Es necesario especificar el bit de salida, ejemplo : LATCbits.RC0"
        #define ONE_WIRE_OUTPUT_BIT         LATCbits.LATC0
    #endif
    #ifndef    ONE_WIRE_CONFIG_BIT
        #warning  "Es necesario especificar el bit de configuracion, ejemplo : TRISCbits.RC0"
        #define ONE_WIRE_CONFIG_BIT         TRISCbits.RC0
    #endif

#if !defined(ONE_WIRE_CTL_H) && defined(ONE_WIRE_INPUT_BIT) && defined(ONE_WIRE_OUTPUT_BIT) && defined(ONE_WIRE_CONFIG_BIT)
#define     ONE_WIRE_CTL_H
    #include "../pic_configuration.h"
    #include <stdbool.h>
//
//  MASTER ROM COMMAND
//
#define ONE_WIRE_CMD_READ_ROM                0x33
#define ONE_WIRE_CMD_MATCH_ROM               0x55
#define ONE_WIRE_CMD_SEARCH_ROM              0xF0
#define ONE_WIRE_CMD_ALARM_SEARCH            0xEC
#define ONE_WIRE_CMD_SKIP_ROM                0xCC
//
//  MASTER TX FUNCTION COMMAND
//
#define ONE_WIRE_CMD_CONVERT_TEMP            0x44
#define ONE_WIRE_CMD_COPY_SCRATCHPAD         0x48
#define ONE_WIRE_CMD_WRITE_SCRATCHPAD        0x4E
#define ONE_WIRE_CMD_READ_SCRATCHPAD         0xBE
#define ONE_WIRE_CMD_RECALL_E                0xB8
#define ONE_WIRE_CMD_READ_POWER_SUPPLY       0xB4

//
//  Microseconds time
//
#define ONE_WIRE_delay_w_0_A                 60
#define ONE_WIRE_delay_w_0_B                 10
#define ONE_WIRE_delay_w_1_A                 6
#define ONE_WIRE_delay_w_1_B                 64
#define ONE_WIRE_delay_r_A                   6
#define ONE_WIRE_delay_r_B                   9
#define ONE_WIRE_delay_r_C                   55
#define ONE_WIRE_delay_R_A                   480
#define ONE_WIRE_delay_R_B                   70
#define ONE_WIRE_delay_R_C                   410
//
//  PORT bits controller mask
//
#define ONE_WIRE_output          0
#define ONE_WIRE_input           1
#define ONE_WIRE_HIGH            1
#define ONE_WIRE_LOW             0
#define ONE_WIRE_set_bit_as      ONE_WIRE_CONFIG_BIT
#define ONE_WIRE_write_on_bit    ONE_WIRE_OUTPUT_BIT
#define ONE_WIRE_read_bit        ONE_WIRE_INPUT_BIT
    /**
     * init_one_wire_protocol
     * Inicializa el bit que pretende utilizarse para el protocolo
     */
    #define init_one_wire_protocol(){ONE_WIRE_read_bit=ONE_WIRE_HIGH;\
                                     ONE_WIRE_set_bit_as=ONE_WIRE_input;}
    /**
     * read_mode
     * Configura el bit en modo lectura
     */
    #define ONE_WIRE_read_mode()             {ONE_WIRE_read_bit=ONE_WIRE_HIGH;\
                                     ONE_WIRE_set_bit_as=ONE_WIRE_input;}
    /**
     * write_mode
     * Configura el bit en modo lectura
     * @return 
     */
    #define ONE_WIRE_write_mode()            {ONE_WIRE_read_bit=ONE_WIRE_HIGH;\
                                    ONE_WIRE_set_bit_as=ONE_WIRE_output;}
    /**
     * send_one
     * Permite transmitir un bit con valor de 1 en el puerto one wire
     */
    #define ONE_WIRE_send_one()              {ONE_WIRE_write_on_bit=ONE_WIRE_LOW;\
                                    __delay_us(ONE_WIRE_delay_w_1_A);\
                                    ONE_WIRE_write_on_bit=ONE_WIRE_HIGH;\
                                    __delay_us(ONE_WIRE_delay_w_1_B);\
                                    }
    /**
     * send_zero
     * Permite transmitir un bit con valor de 0 en el puerto one wire
     */
    #define ONE_WIRE_send_zero()             {ONE_WIRE_write_on_bit=ONE_WIRE_LOW;\
                                    __delay_us(ONE_WIRE_delay_w_0_A);\
                                    ONE_WIRE_write_on_bit=ONE_WIRE_HIGH;\
                                    __delay_us(ONE_WIRE_delay_w_0_B);}
    /**
     * reser
     * Envia el bit de reset por el puerto one wire
     */
    #define ONE_WIRE_reset()                 {ONE_WIRE_write_mode();\
                                    ONE_WIRE_write_on_bit=ONE_WIRE_LOW;\
                                    __delay_us(ONE_WIRE_delay_R_A);\
                                    ONE_WIRE_write_on_bit=ONE_WIRE_HIGH;\
                                    __delay_us(ONE_WIRE_delay_R_B);\
                                    ONE_WIRE_read_mode();}
    /**
     * Mascara de lectura del bit del puerto one wire
     * @return 0 o 1 segun el bit leido, no comprueba error
     */
    unsigned char ONE_WIRE__read_bit_();
    /**
     * read_one_wire_byte
     * Realiza una lectura de 8 bits del puerto one wire
     */
    unsigned char ONE_WIRE_read_byte();
    /**
     * send_byte
     * Envia un byte por el puerto one wire
     * @param   unsigned char, byte que se desea enviar
     * @param   bool, true: comprobar con reset, false: no comprobar con reset
     * @return true si el envio fue exitoso, false sino
     */
    bool    ONE_WIRE_send_byte(unsigned char,bool);
    /**
     * send_stream
     * Envia un arreglo de bytes por el puerto one wire
     * @param   const unsigned char*, arreglo de bytes que se desean transmitir 
     * @return true si el envio fue exitoso, false sino
     */
    bool    ONE_WIRE_send_stream(const unsigned char*);
    /**
     * send_reset
     * Envia un reset por el puerto one wire
     * @return true si algun dispositivo fue detectado, false sino
     */
    bool    ONE_WIRE_send_reset(void);    
    /**
     * Generador de CRC.
     * @param  unsigned char[] Arreglo de 8 bytes ordenados de 1 a 8
     * leidos de la ROM o del SCRATCHPAD
     * @return el CRC generado a partir de los datos leidos
     */
    unsigned char   ONE_WIRE_CRC_generator(unsigned char[]);
    /**
     * Verifica el CRC recibido con el CRC generado
     * @param  unsigned char[] Arreglo de 9 bytes ordenados de 1 a 9
     * leidos de la ROM o del SCRATCHPAD
     * @return true, si es no ha habido error, false, si el CRC no coincide.
     */
    bool    ONE_WIRE_verify_CRC(unsigned char[]);
    
#endif