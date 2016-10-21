/*******************************************************************************
* File Name: SPI_SD.h
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

#if !defined(CY_SPIM_SPI_SD_H)
#define CY_SPIM_SPI_SD_H

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

#define SPI_SD_INTERNAL_CLOCK             (1u)

#if(0u != SPI_SD_INTERNAL_CLOCK)
    #include "SPI_SD_IntClock.h"
#endif /* (0u != SPI_SD_INTERNAL_CLOCK) */

#define SPI_SD_MODE                       (1u)
#define SPI_SD_DATA_WIDTH                 (8u)
#define SPI_SD_MODE_USE_ZERO              (1u)
#define SPI_SD_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define SPI_SD_TX_BUFFER_SIZE             (4u)
#define SPI_SD_RX_BUFFER_SIZE             (4u)
#define SPI_SD_INTERNAL_TX_INT_ENABLED    (0u)
#define SPI_SD_INTERNAL_RX_INT_ENABLED    (0u)

#define SPI_SD_SINGLE_REG_SIZE            (8u)
#define SPI_SD_USE_SECOND_DATAPATH        (SPI_SD_DATA_WIDTH > SPI_SD_SINGLE_REG_SIZE)

#define SPI_SD_FIFO_SIZE                  (4u)
#define SPI_SD_TX_SOFTWARE_BUF_ENABLED    ((0u != SPI_SD_INTERNAL_TX_INT_ENABLED) && \
                                                     (SPI_SD_TX_BUFFER_SIZE > SPI_SD_FIFO_SIZE))

#define SPI_SD_RX_SOFTWARE_BUF_ENABLED    ((0u != SPI_SD_INTERNAL_RX_INT_ENABLED) && \
                                                     (SPI_SD_RX_BUFFER_SIZE > SPI_SD_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
} SPI_SD_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  SPI_SD_Init(void)                           ;
void  SPI_SD_Enable(void)                         ;
void  SPI_SD_Start(void)                          ;
void  SPI_SD_Stop(void)                           ;

void  SPI_SD_EnableTxInt(void)                    ;
void  SPI_SD_EnableRxInt(void)                    ;
void  SPI_SD_DisableTxInt(void)                   ;
void  SPI_SD_DisableRxInt(void)                   ;

void  SPI_SD_Sleep(void)                          ;
void  SPI_SD_Wakeup(void)                         ;
void  SPI_SD_SaveConfig(void)                     ;
void  SPI_SD_RestoreConfig(void)                  ;

void  SPI_SD_SetTxInterruptMode(uint8 intSrc)     ;
void  SPI_SD_SetRxInterruptMode(uint8 intSrc)     ;
uint8 SPI_SD_ReadTxStatus(void)                   ;
uint8 SPI_SD_ReadRxStatus(void)                   ;
void  SPI_SD_WriteTxData(uint8 txData)  \
                                                            ;
uint8 SPI_SD_ReadRxData(void) \
                                                            ;
uint8 SPI_SD_GetRxBufferSize(void)                ;
uint8 SPI_SD_GetTxBufferSize(void)                ;
void  SPI_SD_ClearRxBuffer(void)                  ;
void  SPI_SD_ClearTxBuffer(void)                  ;
void  SPI_SD_ClearFIFO(void)                              ;
void  SPI_SD_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != SPI_SD_BIDIRECTIONAL_MODE)
    void  SPI_SD_TxEnable(void)                   ;
    void  SPI_SD_TxDisable(void)                  ;
#endif /* (0u != SPI_SD_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(SPI_SD_TX_ISR);
CY_ISR_PROTO(SPI_SD_RX_ISR);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 SPI_SD_initVar;


/***************************************
*           API Constants
***************************************/

#define SPI_SD_TX_ISR_NUMBER     ((uint8) (SPI_SD_TxInternalInterrupt__INTC_NUMBER))
#define SPI_SD_RX_ISR_NUMBER     ((uint8) (SPI_SD_RxInternalInterrupt__INTC_NUMBER))

#define SPI_SD_TX_ISR_PRIORITY   ((uint8) (SPI_SD_TxInternalInterrupt__INTC_PRIOR_NUM))
#define SPI_SD_RX_ISR_PRIORITY   ((uint8) (SPI_SD_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define SPI_SD_INT_ON_SPI_DONE    ((uint8) (0u   << SPI_SD_STS_SPI_DONE_SHIFT))
#define SPI_SD_INT_ON_TX_EMPTY    ((uint8) (0u   << SPI_SD_STS_TX_FIFO_EMPTY_SHIFT))
#define SPI_SD_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           SPI_SD_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPI_SD_INT_ON_BYTE_COMP   ((uint8) (0u  << SPI_SD_STS_BYTE_COMPLETE_SHIFT))
#define SPI_SD_INT_ON_SPI_IDLE    ((uint8) (0u   << SPI_SD_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define SPI_SD_INT_ON_TX_NOT_FULL_DEF ((SPI_SD_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (SPI_SD_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define SPI_SD_TX_INIT_INTERRUPTS_MASK    (SPI_SD_INT_ON_SPI_DONE  | \
                                                     SPI_SD_INT_ON_TX_EMPTY  | \
                                                     SPI_SD_INT_ON_TX_NOT_FULL_DEF | \
                                                     SPI_SD_INT_ON_BYTE_COMP | \
                                                     SPI_SD_INT_ON_SPI_IDLE)

#define SPI_SD_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          SPI_SD_STS_RX_FIFO_FULL_SHIFT))
#define SPI_SD_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          SPI_SD_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPI_SD_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          SPI_SD_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define SPI_SD_RX_INIT_INTERRUPTS_MASK    (SPI_SD_INT_ON_RX_FULL      | \
                                                     SPI_SD_INT_ON_RX_NOT_EMPTY | \
                                                     SPI_SD_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define SPI_SD_BITCTR_INIT            (((uint8) (SPI_SD_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/
#if(CY_PSOC3 || CY_PSOC5)
    #define SPI_SD_TXDATA_REG (* (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F0_REG)
    #define SPI_SD_TXDATA_PTR (  (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F0_REG)
    #define SPI_SD_RXDATA_REG (* (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F1_REG)
    #define SPI_SD_RXDATA_PTR (  (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(SPI_SD_USE_SECOND_DATAPATH)
        #define SPI_SD_TXDATA_REG (* (reg16 *) \
                                          SPI_SD_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define SPI_SD_TXDATA_PTR (  (reg16 *) \
                                          SPI_SD_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define SPI_SD_RXDATA_REG (* (reg16 *) \
                                          SPI_SD_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define SPI_SD_RXDATA_PTR (  (reg16 *) \
                                          SPI_SD_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define SPI_SD_TXDATA_REG (* (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F0_REG)
        #define SPI_SD_TXDATA_PTR (  (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F0_REG)
        #define SPI_SD_RXDATA_REG (* (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F1_REG)
        #define SPI_SD_RXDATA_PTR (  (reg8 *) \
                                                SPI_SD_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (SPI_SD_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define SPI_SD_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        SPI_SD_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define SPI_SD_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        SPI_SD_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(SPI_SD_USE_SECOND_DATAPATH)
    #define SPI_SD_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        SPI_SD_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define SPI_SD_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        SPI_SD_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (SPI_SD_USE_SECOND_DATAPATH) */

#define SPI_SD_COUNTER_PERIOD_REG     (* (reg8 *) SPI_SD_BSPIM_BitCounter__PERIOD_REG)
#define SPI_SD_COUNTER_PERIOD_PTR     (  (reg8 *) SPI_SD_BSPIM_BitCounter__PERIOD_REG)
#define SPI_SD_COUNTER_CONTROL_REG    (* (reg8 *) SPI_SD_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define SPI_SD_COUNTER_CONTROL_PTR    (  (reg8 *) SPI_SD_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define SPI_SD_TX_STATUS_REG          (* (reg8 *) SPI_SD_BSPIM_TxStsReg__STATUS_REG)
#define SPI_SD_TX_STATUS_PTR          (  (reg8 *) SPI_SD_BSPIM_TxStsReg__STATUS_REG)
#define SPI_SD_RX_STATUS_REG          (* (reg8 *) SPI_SD_BSPIM_RxStsReg__STATUS_REG)
#define SPI_SD_RX_STATUS_PTR          (  (reg8 *) SPI_SD_BSPIM_RxStsReg__STATUS_REG)

#define SPI_SD_CONTROL_REG            (* (reg8 *) \
                                      SPI_SD_BSPIM_BidirMode_CtrlReg__CONTROL_REG)
#define SPI_SD_CONTROL_PTR            (  (reg8 *) \
                                      SPI_SD_BSPIM_BidirMode_CtrlReg__CONTROL_REG)

#define SPI_SD_TX_STATUS_MASK_REG     (* (reg8 *) SPI_SD_BSPIM_TxStsReg__MASK_REG)
#define SPI_SD_TX_STATUS_MASK_PTR     (  (reg8 *) SPI_SD_BSPIM_TxStsReg__MASK_REG)
#define SPI_SD_RX_STATUS_MASK_REG     (* (reg8 *) SPI_SD_BSPIM_RxStsReg__MASK_REG)
#define SPI_SD_RX_STATUS_MASK_PTR     (  (reg8 *) SPI_SD_BSPIM_RxStsReg__MASK_REG)

#define SPI_SD_TX_STATUS_ACTL_REG     (* (reg8 *) SPI_SD_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPI_SD_TX_STATUS_ACTL_PTR     (  (reg8 *) SPI_SD_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPI_SD_RX_STATUS_ACTL_REG     (* (reg8 *) SPI_SD_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define SPI_SD_RX_STATUS_ACTL_PTR     (  (reg8 *) SPI_SD_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(SPI_SD_USE_SECOND_DATAPATH)
    #define SPI_SD_AUX_CONTROLDP1     (SPI_SD_AUX_CONTROL_DP1_REG)
#endif /* (SPI_SD_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define SPI_SD_STS_SPI_DONE_SHIFT             (0x00u)
#define SPI_SD_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define SPI_SD_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define SPI_SD_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define SPI_SD_STS_SPI_IDLE_SHIFT             (0x04u)
#define SPI_SD_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define SPI_SD_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define SPI_SD_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define SPI_SD_STS_SPI_DONE           ((uint8) (0x01u << SPI_SD_STS_SPI_DONE_SHIFT))
#define SPI_SD_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << SPI_SD_STS_TX_FIFO_EMPTY_SHIFT))
#define SPI_SD_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << SPI_SD_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPI_SD_STS_BYTE_COMPLETE      ((uint8) (0x01u << SPI_SD_STS_BYTE_COMPLETE_SHIFT))
#define SPI_SD_STS_SPI_IDLE           ((uint8) (0x01u << SPI_SD_STS_SPI_IDLE_SHIFT))
#define SPI_SD_STS_RX_FIFO_FULL       ((uint8) (0x01u << SPI_SD_STS_RX_FIFO_FULL_SHIFT))
#define SPI_SD_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << SPI_SD_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPI_SD_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << SPI_SD_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define SPI_SD_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define SPI_SD_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define SPI_SD_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define SPI_SD_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define SPI_SD_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define SPI_SD_FIFO_CLR       (SPI_SD_TX_FIFO_CLR | SPI_SD_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define SPI_SD_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define SPI_SD_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define SPI_SD_AUX_CTRL_FIFO0_CLR         (0x01u)
#define SPI_SD_AUX_CTRL_FIFO1_CLR         (0x02u)
#define SPI_SD_AUX_CTRL_FIFO0_LVL         (0x04u)
#define SPI_SD_AUX_CTRL_FIFO1_LVL         (0x08u)
#define SPI_SD_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define SPI_SD_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define SPI_SD_IS_ENABLED (0u != (SPI_SD_TX_STATUS_ACTL_REG & SPI_SD_INT_ENABLE))

/* Retuns TX status register */
#define SPI_SD_GET_STATUS_TX(swTxSts) ( (uint8)(SPI_SD_TX_STATUS_REG | \
                                                          ((swTxSts) & SPI_SD_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define SPI_SD_GET_STATUS_RX(swRxSts) ( (uint8)(SPI_SD_RX_STATUS_REG | \
                                                          ((swRxSts) & SPI_SD_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define SPI_SD_WriteByte   SPI_SD_WriteTxData
#define SPI_SD_ReadByte    SPI_SD_ReadRxData
void  SPI_SD_SetInterruptMode(uint8 intSrc)       ;
uint8 SPI_SD_ReadStatus(void)                     ;
void  SPI_SD_EnableInt(void)                      ;
void  SPI_SD_DisableInt(void)                     ;

#define SPI_SD_TXDATA                 (SPI_SD_TXDATA_REG)
#define SPI_SD_RXDATA                 (SPI_SD_RXDATA_REG)
#define SPI_SD_AUX_CONTROLDP0         (SPI_SD_AUX_CONTROL_DP0_REG)
#define SPI_SD_TXBUFFERREAD           (SPI_SD_txBufferRead)
#define SPI_SD_TXBUFFERWRITE          (SPI_SD_txBufferWrite)
#define SPI_SD_RXBUFFERREAD           (SPI_SD_rxBufferRead)
#define SPI_SD_RXBUFFERWRITE          (SPI_SD_rxBufferWrite)

#define SPI_SD_COUNTER_PERIOD         (SPI_SD_COUNTER_PERIOD_REG)
#define SPI_SD_COUNTER_CONTROL        (SPI_SD_COUNTER_CONTROL_REG)
#define SPI_SD_STATUS                 (SPI_SD_TX_STATUS_REG)
#define SPI_SD_CONTROL                (SPI_SD_CONTROL_REG)
#define SPI_SD_STATUS_MASK            (SPI_SD_TX_STATUS_MASK_REG)
#define SPI_SD_STATUS_ACTL            (SPI_SD_TX_STATUS_ACTL_REG)

#define SPI_SD_INIT_INTERRUPTS_MASK  (SPI_SD_INT_ON_SPI_DONE     | \
                                                SPI_SD_INT_ON_TX_EMPTY     | \
                                                SPI_SD_INT_ON_TX_NOT_FULL_DEF  | \
                                                SPI_SD_INT_ON_RX_FULL      | \
                                                SPI_SD_INT_ON_RX_NOT_EMPTY | \
                                                SPI_SD_INT_ON_RX_OVER      | \
                                                SPI_SD_INT_ON_BYTE_COMP)
                                                
#define SPI_SD_DataWidth                  (SPI_SD_DATA_WIDTH)
#define SPI_SD_InternalClockUsed          (SPI_SD_INTERNAL_CLOCK)
#define SPI_SD_InternalTxInterruptEnabled (SPI_SD_INTERNAL_TX_INT_ENABLED)
#define SPI_SD_InternalRxInterruptEnabled (SPI_SD_INTERNAL_RX_INT_ENABLED)
#define SPI_SD_ModeUseZero                (SPI_SD_MODE_USE_ZERO)
#define SPI_SD_BidirectionalMode          (SPI_SD_BIDIRECTIONAL_MODE)
#define SPI_SD_Mode                       (SPI_SD_MODE)
#define SPI_SD_DATAWIDHT                  (SPI_SD_DATA_WIDTH)
#define SPI_SD_InternalInterruptEnabled   (0u)

#define SPI_SD_TXBUFFERSIZE   (SPI_SD_TX_BUFFER_SIZE)
#define SPI_SD_RXBUFFERSIZE   (SPI_SD_RX_BUFFER_SIZE)

#define SPI_SD_TXBUFFER       SPI_SD_txBuffer
#define SPI_SD_RXBUFFER       SPI_SD_rxBuffer

#endif /* (CY_SPIM_SPI_SD_H) */


/* [] END OF FILE */
