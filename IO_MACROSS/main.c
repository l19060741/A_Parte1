/**********************************************************************************************************************
// MAIN
 // FileName:        main.c
 // Dependencies:    msp.h, stdint.h
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V10 TI
 // Company:         TecNM /IT Chihuahua
 // Description:        UTILIZACION DE MACROS A REGISTROS DE PERIFERICO GPIO.
 // Authors:         ALFREDO CHACON
 // Updated:         03/2021
 //Nota: no se están usando las estructuras definidas en los sourcefiles dados por el BSP de TI, las macros declaradas
 //      se incializan con las direcciones de memoria de los registros de los perifericos en cuestión
 ************************************************************************************************************************/
/************************************************************************************************
 * * Copyright (C) 2021 by Alfredo Chacon - TecNM /IT Chihuahua
 *
 * Se permite la redistribucion, modificacion o uso de este software en formato fuente o binario
 * siempre que los archivos mantengan estos derechos de autor.
 * Los usuarios pueden modificar esto y usarlo para aprender sobre el campo de software embebido.
 * Alfredo Chacon y el TecNM /IT Chihuahua no son responsables del mal uso de este material.
 *************************************************************************************************/
/************************************************
  HEADER FILES
 ************************************************/
#include<stdint.h>
#include "msp.h"
/************************************************
  DEFINICIONES DE CONSTANTES Y/O MASCARAS
 ************************************************/
#define RETARDO (100000)
#define RETARDO2 (10000)
#define PIN0    (0x01)  //definimos el numero Hexadecimal segun el Pin del PUERTO
#define PIN1    (0x02)
#define PIN2    (0x04)
#define FALSE   0
#define TRUE    1
/************************************************
  DEFINICIONES DE MASCARAS Y MACROS PARA ACCESO A MEMORIA
 ************************************************/

//  MACRO FUNCIONES PARA LEER MEMORIA
#define HWREG8(x)         (*((volatile uint8_t *)(x)))
#define HWREG16(x)        (*((volatile uint16_t *)(x)))
#define HWREG32(x)        (*((volatile uint32_t *)(x)))
// Las funciones macros de preprocesador actúan como una búsqueda y reemplazan el software justo antes de la compilación. Cualquier cosa que escriba se sustituye directamente en su código. Esto nos permite saltar llamadas y regresar de la función,
#define P1_IN  (HWREG8(0x040004c00))
#define P1_OUT (HWREG8(0x040004c02))
#define P1_DIR (HWREG8(0x040004c04))
#define P1_REN (HWREG8(0x040004c06))

#define P2_OUT (HWREG8(0x040004c03))
#define P2_DIR (HWREG8(0x040004c05))

/************************************************
    PROTOTIPO DE FUNCION
************************************************/
extern uint8_t Gpio_Pin_in( uint_fast16_t pin_);

/*****************************************************************************
 * Function: MAIN
 * Preconditions: NINGUNA.
 * Overview:
 * Input: NINGUNA.
 * Output: NINGUNA
 *
 *****************************************************************************/
void main(void)
{
    /************************************************
       DECLARACION DE VARIABLES
    ************************************************/
    uint32_t i;
    uint8_t bandera = 0;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // DETIENE EL TIMER DEL WATCHDOG

    /************************************************
            DEREFERENCIA CON MACROS
     ************************************************/
    P1_DIR &= ~0x10;                    //PIN P1.4 COMO ENTRADA
    P1_REN |= 0x10;                     //HABILITAMOS RESISTENCIA
    P1_OUT |= 0x10;                     //PULL UP

    P2_DIR |= 0x07;                     //PIN P2.0, P2.1, P2.2  COMO SALIDA, LEDS RGB
    P2_OUT &= ~0x07;                    // APAGADOS
                                        // PIN P1.0 COMO SALIDA,  --> LED <--

    while(1){


        if ( Gpio_Pin_in(0x10) != 1)           // ENTRA AL PRESIONAR EL SWITCH DEL PUERTO P1.4 (CUANDO ESTA EN BAJO)
        {
            if(bandera == FALSE)
                {
                    bandera = TRUE;
                    while (bandera==TRUE){                          //BANDERA EN ALTO
                        P2_OUT &= ~0x07;                            //APAGA BLUE
                        P2_OUT |= 0x01;                             //ENCIENDE RED
                        for(i=RETARDO; i>0 ;i--);
                        P2_OUT &= ~0x01;                            //APAGA RED
                        P2_OUT |= 0x02;                             //ENCIENDE GREEN
                        for(i=RETARDO; i>0 ;i--);
                        P2_OUT &= ~0x02;                            //APAGA GREEN
                        P2_OUT |= 0x04;                             //ENCIENDE BLUE
                        for(i=RETARDO; i>0 ;i--);
                     if ( Gpio_Pin_in(0x10) != 1) bandera=FALSE;    // ENTRA CUANDO PRESIONAMOS BOTON DE P1.4
                     while( Gpio_Pin_in(0x10) != 1);                //MIENTRAS ESTE PRESIONADO EL BOTON
                    }

                }
              P2_OUT &= ~0x07;        //APAGA TODOS

        }
        for(i=RETARDO2; i>0 ;i--);
    }
}

/*****************************************************************************
 * Function: Gpio_Pin_in
 * Preconditions: PUERTO1 SELECCIONADO.
 * Overview: LEE EL PIN DESEADO
 * Input: pin_.
 * Output: 0/1
 *
 *****************************************************************************/
uint8_t Gpio_Pin_in(uint_fast16_t pin_)
{
    uint_fast16_t inputPinValue;
    inputPinValue = P1_IN & (pin_);     // LEE EL REGISTRO QUE INDICA EL VALOR DE ENTRADA DE UN PIN
    if (inputPinValue > 0)              // RETORNA EL VALOR 1 o 0
        return (0x01);
    return (0x00);
}
