/*
 * File:   keypad_ctl.c
 * Author: josepablocruzbaas
 *
 * Created on June 18, 2017, 9:42 PM
 */
#include "keypad_ctl.h"

unsigned char keypad_getch(){
        volatile unsigned char key_1;
        volatile unsigned char key_2;
        volatile unsigned char port_buffer=0x00;
        volatile unsigned char latch_change;
        //Esperamos a que se precione una tecla
        while( (KEYBOARD_P&0xF0) == 0xF0 ){}
        debounce();
        port_buffer = KEYBOARD_P;
        //
        //Almacenamos la fila
        //
        if(!kb_bits.bit0){
            key_1 = 3;
        }else if(!kb_bits.bit1){
            key_1 = 2;
        }else if(!kb_bits.bit2){
            key_1 = 1;
        }else if(!kb_bits.bit3){
            key_1 = 0;
        }
        
        //
        //  Almacenamos la columna
        //
        port_buffer &= 0xF0;
        latch_change = 0x0E;
        KEYBOARD_L = latch_change;
        for(unsigned char i=0;i<4;i++){
            //Si coincide con la fila
            if( port_buffer == (KEYBOARD_P&0xF0) ){
                key_2 = 3-i;
                break;
            }
            //Probamos con el siguiente bit
            latch_change <<=1 ;
            latch_change +=1;
            KEYBOARD_L = latch_change;
        }
        
        //Restauramos el puerto
        KEYBOARD_L = 0x00;
        init_beep();
        __delay_ms(100);
        end_beep();
        //Esperar a que deje de presionar la tecla
        while( (KEYBOARD_P&0xF0) != 0xF0 ){}
        //Recuperamos la lectura
        return keymask[key_1][key_2];
}
unsigned char keypad_read(){
    volatile unsigned char key_1;
    volatile unsigned char key_2;
    volatile unsigned char port_buffer=0x00;
    volatile unsigned char latch_change;
    debounce();
    port_buffer = KEYBOARD_P;
    //
    //Almacenamos la fila
    //
    if(!kb_bits.bit0){
        key_1 = 3;
    }else if(!kb_bits.bit1){
        key_1 = 2;
    }else if(!kb_bits.bit2){
        key_1 = 1;
    }else if(!kb_bits.bit3){
        key_1 = 0;
    }

    //
    //  Almacenamos la columna
    //
    port_buffer &= 0xF0;
    latch_change = 0x0E;
    KEYBOARD_L = latch_change;
    for(unsigned char i=0;i<4;i++){
        //Si coincide con la fila
        if( port_buffer == (KEYBOARD_P&0xF0) ){
            key_2 = 3-i;
            break;
        }
        //Probamos con el siguiente bit
        latch_change <<=1 ;
        latch_change +=1;
        KEYBOARD_L = latch_change;
    }

    //Restauramos el puerto
    KEYBOARD_L = 0x00;
    //Recuperamos la lectura
    return keymask[key_1][key_2];
}