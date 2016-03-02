/*
 * ports.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Sean
 */

#ifndef SOURCES_PORTS_H_
#define SOURCES_PORTS_H_

#define PORT_MUX_ALT0 0x0u
#define PORT_MUX_ALT1 0x1u
#define PORT_MUX_ALT2 0x2u
#define PORT_MUX_ALT3 0x3u
#define PORT_MUX_ALT4 0x4u
#define PORT_MUX_GPIO PORT_MUX_ALT1


void portB_setup();

void portD_setup();

#endif /* SOURCES_PORTS_H_ */
