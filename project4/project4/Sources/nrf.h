#ifndef SOURCES_NRF_H_
#define SOURCES_NRF_H_
#include <stdint.h>
#include <stddef.h>
#include "spi.h"

#define REG_CONFIG 			0x00
#define REG_CONFIG_LEN 		1

#define REG_EN_AA			0x01
#define REG_EN_AA_LEN		1

#define REG_EN_RXADDR		0x02
#define REG_EN_RXADDR_LEN	1

#define REG_SETUP_AW		0x03
#define REG_SETUP_AW_LEN	1

#define REG_SETUP_RETR		0x04
#define REG_SETUP_RETR_LEN 	1

#define REG_RF_CH			0x05
#define REG_RF_CH_LEN		1

#define REG_RF_SETUP		0x06
#define REG_RF_SETUP_LEN	1

#define REG_STATUS			0x07
#define REG_STATUS_LEN		1

#define REG_OBSERVE_TX		0x08
#define REG_OBSERVE_TX_LEN 	1

#define REG_CD				0x09
#define REG_CD_LEN			1

#define REG_RX_ADDR_P0		0x0A
#define REG_RX_ADDR_P0_LEN 	5

#define REG_RX_ADDR_P1		0x0B
#define REG_RX_ADDR_P1_LEN 	5

#define REG_RX_ADDR_P2		0x0C
#define REG_RX_ADDR_P2_LEN 	1

#define REG_RX_ADDR_P3		0x0D
#define REG_RX_ADDR_P3_LEN 	1

#define REG_RX_ADDR_P4		0x0E
#define REG_RX_ADDR_P4_LEN 	1

#define REG_RX_ADDR_P5		0x0F
#define REG_RX_ADDR_P5_LEN 	1

#define REG_TX_ADDR			0x10
#define REG_TX_ADDR_LEN		5

#define REG_RX_PW_P0		0x11
#define REG_RX_PW_P0_LEN	1

#define REG_RX_PW_P1		0x12
#define REG_RX_PW_P1_LEN	1

#define REG_RX_PW_P2		0x13
#define REG_RX_PW_P2_LEN	1

#define REG_RX_PW_P3		0x14
#define REG_RX_PW_P3_LEN	1

#define REG_RX_PW_P4		0x15
#define REG_RX_PW_P4_LEN	1

#define REG_RX_PW_P5		0x16
#define REG_RX_PW_P5_LEN	1

#define REG_FIFO_STATUS		0x17
#define REG_FIFO_STATUS_LEN 1

#define REG_DYNPD			0x1C
#define REG_DYNPD_LEN		1

#define REG_FEATURE			0x1D
#define REG_FEATURE_		1

#define MAX_REG				REG_FEATURE

// Commands - from datasheet
#define R_REGISTER			0x00
#define W_REGISTER			0x20
#define	R_RX_PAYLOAD		0x61
#define W_TX_PAYLOAD		0xA0
#define FLUSH_TX			0xE1
#define FLUSH_RX			0xE2
#define REUSE_TX_PL			0xE3
#define ACTIVATE			0x50
#define R_RX_PL_WID			0x60
#define W_ACK_PAYLOAD		0xA8 //This needs to be OR'd with 0bPPP, where PPP is in range 000 - 101
#define W_TX_PAYLOAD_NOACK	0xB0
#define NOP					0xFF


// Function prototypes
void 	NRF_Write_Register(uint8_t reg, size_t len, uint8_t *buffer);
uint8_t *NRF_Read_Register(uint8_t reg, size_t len, uint8_t* buffer);
#endif /* SOURCES_NRF_H_ */
