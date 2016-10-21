/*******************************************************************************
* File Name: SPI1.h
* Version 2.50
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_SPI1_H)
#define CY_SPIM_SPI1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component SPI_Master_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define SPI1_INTERNAL_CLOCK             (1u)

#if(0u != SPI1_INTERNAL_CLOCK)
    #include "SPI1_IntClock.h"
#endif /* (0u != SPI1_INTERNAL_CLOCK) */

#define SPI1_MODE                       (1u)
#define SPI1_DATA_WIDTH                 (9u)
#define SPI1_MODE_USE_ZERO              (1u)
#define SPI1_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define SPI1_TX_BUFFER_SIZE             (4u)
#define SPI1_RX_BUFFER_SIZE             (4u)
#define SPI1_INTERNAL_TX_INT_ENABLED    (0u)
#define SPI1_INTERNAL_RX_INT_ENABLED    (0u)

#define SPI1_SINGLE_REG_SIZE            (8u)
#define SPI1_USE_SECOND_DATAPATH        (SPI1_DATA_WIDTH > SPI1_SINGLE_REG_SIZE)

#define SPI1_FIFO_SIZE                  (4u)
#define SPI1_TX_SOFTWARE_BUF_ENABLED    ((0u != SPI1_INTERNAL_TX_INT_ENABLED) && \
                                                     (SPI1_TX_BUFFER_SIZE > SPI1_FIFO_SIZE))

#define SPI1_RX_SOFTWARE_BUF_ENABLED    ((0u != SPI1_INTERNAL_RX_INT_ENABLED) && \
                                                     (SPI1_RX_BUFFER_SIZE > SPI1_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
} SPI1_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  SPI1_Init(void)                           ;
void  SPI1_Enable(void)                         ;
void  SPI1_Start(void)                          ;
void  SPI1_Stop(void)                           ;

void  SPI1_EnableTxInt(void)                    ;
void  SPI1_EnableRxInt(void)                    ;
void  SPI1_DisableTxInt(void)                   ;
void  SPI1_DisableRxInt(void)                   ;

void  SPI1_Sleep(void)                          ;
void  SPI1_Wakeup(void)                         ;
void  SPI1_SaveConfig(void)                     ;
void  SPI1_RestoreConfig(void)                  ;

void  SPI1_SetTxInterruptMode(uint8 intSrc)     ;
void  SPI1_SetRxInterruptMode(uint8 intSrc)     ;
uint8 SPI1_ReadTxStatus(void)                   ;
uint8 SPI1_ReadRxStatus(void)                   ;
void  SPI1_WriteTxData(uint16 txData)  \
                                                            ;
uint16 SPI1_ReadRxData(void) \
                                                            ;
uint8 SPI1_GetRxBufferSize(void)                ;
uint8 SPI1_GetTxBufferSize(void)                ;
void  SPI1_ClearRxBuffer(void)                  ;
void  SPI1_ClearTxBuffer(void)                  ;
void  SPI1_ClearFIFO(void)                              ;
void  SPI1_PutArray(const uint16 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != SPI1_BIDIRECTIONAL_MODE)
    void  SPI1_TxEnable(void)                   ;
    void  SPI1_TxDisable(void)                  ;
#endif /* (0u != SPI1_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(SPI1_TX_ISR);
CY_ISR_PROTO(SPI1_RX_ISR);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 SPI1_initVar;


/***************************************
*           API Constants
***************************************/

#define SPI1_TX_ISR_NUMBER     ((uint8) (SPI1_TxInternalInterrupt__INTC_NUMBER))
#define SPI1_RX_ISR_NUMBER     ((uint8) (SPI1_RxInternalInterrupt__INTC_NUMBER))

#define SPI1_TX_ISR_PRIORITY   ((uint8) (SPI1_TxInternalInterrupt__INTC_PRIOR_NUM))
#define SPI1_RX_ISR_PRIORITY   ((uint8) (SPI1_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define SPI1_INT_ON_SPI_DONE    ((uint8) (0u   << SPI1_STS_SPI_DONE_SHIFT))
#define SPI1_INT_ON_TX_EMPTY    ((uint8) (0u   << SPI1_STS_TX_FIFO_EMPTY_SHIFT))
#define SPI1_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           SPI1_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPI1_INT_ON_BYTE_COMP   ((uint8) (0u  << SPI1_STS_BYTE_COMPLETE_SHIFT))
#define SPI1_INT_ON_SPI_IDLE    ((uint8) (0u   << SPI1_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define SPI1_INT_ON_TX_NOT_FULL_DEF ((SPI1_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (SPI1_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define SPI1_TX_INIT_INTERRUPTS_MASK    (SPI1_INT_ON_SPI_DONE  | \
                                                     SPI1_INT_ON_TX_EMPTY  | \
                                                     SPI1_INT_ON_TX_NOT_FULL_DEF | \
                                                     SPI1_INT_ON_BYTE_COMP | \
                                                     SPI1_INT_ON_SPI_IDLE)

#define SPI1_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          SPI1_STS_RX_FIFO_FULL_SHIFT))
#define SPI1_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          SPI1_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPI1_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          SPI1_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define SPI1_RX_INIT_INTERRUPTS_MASK    (SPI1_INT_ON_RX_FULL      | \
                                                     SPI1_INT_ON_RX_NOT_EMPTY | \
                                                     SPI1_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define SPI1_BITCTR_INIT            (((uint8) (SPI1_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/
#if(CY_PSOC3 || CY_PSOC5)
    #define SPI1_TXDATA_REG (* (reg16 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F0_REG)
    #define SPI1_TXDATA_PTR (  (reg16 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F0_REG)
    #define SPI1_RXDATA_REG (* (reg16 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F1_REG)
    #define SPI1_RXDATA_PTR (  (reg16 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(SPI1_USE_SECOND_DATAPATH)
        #define SPI1_TXDATA_REG (* (reg16 *) \
                                          SPI1_BSPIM_sR16_Dp_u0__16BIT_F0_REG)
        #define SPI1_TXDATA_PTR (  (reg16 *) \
                                          SPI1_BSPIM_sR16_Dp_u0__16BIT_F0_REG)
        #define SPI1_RXDATA_REG (* (reg16 *) \
                                          SPI1_BSPIM_sR16_Dp_u0__16BIT_F1_REG)
        #define SPI1_RXDATA_PTR (  (reg16 *) \
                                          SPI1_BSPIM_sR16_Dp_u0__16BIT_F1_REG)
    #else
        #define SPI1_TXDATA_REG (* (reg8 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F0_REG)
        #define SPI1_TXDATA_PTR (  (reg8 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F0_REG)
        #define SPI1_RXDATA_REG (* (reg8 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F1_REG)
        #define SPI1_RXDATA_PTR (  (reg8 *) \
                                                SPI1_BSPIM_sR16_Dp_u0__F1_REG)
    #endif /* (SPI1_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define SPI1_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        SPI1_BSPIM_sR16_Dp_u0__DP_AUX_CTL_REG)
#define SPI1_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        SPI1_BSPIM_sR16_Dp_u0__DP_AUX_CTL_REG)

#if(SPI1_USE_SECOND_DATAPATH)
    #define SPI1_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        SPI1_BSPIM_sR16_Dp_u1__DP_AUX_CTL_REG)
    #define SPI1_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        SPI1_BSPIM_sR16_Dp_u1__DP_AUX_CTL_REG)
#endif /* (SPI1_USE_SECOND_DATAPATH) */

#define SPI1_COUNTER_PERIOD_REG     (* (reg8 *) SPI1_BSPIM_BitCounter__PERIOD_REG)
#define SPI1_COUNTER_PERIOD_PTR     (  (reg8 *) SPI1_BSPIM_BitCounter__PERIOD_REG)
#define SPI1_COUNTER_CONTROL_REG    (* (reg8 *) SPI1_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define SPI1_COUNTER_CONTROL_PTR    (  (reg8 *) SPI1_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define SPI1_TX_STATUS_REG          (* (reg8 *) SPI1_BSPIM_TxStsReg__STATUS_REG)
#define SPI1_TX_STATUS_PTR          (  (reg8 *) SPI1_BSPIM_TxStsReg__STATUS_REG)
#define SPI1_RX_STATUS_REG          (* (reg8 *) SPI1_BSPIM_RxStsReg__STATUS_REG)
#define SPI1_RX_STATUS_PTR          (  (reg8 *) SPI1_BSPIM_RxStsReg__STATUS_REG)

#define SPI1_CONTROL_REG            (* (reg8 *) \
                                      SPI1_BSPIM_BidirMode_CtrlReg__CONTROL_REG)
#define SPI1_CONTROL_PTR            (  (reg8 *) \
                                      SPI1_BSPIM_BidirMode_CtrlReg__CONTROL_REG)

#define SPI1_TX_STATUS_MASK_REG     (* (reg8 *) SPI1_BSPIM_TxStsReg__MASK_REG)
#define SPI1_TX_STATUS_MASK_PTR     (  (reg8 *) SPI1_BSPIM_TxStsReg__MASK_REG)
#define SPI1_RX_STATUS_MASK_REG     (* (reg8 *) SPI1_BSPIM_RxStsReg__MASK_REG)
#define SPI1_RX_STATUS_MASK_PTR     (  (reg8 *) SPI1_BSPIM_RxStsReg__MASK_REG)

#define SPI1_TX_STATUS_ACTL_REG     (* (reg8 *) SPI1_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPI1_TX_STATUS_ACTL_PTR     (  (reg8 *) SPI1_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPI1_RX_STATUS_ACTL_REG     (* (reg8 *) SPI1_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define SPI1_RX_STATUS_ACTL_PTR     (  (reg8 *) SPI1_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(SPI1_USE_SECOND_DATAPATH)
    #define SPI1_AUX_CONTROLDP1     (SPI1_AUX_CONTROL_DP1_REG)
#endif /* (SPI1_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define SPI1_STS_SPI_DONE_SHIFT             (0x00u)
#define SPI1_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define SPI1_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define SPI1_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define SPI1_STS_SPI_IDLE_SHIFT             (0x04u)
#define SPI1_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define SPI1_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define SPI1_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define SPI1_STS_SPI_DONE           ((uint8) (0x01u << SPI1_STS_SPI_DONE_SHIFT))
#define SPI1_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << SPI1_STS_TX_FIFO_EMPTY_SHIFT))
#define SPI1_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << SPI1_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPI1_STS_BYTE_COMPLETE      ((uint8) (0x01u << SPI1_STS_BYTE_COMPLETE_SHIFT))
#define SPI1_STS_SPI_IDLE           ((uint8) (0x01u << SPI1_STS_SPI_IDLE_SHIFT))
#define SPI1_STS_RX_FIFO_FULL       ((uint8) (0x01u << SPI1_STS_RX_FIFO_FULL_SHIFT))
#define SPI1_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << SPI1_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPI1_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << SPI1_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define SPI1_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define SPI1_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define SPI1_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define SPI1_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define SPI1_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define SPI1_FIFO_CLR       (SPI1_TX_FIFO_CLR | SPI1_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define SPI1_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define SPI1_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define SPI1_AUX_CTRL_FIFO0_CLR         (0x01u)
#define SPI1_AUX_CTRL_FIFO1_CLR         (0x02u)
#define SPI1_AUX_CTRL_FIFO0_LVL         (0x04u)
#define SPI1_AUX_CTRL_FIFO1_LVL         (0x08u)
#define SPI1_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define SPI1_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define SPI1_IS_ENABLED (0u != (SPI1_TX_STATUS_ACTL_REG & SPI1_INT_ENABLE))

/* Retuns TX status register */
#define SPI1_GET_STATUS_TX(swTxSts) ( (uint8)(SPI1_TX_STATUS_REG | \
                                                          ((swTxSts) & SPI1_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define SPI1_GET_STATUS_RX(swRxSts) ( (uint8)(SPI1_RX_STATUS_REG | \
                                                          ((swRxSts) & SPI1_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define SPI1_WriteByte   SPI1_WriteTxData
#define SPI1_ReadByte    SPI1_ReadRxData
void  SPI1_SetInterruptMode(uint8 intSrc)       ;
uint8 SPI1_ReadStatus(void)                     ;
void  SPI1_EnableInt(void)                      ;
void  SPI1_DisableInt(void)                     ;

#define SPI1_TXDATA                 (SPI1_TXDATA_REG)
#define SPI1_RXDATA                 (SPI1_RXDATA_REG)
#define SPI1_AUX_CONTROLDP0         (SPI1_AUX_CONTROL_DP0_REG)
#define SPI1_TXBUFFERREAD           (SPI1_txBufferRead)
#define SPI1_TXBUFFERWRITE          (SPI1_txBufferWrite)
#define SPI1_RXBUFFERREAD           (SPI1_rxBufferRead)
#define SPI1_RXBUFFERWRITE          (SPI1_rxBufferWrite)

#define SPI1_COUNTER_PERIOD         (SPI1_COUNTER_PERIOD_REG)
#define SPI1_COUNTER_CONTROL        (SPI1_COUNTER_CONTROL_REG)
#define SPI1_STATUS                 (SPI1_TX_STATUS_REG)
#define SPI1_CONTROL                (SPI1_CONTROL_REG)
#define SPI1_STATUS_MASK            (SPI1_TX_STATUS_MASK_REG)
#define SPI1_STATUS_ACTL            (SPI1_TX_STATUS_ACTL_REG)

#define SPI1_INIT_INTERRUPTS_MASK  (SPI1_INT_ON_SPI_DONE     | \
                                                SPI1_INT_ON_TX_EMPTY     | \
                                                SPI1_INT_ON_TX_NOT_FULL_DEF  | \
                                                SPI1_INT_ON_RX_FULL      | \
                                                SPI1_INT_ON_RX_NOT_EMPTY | \
                                                SPI1_INT_ON_RX_OVER      | \
                                                SPI1_INT_ON_BYTE_COMP)
                                                
#define SPI1_DataWidth                  (SPI1_DATA_WIDTH)
#define SPI1_InternalClockUsed          (SPI1_INTERNAL_CLOCK)
#define SPI1_InternalTxInterruptEnabled (SPI1_INTERNAL_TX_INT_ENABLED)
#define SPI1_InternalRxInterruptEnabled (SPI1_INTERNAL_RX_INT_ENABLED)
#define SPI1_ModeUseZero                (SPI1_MODE_USE_ZERO)
#define SPI1_BidirectionalMode          (SPI1_BIDIRECTIONAL_MODE)
#define SPI1_Mode                       (SPI1_MODE)
#define SPI1_DATAWIDHT                  (SPI1_DATA_WIDTH)
#define SPI1_InternalInterruptEnabled   (0u)

#define SPI1_TXBUFFERSIZE   (SPI1_TX_BUFFER_SIZE)
#define SPI1_RXBUFFERSIZE   (SPI1_RX_BUFFER_SIZE)

#define SPI1_TXBUFFER       SPI1_txBuffer
#define SPI1_RXBUFFER       SPI1_rxBuffer

#endif /* (CY_SPIM_SPI1_H) */


/* [] END OF FILE */
