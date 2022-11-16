/*
 * initPorts.h
 *Este archivo es para inicializar todos los puertos y no estén en tercer estado para reducir
 *el consumo de corriente según la ULP advisor.
 *  Created on: 4 oct. 2021
 *      Author: Alfredo Chacón
 */

#ifndef INCLUDES_INITPORTS_H_
#define INCLUDES_INITPORTS_H_

 (*((volatile uint8_t *)(0x040004c04))) &= ~0x10;                               //PIN P1.4 COMO ENTRADA                          (p1_dir)
 (*((volatile uint8_t *)(0x040004c06))) |= 0x10;                                //HABILITAMOS RESISTENCIA                        (p1_ren)
 (*((volatile uint8_t *)(0x040004c02))) |= 0x10;                                //PULL UP                                        (p1_out)

 (*((volatile uint8_t *)(0x040004c05))) |= 0x07;                                //PIN P2.0, P2.1, P2.2  COMO SALIDA, LEDS RGB    (p2_dir)
 (*((volatile uint8_t *)(0x040004c03))) &= ~0x07;                               // APAGADOS                                      (p2_out)



#endif /* INCLUDES_INITPORTS_H_ */
