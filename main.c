/*
 * File:   main.c
 * Author: Jonathan Guerrero
 *
 * Created on 27 de julio de 2020, 04:39 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h> 

#define _XTAL_FREQ 4000000
int temp;
char str[10];

int numero;
int dec;
int uni;
int tiempo = 10;
int portc_num;
uint8_t segundos = 58;
uint8_t minutos = 0;
uint8_t parte_baja;
uint8_t parte_alta;
uint8_t parteb_baja;

int conta = 0;

void __interrupt() tcInt(void){     //only process timer0-triggered interrupts

   if(INTCONbits.TMR0IE  && INTCONbits.TMR0IF)
   {
        conta++;
        INTCONbits.T0IF=0;
        TMR0=62;
        if(conta==20)
        {
            segundos++;
            conta = 0;
            //PORTB = segundos;
            PORTBbits.RB7 = !PORTBbits.RB7 ;
            eeprom_write (0x20, segundos);
        }
        if(segundos == 60)
        {
        minutos++;
        segundos = 0;
        conta = 0;
        eeprom_write (0x21, minutos);
        }
   }
}

void descomponerNumero(numero)
{
    dec = numero /10;
    uni = numero % 10;
}
void temperatura()
{
    ADCON0bits.GO   =   1;
    while(ADCON0bits.GO);
    temp = ADRESH * 0.59;
}
void main(void) {
    
    TRISA = 1;
    TRISB = 1;
    TRISC = 0;
    TRISD = 0;
    
    ADCON0 = 0X81;
    ADCON1 = 0X45;      //ajustar voltaje de referencia Vref+
    PORTC = 0;
    PORTD = 0;
    OPTION_REG = 0x07;
    INTCON  =   0X80;
    TMR0 = 62;
    
    while(1)
    {
        INTCON  =   0X80;
        temperatura();
        descomponerNumero(temp);
            parte_alta = 2;
            parte_baja = dec;
            PORTC = (parte_alta << 4) | parte_baja;
            __delay_ms(tiempo);        
            parte_alta = 1;
            parte_baja = uni;
            PORTC = (parte_alta << 4) | parte_baja;
            __delay_ms(tiempo);
            
            //segundos = eeprom_read(0x20);
            //minutos = eeprom_read(0x21);
            descomponerNumero(segundos);
        parte_alta = 7;
        parte_baja = uni;
        PORTD = (parte_alta << 4) | parte_baja;
        __delay_ms(tiempo);
        parte_alta = 11;
        parte_baja = dec;
        PORTD = (parte_alta << 4) + parte_baja;
                
        descomponerNumero(minutos);
        parte_alta = 13;
        parte_baja = uni;
        PORTD = (parte_alta << 4) | parte_baja;
        __delay_ms(5);
        parte_alta = 14;
        parte_baja = dec;
        PORTD = (parte_alta << 4) | parte_baja;
        __delay_ms(5);
            
        while(temp >= 80)
        {
            INTCON  =   0XA0;
            temperatura();
            descomponerNumero(temp);
            parte_alta = 2;
            parte_baja = dec;
            PORTC = (parte_alta << 4) | parte_baja;
            __delay_ms(tiempo);        
            parte_alta = 1;
            parte_baja = uni;
            PORTC = (parte_alta << 4) | parte_baja;
            __delay_ms(tiempo); 
            
            descomponerNumero(segundos);
        parte_alta = 7;
        parte_baja = uni;
        PORTD = (parte_alta << 4) | parte_baja;
        __delay_ms(tiempo);
        parte_alta = 11;
        parte_baja = dec;
        PORTD = (parte_alta << 4) + parte_baja;
                
        descomponerNumero(minutos);
        parte_alta = 13;
        parte_baja = uni;
        PORTD = (parte_alta << 4) | parte_baja;
        __delay_ms(5);
        parte_alta = 14;
        parte_baja = dec;
        PORTD = (parte_alta << 4) | parte_baja;
        __delay_ms(5);
        }

    }
    return;
}
