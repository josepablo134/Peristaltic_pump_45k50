#include "EEPROM_ctl.h"

bool EEPROM_write(unsigned char data,unsigned char DIR){
    unsigned char GIE_STATE = GIE;//GUARDAR GIE CONFIG.
    EEADR = DIR;
    EEDATA = data;
    EECON1bits.EEPGD = EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;//Activar escritura
    
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;//Comenzar a escribir
    
    GIE = GIE_STATE;//RESTAURAR GIE CONFIG.
    
    while(!EEIF){}//Esperar a que finalice escritura
    EEIF = 0;//Limpiar interrupción
    EECON1bits.WREN = 0;//Desactivar escritura
    
    return (!EECON1bits.WRERR);
}

unsigned char EEPROM_read(unsigned char DIR){
    EEADR = DIR;
    EECON1bits.EEPGD = EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;//Activar lectura
    
    return EEDATA;//Devolver lectura
}