
/**********************************************************************************************************************
 // FileName:        main.c
 // Dependencies:    msp.h, stdint.h
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V10 TI
 // Company:         TecNM /IT Chihuahua
 // Description:     ACCESO A MEMORIA DE REGISTROS DE PERIFERICO GPIO CON ESTRUCTURAS.
 // Authors:         ALFREDO CHACON
 // Updated:         03/2021
 //Nota: no se estan usando las estructuras definidas en los sorucefiles dados por el BSP de TI, los punteros declarados
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
//#include<stdio.h>
/************************************************
  DEFINICIONES DE CONSTANTES Y/O MASCARAS
 ************************************************/
#define PERI_BASE                   ((uint32_t) 0x40000000)
#define P1_Y_P2_BASE                (PERI_BASE + 0x0000004c00)
#define PUERTO_P1                   ((Struct_Puerto_impar *) P1_Y_P2_BASE)
#define PUERTO_P2                   ((Struct_Puerto_par *) P1_Y_P2_BASE)

#define RETARDO (100000)
#define RETARDO2 (10000)
#define PIN0    (0x01)  //definimos el numero Hexadecimal segun el Pin del PUERTO
#define PIN1    (0x02)
#define PIN2    (0x04)
#define PIN4    (0x10)
#define FALSE 0U
#define TRUE 1U
#define GIT 0
/************************************************
  DEFINICIONES DE ESTRUCTURAS
 ************************************************/
typedef struct {
  __I uint8_t IN;                                                                 /*!< Port Input */
  uint8_t RESERVED0;
  __IO uint8_t OUT;                                                               /*!< Port Output */
  uint8_t RESERVED1;
  __IO uint8_t DIR;                                                               /*!< Port Direction */
  uint8_t RESERVED2;
  __IO uint8_t REN;                                                               /*!< Port Resistor Enable */
  uint8_t RESERVED3;
  __IO uint8_t DS;                                                                /*!< Port Drive Strength */
  uint8_t RESERVED4;
  __IO uint8_t SEL0;                                                              /*!< Port Select 0 */
  uint8_t RESERVED5;
  __IO uint8_t SEL1;                                                              /*!< Port Select 1 */
  uint8_t RESERVED6;
  __I  uint16_t IV;                                                               /*!< Port Interrupt Vector Value */
  uint8_t RESERVED7[6];
  __IO uint8_t SELC;                                                              /*!< Port Complement Select */
  uint8_t RESERVED8;
  __IO uint8_t IES;                                                               /*!< Port Interrupt Edge Select */
  uint8_t RESERVED9;
  __IO uint8_t IE;                                                                /*!< Port Interrupt Enable */
  uint8_t RESERVED10;
  __IO uint8_t IFG;                                                               /*!< Port Interrupt Flag */
  uint8_t RESERVED11;
} Struct_Puerto_impar;

typedef struct {
  uint8_t RESERVED0;
  __I uint8_t IN;                                                                 /*!< Port Input */
  uint8_t RESERVED1;
  __IO uint8_t OUT;                                                               /*!< Port Output */
  uint8_t RESERVED2;
  __IO uint8_t DIR;                                                               /*!< Port Direction */
  uint8_t RESERVED3;
  __IO uint8_t REN;                                                               /*!< Port Resistor Enable */
  uint8_t RESERVED4;
  __IO uint8_t DS;                                                                /*!< Port Drive Strength */
  uint8_t RESERVED5;
  __IO uint8_t SEL0;                                                              /*!< Port Select 0 */
  uint8_t RESERVED6;
  __IO uint8_t SEL1;                                                              /*!< Port Select 1 */
  uint8_t RESERVED7[9];
  __IO uint8_t SELC;                                                              /*!< Port Complement Select */
  uint8_t RESERVED8;
  __IO uint8_t IES;                                                               /*!< Port Interrupt Edge Select */
  uint8_t RESERVED9;
  __IO uint8_t IE;                                                                /*!< Port Interrupt Enable */
  uint8_t RESERVED10;
  __IO uint8_t IFG;                                                               /*!< Port Interrupt Flag */
  __I uint16_t IV;                                                                /*!< Port Interrupt Vector Value */
} Struct_Puerto_par;


/************************************************
    PROTOTIPO DE FUNCION
************************************************/
extern uint8_t GPIO_PIN_IN( uint_fast16_t pin_);

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
            DEREFERENCIA DE APUNTADORES
        ************************************************/
        PUERTO_P1->DIR &= ~0x10;                               //PIN P1.4 COMO ENTRADA
        PUERTO_P1->REN |= 0x10;                                //HABILITAMOS RESISTENCIA
        PUERTO_P1->OUT |= 0x10;                                //PULL UP

        PUERTO_P2->DIR |= 0x07;                                //PIN P2.0, P2.1, P2.2  COMO SALIDA, LEDS RGB
        PUERTO_P2->OUT= ~0x07;                               // APAGADOS
        while(1){
            if ( GPIO_PIN_IN(0x10) != 1)                // ENTRA AL PRESIONAR EL SWITCH DEL PUERTO P1.4 (CUANDO ESTA EN BAJO)
            {
                if(bandera == FALSE)
                    {
                        bandera = TRUE;                                 //BANDERA EN ALTO
                        while (bandera==TRUE){
                            PUERTO_P2->OUT &= ~0x07;                           //APAGA BLUE
                            PUERTO_P2->OUT |= 0x01;                            //ENCIENDE RED
                            for(i=RETARDO; i>0 ;i--);
                            PUERTO_P2->OUT &= ~0x01;                           //APAGA RED
                            PUERTO_P2->OUT |= 0x02;                            //ENCIENDE GREEN
                            for(i=RETARDO; i>0 ;i--);
                            PUERTO_P2->OUT &= ~0x02;                           //APAGA GREEN
                            PUERTO_P2->OUT |= 0x04;                            //ENCIENDE BLUE
                            for(i=RETARDO; i>0 ;i--);
                         if ( GPIO_PIN_IN(0x10) != 1) bandera=FALSE;    // ENTRA CUANDO PRESIONAMOS BOTON DE P1.4
                         while( GPIO_PIN_IN(0x10) != 1);                //MIENTRAS ESTE PRESIONADO EL BOTON
                        }

                    }
                PUERTO_P2->OUT  &= ~0x07;                         //APAGA TODOS

            }
            for(i=RETARDO2; i>0 ;i--);
        }
}
/*****************************************************************************
 * Function: GPIO_PIN_IN
 * Preconditions: PUERTO1 SELECCIONADO.
 * Overview: LEE EL PIN DESEADO
 * Input: pin_.
 * Output: 0/1
 *
 *****************************************************************************/
uint8_t GPIO_PIN_IN( uint_fast16_t pin_)
{
    uint_fast16_t inputPinValue;
    inputPinValue = PUERTO_P1->IN & (pin_);        // LEE EL REGISTRO QUE INDICA EL VALOR DE ENTRADA DE UN PIN
    if (inputPinValue > 0)                  // RETORNA EL VALOR 1 o 0
        return (0x01);
    return (0x00);
}
