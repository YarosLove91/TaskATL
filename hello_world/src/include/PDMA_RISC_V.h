//
// Created by MYaro on 20.02.2023.
//

#ifndef TIMER_PDMA_RISCV_V_H
#define TIMER_PDMA_RISCV_V_H
/******************************************************************************/
/*                                                                            */
/*                         Platform DMA Engine (PDMA)                         */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for PDMA register  ********************/


#include <stddef.h>
#include <stdint.h>

#include <macro_def.h>
#include <config.h>
#include <stdio.h>
#include <panic.h>


#define PDMA_BASE            ((char *)(uintptr_t)0x03000000UL)      // Основание адресного пространства модулей PDMA
#define PDMA_OFFSET          0x1000UL                               // Смещение между каналами

#define PDMA_CHANNEL_ID0 0                                          // Каналы ПДП
#define PDMA_CHANNEL_ID1 1
#define PDMA_CHANNEL_ID2 2
#define PDMA_CHANNEL_ID3 3

#define PDMA_MAX_TRANSACTION	64                                  // ПДП копирует блоками по 8 байт
#define	U(a)		        ((pdmareg32_t) a)

// 
typedef uint32_t pdmareg32_t;
typedef uint64_t pdmareg64_t;

// Сделано в стиле SPL STM32
// Смещение регистров ПДП задается 
typedef struct {
    __IOM pdmareg32_t   PDMA_CONTROL;
    __IOM pdmareg32_t   PDMA_NEXT_CONFIG;
    __IOM pdmareg64_t   PDMA_NEXT_BYTES;
    __IOM pdmareg64_t   PDMA_NEXT_DST;
    __IOM pdmareg64_t   PDMA_NEXT_SOURCE;
    __IM pdmareg32_t    PDMA_EXEC_CFG;
    __IM pdmareg64_t    PDMA_EXEC_BYTES;
    __IM pdmareg64_t    PDMA_EXEC_DEST;
    __IM pdmareg64_t    PDMA_EXEC_SOURCE;
}PDMA_TypeDef;

// Сделано по образу и подобию SPL
#define PDMA_CH0                 ((PDMA_TypeDef *)DMA0_UNIT_BASE)
#define PDMA_CH1                 ((PDMA_TypeDef *)DMA1_UNIT_BASE)
#define PDMA_CH2                 ((PDMA_TypeDef *)DMA2_UNIT_BASE)
#define PDMA_CH3                 ((PDMA_TypeDef *)DMA3_UNIT_BASE)


#define DMA0_UNIT_BASE    (PDMA_BASE + (PDMA_OFFSET * PDMA_CHANNEL_ID0))           // 3000000
#define DMA1_UNIT_BASE    (PDMA_BASE + (PDMA_OFFSET * PDMA_CHANNEL_ID1))           // 3001000
#define DMA2_UNIT_BASE    (PDMA_BASE + (PDMA_OFFSET * PDMA_CHANNEL_ID2))           // 3002000
#define DMA3_UNIT_BASE    (PDMA_BASE + (PDMA_OFFSET * PDMA_CHANNEL_ID3))           // 3003000


//
/******************  Bit definition for channel control register  ******************/
#define PDMA_CONTROL_CLAIM_Pos   (0U)
#define PDMA_CONTROL_CLAIM_Msk   (0x1UL << PDMA_CONTROL_CLAIM_Pos)
#define PDMA_CONTROL_CLAIM_Set   PDMA_CONTROL_CLAIM_Msk
#define PDMA_CONTROL_CLAIM_Clr   (0x0UL << PDMA_CONTROL_CLAIM_Pos)

#define PDMA_CONTROL_RUN_Pos     (1U)
#define PDMA_CONTROL_RUN_Msk     (0x01UL<< PDMA_CONTROL_RUN_Pos)
#define PDMA_CONTROL_RUN_Set     PDMA_CONTROL_RUN_Msk
#define PDMA_CONTROL_RUN_Clr     (0x0UL << PDMA_CONTROL_CLAIM_Pos)

#define PDMA_CONTROL_doneIE_Pos  (26U)
#define PDMA_CONTROL_doneIE_Msk  (0x01UL<< PDMA_CONTROL_doneIE_Pos)
#define PDMA_CONTROL_doneIE      PDMA_CONTROL_doneIE_Msk

#define PDMA_CONTROL_errorIE_Pos (27U)
#define PDMA_CONTROL_errorIE_Msk (0x01UL<< PDMA_CONTROL_errorIE_Pos)
#define PDMA_CONTROL_errorIE     PDMA_CONTROL_errorIE_Msk

#define PDMA_CONTROL_fDONE_Pos   (30U)
#define PDMA_CONTROL_fDONE_Msk   (0x01UL<< PDMA_CONTROL_fDONE_Pos)
#define PDMA_CONTROL_fDONE       PDMA_CONTROL_fDONE_Msk


#define PDMA_CONTROL_fERROR_Pos  (31U)
#define PDMA_CONTROL_fERROR_Msk  (0x01UL<< PDMA_CONTROL_fERROR_Pos)
#define PDMA_CONTROL_fERROR      PDMA_CONTROL_fERROR_Msk

//
/******************  Bit definition for channel next configuration register  ******************/
#define PDMA_NCR_repeat_Pos   (2U)
#define PDMA_NCR_repeat_Msk   (0x01UL<< PDMA_NCR_repeat_Pos)
#define PDMA_NCR_repeat       PDMA_NCR_repeat_Msk

#define PDMA_NCR_order_Pos   (3U)
#define PDMA_NCR_order_Msk   (0x01UL<< PDMA_NCR_order_Pos)
#define PDMA_NCR_order       PDMA_NCR_order_Msk

#define PDMA_NCR_wsize_Pos   (24U)
#define PDMA_NCR_wsize_Msk   (0x01UL<< PDMA_NCR_wsize_Pos)
#define PDMA_NCR_wsize       PDMA_NCR_wsize_Msk

#define PDMA_NCR_rsize_Pos   (28U)
#define PDMA_NCR_rsize_Msk   (0x01UL<< PDMA_NCR_rsize_Pos)
#define PDMA_NCR_rsize       PDMA_NCR_rsize_Msk


#define PDMA_NEXT_CONFIG_WSIZE(s)	((s) << PDMA_NCR_wsize_Pos)
#define PDMA_NEXT_CONFIG_RSIZE(s)	((s) << PDMA_NCR_rsize_Pos)

// Количество байтиков в пакете на запись и чтение модуля ПДП
enum {
PDMA_NCR_WSIZE_1BYTE = 0x00,
PDMA_NCR_WSIZE_2BYTE, 
PDMA_NCR_WSIZE_4BYTE, 
PDMA_NCR_WSIZE_8BYTE,
PDMA_NCR_WSIZE_16BYTE,
PDMA_NCR_WSIZE_32BYTE,
PDMA_NCR_WSIZE_64BYTE
};

enum {
PDMA_NCR_RSIZE_1BYTE = 0x00,
PDMA_NCR_RSIZE_2BYTE, 
PDMA_NCR_RSIZE_4BYTE, 
PDMA_NCR_RSIZE_8BYTE,
PDMA_NCR_RSIZE_16BYTE,
PDMA_NCR_RSIZE_32BYTE,
PDMA_NCR_RSIZE_64BYTE
};


// Прототипы функции
void dma_memcpy(void *dst, const volatile void *src, size_t size, int wait_ready);
void dma_wait_transaction(void);

#define DMA_NCR_RSIZE
#endif //TIMER_PDMA_RISCV_V_H
