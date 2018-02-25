/*
 * File:   1_wire_protocol.c
 * Author: josepablocruzbaas
 *
 * Created on June 18, 2017, 9:43 PM
 */

#include "one_wire_ctl.h"

unsigned char ONE_WIRE__read_bit_(){
    unsigned char _bit_ = 0;
    ONE_WIRE_write_mode();
    ONE_WIRE_write_on_bit = ONE_WIRE_LOW;
    __delay_us(ONE_WIRE_delay_r_A);
    ONE_WIRE_write_on_bit = ONE_WIRE_HIGH;
    ONE_WIRE_read_mode();
    __delay_us(ONE_WIRE_delay_r_B);
    _bit_ = ONE_WIRE_read_bit;
    __delay_us(ONE_WIRE_delay_r_C);
    return _bit_;
}

unsigned char ONE_WIRE_read_byte(){
    ONE_WIRE_read_mode();
    unsigned char data_byte_buffer=0;
    unsigned char bit_counter;
    for(bit_counter=0;bit_counter<8;bit_counter++){
        if(ONE_WIRE__read_bit_()){
            data_byte_buffer >>= 1;    //Escribir uno
            data_byte_buffer += 128;  
        }else{
            data_byte_buffer >>= 1;      //Escribir cero
        }
    }
    return data_byte_buffer;
}
bool    ONE_WIRE_send_reset(){
    bool reset_state = false;
    ONE_WIRE_reset();
    reset_state = ONE_WIRE_read_bit;
    __delay_us(ONE_WIRE_delay_R_C);
    return !reset_state;
}
bool    ONE_WIRE_send_byte(unsigned char data_byte,bool detect){
    //Es necesario poner en modo escritura para enviar cualquier bit
    unsigned char bit_counter;
    ONE_WIRE_write_mode();
    if(detect){
        if(!ONE_WIRE_send_reset())//Comprobar si existe slave
            return false;
        //send_reset configura en modo lectura por defecto
        ONE_WIRE_write_mode();
    }
    for(bit_counter=0;bit_counter<8;bit_counter++){
        //Primero el LSB
        if(data_byte & 0x01){//Leer el 1er bit primero
            ONE_WIRE_send_one();//Escribir 1
        }else{
            ONE_WIRE_send_zero();//Escribir 0
        }
        data_byte >>= 1;//Siguiente bit
    }
    return true;
}
bool    ONE_WIRE_send_stream(const unsigned char* data_bytes){
    while(*data_bytes){
        ONE_WIRE_send_byte(*data_bytes++,false);//Enviamos byte sin comprobar
    }
    return true;
}

unsigned char   ONE_WIRE_CRC_generator(unsigned char[]){
    return 0x00;
}

bool    ONE_WIRE_verify_CRC(unsigned char[]){
    return true;
}