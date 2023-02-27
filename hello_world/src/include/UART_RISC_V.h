//
// Created by MYaro on 20.02.2023.
//

#ifndef UART_RISC_V_H
#define UART_RISC_V_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "macro_def.h"
#include "config.h"

#define UART0_BASE      0x10010000UL
#define UART1_BASE      0x10011000UL

#define UART0    ((UART_TypeDef*)UART0_BASE)
#define UART1    ((UART_TypeDef*)UART1_BASE)


#define cpu_freq 32000000UL                     // Вообще скорость проца была определена в какром-т системном файле QEMU
#define uart_freq cpu_freq
#define baud_rate 9600
#define divisor  ((uart_freq / baud_rate) - 1)

typedef struct {
    __IOM       uint32_t    txDATA;
    __IOM       uint32_t    rxDATA;
    __IOM       uint32_t    txCTRL;
    __IOM       uint32_t    rxCTRL;
    __IOM       uint32_t    iEnable;
    __IM        uint32_t    iPending;
    __IOM       uint32_t    baudRateREG;
}UART_TypeDef;

// Transmit Data Reg bit position (txDATA)
#define UART_DATA_W_Pos     (0U)
#define UART_DATA_W_Msk     (0x01UL << UART_DATA_W_Pos)
#define UART_DATA_W         UART_DATA_W_Msk

#define UART_FULL_W_Pos     (31U)
#define UART_FULL_W_Msk     (0x01UL<< UART_FULL_W_Pos)
#define UART_FULL_W         UART_FULL_W_Msk

// Receive Data Reg bit position (rxDATA)
#define UART_DATA_REN_Pos     (0U)
#define UART_DATA_REN_Msk     (0x01UL << UART_DATA_REN_Pos)
#define UART_DATA_R_EN        UART_DATA_REN_Msk
#define UART_DATA_R_DIS       (0x00UL << UART_DATA_REN_Pos)

#define UART_FULL_R_Pos     (31U)
#define UART_FULL_R_Msk     (0x01UL<< UART_FULL_R_Pos)
#define UART_FULL_R         UART_FULL_R_Msk

// Transmit control register    (txCTRL)
#define UART_TXEN_Pos      (0U)
#define UART_TXEN_Msk      (0x01UL << UART_TXEN_Pos)
#define UART_TXEN_EN       UART_TXEN_Msk
#define UART_TXEN_DIS      (0x00 << UART_TXEN_Pos)


#define UART_NSTOP_Pos     (01U)
#define UART_NSTOP_Msk     (0x01UL<< UART_NSTOP_Pos)
#define UART_NSTOP         UART_NSTOP_Msk

#define UART_TXCNT_Pos     (16U)
#define UART_TXCNT_Msk     (0x01UL  << UART_TXCNT_Pos)
#define UART_TXCNT         UART_TXCNT_Msk

// UART receive control reg     (rxCTRL)
#define UART_rxEN_Pos      (0U)
#define UART_rxEN_Msk      (0x01UL  << UART_rxEN_Pos)
#define UART_rxEN_EN       UART_rxEN_Msk
#define UART_rxEN_DIS      (0x00UL  << UART_rxEN_Pos)

#define UART_rxCNT_Pos     (16U)
#define UART_rxCNT_Msk     (0x01UL  << UART_rxCNT_Pos)
#define UART_rxCNT         UART_rxCNT_Msk

// UART interrupt enable control reg
#define UART_txWMi_Pos     (0U)
#define UART_txWMi_Msk     (0x01UL << UART_txWMi_Pos)
#define UART_txWMi         UART_txWMi_Msk

#define UART_rxWMi_Pos     (1U)
#define UART_rxWMi_Msk     (0x01UL<< UART_rxWMi_Pos)
#define UART_rxWMi         UART_rxWMi_Msk

// UART interrupt pending Reg
#define UART_txWMp_Pos     (0U)
#define UART_txWMp_Msk     (0x01UL << UART_txWMp_Pos)
#define UART_txWMp         UART_txWMp_Msk

#define UART_rxWMp_Pos     (1U)
#define UART_rxWMp_Msk     (0x01UL<< UART_rxWMp_Pos)
#define UART_FULLp         UART_rxWMp_Msk

int printf(const char* s, ...);
void uart_init(void);


#endif //UART_RISC_V_H
