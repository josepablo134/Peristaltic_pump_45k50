#include "app.h"

void interrupt ISR(){
    if(TMR0IF){//clock interrupt
        APP_STATE_REG |= 0x1;
        TMR0=336;
        TMR0IF = 0;
        TMR0_COUNTER++;
        if(TMR0_COUNTER==31){
            //clock event
            TMR0_COUNTER=0;
            clock_event();
        }
    }
    if(ADIF){//ADC interrupt
        ADIF = 0;
        if(APP_STATE_REG & 0x4){
            if(APP_STATE_REG & 0x2){
                PUMP((ADRESH<<8)+(ADRESL&0xFF));
            }else{
                PUMP(0);
            }
        }else{
                PUMP(0);
        }
    }
}

void init_app(){
    recovery_default_config();
    
    init_beep();
    __delay_ms(100);
    end_beep();
    __delay_ms(10);
    init_beep();
    __delay_ms(100);
    end_beep();
    main_menu();
    lcd_clear();
    lcd_puts("ERROR!");
}

void recovery_default_config(){
    PUBLIC_TEMP = EEPROM_read(0x00);
    MAX_TEMP    = EEPROM_read(0x01);
    MIN_TEMP    = EEPROM_read(0x02);
    VOLUME      = EEPROM_read(0x03);
    clock.HRS   = EEPROM_read(0x04);
    clock.MIN   = EEPROM_read(0x05);
    clock.SEC   = EEPROM_read(0x06);
}

void save_default_config(){
    EEPROM_write(PUBLIC_TEMP,0x00);
    EEPROM_write(MAX_TEMP   ,0x01);
    EEPROM_write(MIN_TEMP   ,0x02);
    EEPROM_write(VOLUME     ,0x03);
    EEPROM_write(clock.HRS  ,0x04);
    EEPROM_write(clock.MIN  ,0x05);
    EEPROM_write(clock.SEC  ,0x06);
}

unsigned char get_temp(){
    char temp_buffer = 0;
    
    ONE_WIRE_send_byte(ONE_WIRE_CMD_SKIP_ROM,true);
    ONE_WIRE_send_byte(ONE_WIRE_CMD_CONVERT_TEMP,false);
    while(!ONE_WIRE__read_bit_()){}//Esperar a que finalice
    ONE_WIRE_send_byte(ONE_WIRE_CMD_SKIP_ROM,true);
    ONE_WIRE_send_byte(ONE_WIRE_CMD_READ_SCRATCHPAD,false);
    temp_buffer =  (ONE_WIRE_read_byte())>>4;
    temp_buffer += (ONE_WIRE_read_byte())<<4;
    
    return temp_buffer;//La temperatura puede ser negativa
}

void init_clock(){
    clock.SEC = 0;
    clock.MIN = 0;
    clock.HRS = 0;
}

void print_clock(){
    lcd_putch(HexToChar[ (clock.HRS/10)%10 ]);
    lcd_putch(HexToChar[ clock.HRS%10 ]);
    lcd_putch(':');
    lcd_putch(HexToChar[ (clock.MIN/10)%10 ]);
    lcd_putch(HexToChar[ clock.MIN%10 ]);
    lcd_putch(':');
    lcd_putch(HexToChar[ (clock.SEC/10)%10 ]);
    lcd_putch(HexToChar[ clock.SEC%10 ]);
}

void set_sec(unsigned char SEC){clock.SEC = SEC%60;}
void set_min(unsigned char MIN){clock.MIN = MIN%60;}
void set_hrs(unsigned char HRS){clock.HRS = HRS%24;}

void clock_event(){
    if(clock.SEC==0){
        clock.SEC = 59;
        if(clock.MIN==0){
            clock.MIN = 59;
            if(clock.HRS==0){
                clock.SEC = 0;
                clock.MIN = 0;
                clock.SEC = 0;
                //Desactivar todos los servicios!
                stop_tmr0();
                DISABLE_PUMP();
                DISABLE_EVENTS();
                DISABLE_PUMP();
                DISABLE_ADC_INT();
                lcd_clear();
                lcd_puts("FINALIZAMOS!");
                __delay_ms(500);
                asm("RESET");
            }else{
                clock.SEC--;
                clock.MIN--;
                clock.HRS--;
            }
        }else{
            clock.SEC--;
            clock.MIN--;
        }
    }else{
        clock.SEC--;
    }
    lcd_goto(0x04);
    print_clock();
    lcd_goto(0x49);
    if(LAST_TEMP>127){
        lcd_putch('-');
        lcd_put_temp(~LAST_TEMP);
    }else{
        lcd_put_temp(LAST_TEMP);
    }
}


//
//  INTERFACES DE USUARIO
//
void config_system(){
    unsigned char key_buffer = 0;
    DISABLE_EVENTS();
    while(1){
        lcd_clear();
        lcd_puts(" A    B   #  C");
        lcd_goto(0x40);
        lcd_puts("TIME TEMP OK <-");
        key_buffer = keypad_getch();
        clear_keypad_buffer();
        if(key_buffer == 'A'){
            get_time();
            continue;
        }else if(key_buffer == 'B'){
            get_temp_range();
            continue;
        }else if(key_buffer == 'C'){
            lcd_clear();
            lcd_puts_text_align("RECUP CONFIG?",LCD_ALIGN_CENTER);
            lcd_goto(0x40);
            lcd_puts_text_align("CONFIRMAR",LCD_ALIGN_CENTER);
            keypad_getch();
            recovery_default_config();
        }else if(key_buffer == '#'){
            lcd_clear();
            lcd_puts_text_align("LISTO",LCD_ALIGN_CENTER);
            lcd_goto(0x40);
            lcd_puts_text_align("CONFIRMAR",LCD_ALIGN_CENTER);
            keypad_getch();
            break;
        }
    }
    save_default_config();
}

void get_temp_range(){
    unsigned char counter = 0;
    lcd_clear();
    lcd_puts_text_align("TEMPERATURE",LCD_ALIGN_CENTER);
    while(1){
        lcd_goto(0x47 + counter);
        char_buffer[counter] = keypad_getch();
        if(char_buffer[counter] == '#'){
            break;
        }
        if(char_buffer[counter] != 'D'){
            if(char_buffer[counter]>='0' && char_buffer[counter]<='9'){
                lcd_putch(char_buffer[counter]);
                counter++;
                if(counter==2){break;}
            }
        }else{
            if(counter==0){continue;}
            char_buffer[counter] = 0;
            counter--;
        }
    }
    if(char_buffer[0]=='#'){//No recibí nada
        counter = 0;
    }else if(char_buffer[1]=='#'){//Solo recibí un numero
        counter = (char_buffer[0]-'0');
    }else{//Recibi todos los caracteres
        counter = (char_buffer[0]-'0')*10 +\
                 (char_buffer[1]-'0');
    }
    
    lcd_clear();
    lcd_puts("TEMP : ");
    lcd_putch( (counter/10)%10 + '0' );
    lcd_putch( (counter)%10 + '0' );
    lcd_goto(0x40);
    lcd_puts("CONTINUAR? [A]");
    
    if(keypad_getch() == 'A'){
            PUBLIC_TEMP = counter;
            MAX_TEMP = counter+TEMP_RANGE_MAX;
            if( counter <= TEMP_RANGE_MIN ){
                MIN_TEMP = 0;
            }else{
                MIN_TEMP = counter-TEMP_RANGE_MIN;
            }
    }else{
        lcd_clear();
        lcd_puts_text_align("CANCELADO",LCD_ALIGN_CENTER);
        __delay_ms(500);
    }
    lcd_clear();
}
void get_volume(){
    unsigned char counter = 0;
    lcd_clear();
    lcd_puts_text_align("VOLUME",LCD_ALIGN_CENTER);
    
    while(1){
        lcd_goto(0x46 + counter);
        char_buffer[counter] = keypad_getch();
        if(char_buffer[counter] == '#'){
            break;
        }
        if(char_buffer[counter] != 'D'){
            if(char_buffer[counter]>='0' && char_buffer[counter]<='9'){
                lcd_putch(char_buffer[counter]);
                counter++;
                if(counter==3){break;}
            }
        }else{
            if(counter==0){continue;}
            char_buffer[counter] = 0;
            counter--;
        }
    }
    if(char_buffer[0]=='#'){//No recibí nada
        counter = 0;
    }else if(char_buffer[1]=='#'){//Solo recibí un numero
        counter = (char_buffer[0]-'0');
    }else if(char_buffer[2]=='#'){//Solo recibí 2 numeros
        counter = (char_buffer[0]-'0')*10 +\
                 (char_buffer[1]-'0');
    }else{//Recibi todos los caracteres
        counter = (char_buffer[0]-'0')*100 +\
                 (char_buffer[1]-'0')*10 +\
                 (char_buffer[2]-'0');
    }
    counter %= 100;
    lcd_clear();
    lcd_puts("VOL : ");
    lcd_putch( (counter/100)%10 + '0' );
    lcd_putch( (counter/10)%10 + '0' );
    lcd_putch( (counter)%10 + '0' );
    lcd_goto(0x40);
    lcd_puts("CONTINUAR? [A]");
    
    if(keypad_getch() == 'A'){
        VOLUME = counter;
    }else{
        lcd_clear();
        lcd_puts_text_align("CANCELADO",LCD_ALIGN_CENTER);
        __delay_ms(500);
    }
    lcd_clear();
}
void get_time(){
    unsigned char counter = 0;
    lcd_clear();
    lcd_puts_text_align("CRONOMETRO",LCD_ALIGN_CENTER);
    __delay_ms(900);
    lcd_clear();
    lcd_puts_text_align("HH:MM:SS",LCD_ALIGN_CENTER);
    //Obtener hora
    while(1){
        lcd_goto(0x44 + counter);
        char_buffer[counter] = keypad_getch();
        if(char_buffer[counter] == '#'){
            if(counter == 0){
                break;
            }else{
                break;
            }
        }
        if(char_buffer[counter] != 'D'){
            if(char_buffer[counter]>='0' && char_buffer[counter]<='9'){
                lcd_putch(char_buffer[counter]);
                counter++;
                if(counter==2){break;}
            }
        }else{
            if(counter==0){continue;}
            char_buffer[counter] = 0;
            counter--;
        }
    }
    lcd_putch(':');
    //Obtener minutos
    counter = 2;
    while(1){
        lcd_goto(0x44 + counter + 1);
        char_buffer[counter] = keypad_getch();
        if(char_buffer[counter] == '#'){
            if(counter == 0){
                break;
            }else{
                break;
            }
        }
        if(char_buffer[counter] != 'D'){
            if(char_buffer[counter]>='0' && char_buffer[counter]<='9'){
                lcd_putch(char_buffer[counter]);
                counter++;
                if(counter==4){break;}
            }
        }else{
            if(counter==2){continue;}
            char_buffer[counter] = 0;
            counter--;
        }
    }
    lcd_putch(':');
    //Obtener segundos
    counter = 4;
    while(1){
        lcd_goto(0x44 + counter + 2);
        char_buffer[counter] = keypad_getch();
        
        if(char_buffer[counter] == '#'){
            break;
        }
        if(char_buffer[counter] != 'D'){
            if(char_buffer[counter]>='0' && char_buffer[counter]<='9'){
                lcd_putch(char_buffer[counter]);
                counter++;
                if(counter==6){break;}
            }
        }else{
            if(counter==4){continue;}
            char_buffer[counter] = 0;
            counter--;
        }
    }
    lcd_clear();
    //
    //  Presentar horas
    //
    if(char_buffer[0]=='#'){
        lcd_puts("00");
    }else if(char_buffer[1]=='#'){
        lcd_putch('0');
        lcd_putch(char_buffer[0]);
    }else{
        lcd_putch(char_buffer[0]);
        lcd_putch(char_buffer[1]);    
    }
    lcd_putch(':');
    //
    //  Presentar minutos
    //
    if(char_buffer[2]=='#'){
        lcd_puts("00");
    }else if(char_buffer[3]=='#'){
        lcd_putch('0');
        lcd_putch(char_buffer[2]);
    }else{
        lcd_putch(char_buffer[2]);
        lcd_putch(char_buffer[3]);    
    }
    
    lcd_putch(':');
    //
    //  Presentar segundos
    //
    if(char_buffer[4]=='#'){
        lcd_puts("00");
    }else if(char_buffer[5]=='#'){
        lcd_putch('0');
        lcd_putch(char_buffer[4]);
    }else{
        lcd_putch(char_buffer[4]);
        lcd_putch(char_buffer[5]);    
    }
    lcd_goto(0x40);
    lcd_puts_text_align("CONTINUAR? [A]",LCD_ALIGN_CENTER);
    if(keypad_getch() == 'A'){
        //Parsear segundos
        if(char_buffer[4]=='#'){//No recibí nada
            set_sec(0);
        }else if(char_buffer[5]=='#'){//Recibí solo un caracter
            set_sec(char_buffer[4]-'0');
        }else{//Recibí todos los caracteres
            set_sec( (char_buffer[4]-'0')*10 +\
                         char_buffer[5]-'0');
        }
        //Parsear minutos
        if(char_buffer[2]=='#'){//No recibí nada
            set_min(0);
        }else if(char_buffer[3]=='#'){//Recibí solo un caracter
            set_min( char_buffer[2]-'0');
        }else{//Recibí todos los caracteres
            set_min( (char_buffer[2]-'0')*10 +\
                         char_buffer[3]-'0' );
        }
        //Parsear horas
        if(char_buffer[0]=='#'){//No recibí nada
            set_hrs(0);
        }else if(char_buffer[1]=='#'){//Recibí solo un caracter
            set_hrs(char_buffer[0]-'0');
        }else{//Recibí todos los caracteres
            set_hrs( (char_buffer[0]-'0')*10 +\
                         char_buffer[1]-'0');
        }
    }else{
        lcd_clear();
        lcd_puts_text_align("CANCELADO",LCD_ALIGN_CENTER);
        __delay_ms(500);
    }
    lcd_clear();
}

void main_menu(){
MAIN_MENU:
    lcd_clear();
    lcd_puts("[A]  COMENZAR   ");
    lcd_goto(0x40);
    lcd_puts("[B]  CONFIGURAR ");
    while(1){
        switch(keypad_getch()){
            case 'A':
                goto INIT_THERAPY;
            case 'B':
                config_system();
                goto MAIN_MENU;
            default:
                continue;
        }
    }
INIT_THERAPY:
    lcd_clear();
    lcd_puts("[A]  COMENZAR YA");
    lcd_goto(0x40);
    lcd_puts("[B]  CONF ANTES");
    switch(keypad_getch()){
        case 'A':
            goto START_NOW;
        case 'B':
            goto CONFIG_THERAPY;
        default:
            goto INIT_THERAPY;
    }
START_NOW:
    //Activar todos los servicios
    lcd_clear();
    TMR0_COUNTER = 0;
    start_tmr0();
    //PUMP(512);
    ENABLE_EVENTS();
  //ENABLE_ADC_INT();
    ENABLE_PUMP();
    read_adc_with_int();
    lcd_goto(0x40);
    lcd_puts("TEMP :");
    while(1){
        LAST_TEMP = get_temp();
        if(LAST_TEMP>127){// si es 0 o menor
            DISABLE_PUMP();
            DISABLE_ADC_INT();
            PUMP(0);
        }else if(LAST_TEMP>MAX_TEMP){//Si es mayor al rango configurado
            read_adc_with_int();
            ENABLE_ADC_INT();
            ENABLE_PUMP();
        }else{
            //Si esta dentro del rango
            //if(LAST_TEMP>MIN_TEMP && LAST_TEMP<MAX_TEMP ){
            if(LAST_TEMP < MAX_TEMP){
                DISABLE_PUMP();
                DISABLE_ADC_INT();
                PUMP(0);
            }
        }
    }
CONFIG_THERAPY:
    lcd_clear();
    lcd_puts("[A] TIEMPO      ");
    lcd_goto(0x40);
    lcd_puts("[B] TEMPERATURA ");
    clear_keypad_buffer();
    switch(keypad_getch()){
        case 'A':
            get_time();
            goto CONFIG_THERAPY;
        case 'B':
            get_temp_range();
            goto CONFIG_THERAPY;
        case '#':
            lcd_clear();
            lcd_puts_text_align("GUARDAR? [A]",LCD_ALIGN_CENTER);
            if(keypad_getch() == 'A'){
                lcd_clear();
                save_default_config();
                lcd_puts_text_align("GUARDADO",LCD_ALIGN_CENTER);
                __delay_ms(500);
            }
            lcd_clear();
            lcd_puts_text_align("COMENZAR? [A]",LCD_ALIGN_CENTER);
            if(keypad_getch() == 'A'){
                goto START_NOW;
            }
        default:
            goto CONFIG_THERAPY;
    }
    goto MAIN_MENU;
}