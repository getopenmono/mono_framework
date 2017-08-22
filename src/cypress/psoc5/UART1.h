/*******************************************************************************
* File Name: UART1.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_UART1_H)
#define CY_UART_UART1_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define UART1_RX_ENABLED                     (1u)
#define UART1_TX_ENABLED                     (1u)
#define UART1_HD_ENABLED                     (0u)
#define UART1_RX_INTERRUPT_ENABLED           (0u)
#define UART1_TX_INTERRUPT_ENABLED           (0u)
#define UART1_INTERNAL_CLOCK_USED            (1u)
#define UART1_RXHW_ADDRESS_ENABLED           (0u)
#define UART1_OVER_SAMPLE_COUNT              (8u)
#define UART1_PARITY_TYPE                    (0u)
#define UART1_PARITY_TYPE_SW                 (0u)
#define UART1_BREAK_DETECT                   (0u)
#define UART1_BREAK_BITS_TX                  (13u)
#define UART1_BREAK_BITS_RX                  (13u)
#define UART1_TXCLKGEN_DP                    (1u)
#define UART1_USE23POLLING                   (1u)
#define UART1_FLOW_CONTROL                   (0u)
#define UART1_CLK_FREQ                       (0u)
#define UART1_TX_BUFFER_SIZE                 (4u)
#define UART1_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define UART1_CONTROL_REG_REMOVED            (0u)
#else
    #define UART1_CONTROL_REG_REMOVED            (1u)
#endif /* End UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct UART1_backupStruct_
{
    uint8 enableState;

    #if(UART1_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End UART1_CONTROL_REG_REMOVED */

} UART1_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void UART1_Start(void) ;
void UART1_Stop(void) ;
uint8 UART1_ReadControlRegister(void) ;
void UART1_WriteControlRegister(uint8 control) ;

void UART1_Init(void) ;
void UART1_Enable(void) ;
void UART1_SaveConfig(void) ;
void UART1_RestoreConfig(void) ;
void UART1_Sleep(void) ;
void UART1_Wakeup(void) ;

/* Only if RX is enabled */
#if( (UART1_RX_ENABLED) || (UART1_HD_ENABLED) )

    #if (UART1_RX_INTERRUPT_ENABLED)
        #define UART1_EnableRxInt()  CyIntEnable (UART1_RX_VECT_NUM)
        #define UART1_DisableRxInt() CyIntDisable(UART1_RX_VECT_NUM)
        CY_ISR_PROTO(UART1_RXISR);
    #endif /* UART1_RX_INTERRUPT_ENABLED */

    void UART1_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void UART1_SetRxAddress1(uint8 address) ;
    void UART1_SetRxAddress2(uint8 address) ;

    void  UART1_SetRxInterruptMode(uint8 intSrc) ;
    uint8 UART1_ReadRxData(void) ;
    uint8 UART1_ReadRxStatus(void) ;
    uint8 UART1_GetChar(void) ;
    uint16 UART1_GetByte(void) ;
    uint8 UART1_GetRxBufferSize(void)
                                                            ;
    void UART1_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define UART1_GetRxInterruptSource   UART1_ReadRxStatus

#endif /* End (UART1_RX_ENABLED) || (UART1_HD_ENABLED) */

/* Only if TX is enabled */
#if(UART1_TX_ENABLED || UART1_HD_ENABLED)

    #if(UART1_TX_INTERRUPT_ENABLED)
        #define UART1_EnableTxInt()  CyIntEnable (UART1_TX_VECT_NUM)
        #define UART1_DisableTxInt() CyIntDisable(UART1_TX_VECT_NUM)
        #define UART1_SetPendingTxInt() CyIntSetPending(UART1_TX_VECT_NUM)
        #define UART1_ClearPendingTxInt() CyIntClearPending(UART1_TX_VECT_NUM)
        CY_ISR_PROTO(UART1_TXISR);
    #endif /* UART1_TX_INTERRUPT_ENABLED */

    void UART1_SetTxInterruptMode(uint8 intSrc) ;
    void UART1_WriteTxData(uint8 txDataByte) ;
    uint8 UART1_ReadTxStatus(void) ;
    void UART1_PutChar(uint8 txDataByte) ;
    void UART1_PutString(const char8 string[]) ;
    void UART1_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void UART1_PutCRLF(uint8 txDataByte) ;
    void UART1_ClearTxBuffer(void) ;
    void UART1_SetTxAddressMode(uint8 addressMode) ;
    void UART1_SendBreak(uint8 retMode) ;
    uint8 UART1_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define UART1_PutStringConst         UART1_PutString
    #define UART1_PutArrayConst          UART1_PutArray
    #define UART1_GetTxInterruptSource   UART1_ReadTxStatus

#endif /* End UART1_TX_ENABLED || UART1_HD_ENABLED */

#if(UART1_HD_ENABLED)
    void UART1_LoadRxConfig(void) ;
    void UART1_LoadTxConfig(void) ;
#endif /* End UART1_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART1) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    UART1_CyBtldrCommStart(void) CYSMALL ;
    void    UART1_CyBtldrCommStop(void) CYSMALL ;
    void    UART1_CyBtldrCommReset(void) CYSMALL ;
    cystatus UART1_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus UART1_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART1)
        #define CyBtldrCommStart    UART1_CyBtldrCommStart
        #define CyBtldrCommStop     UART1_CyBtldrCommStop
        #define CyBtldrCommReset    UART1_CyBtldrCommReset
        #define CyBtldrCommWrite    UART1_CyBtldrCommWrite
        #define CyBtldrCommRead     UART1_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART1) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define UART1_BYTE2BYTE_TIME_OUT (25u)
    #define UART1_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define UART1_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define UART1_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define UART1_SET_SPACE      (0x00u)
#define UART1_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (UART1_TX_ENABLED) || (UART1_HD_ENABLED) )
    #if(UART1_TX_INTERRUPT_ENABLED)
        #define UART1_TX_VECT_NUM            (uint8)UART1_TXInternalInterrupt__INTC_NUMBER
        #define UART1_TX_PRIOR_NUM           (uint8)UART1_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* UART1_TX_INTERRUPT_ENABLED */

    #define UART1_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define UART1_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define UART1_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(UART1_TX_ENABLED)
        #define UART1_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (UART1_HD_ENABLED) */
        #define UART1_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (UART1_TX_ENABLED) */

    #define UART1_TX_STS_COMPLETE            (uint8)(0x01u << UART1_TX_STS_COMPLETE_SHIFT)
    #define UART1_TX_STS_FIFO_EMPTY          (uint8)(0x01u << UART1_TX_STS_FIFO_EMPTY_SHIFT)
    #define UART1_TX_STS_FIFO_FULL           (uint8)(0x01u << UART1_TX_STS_FIFO_FULL_SHIFT)
    #define UART1_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << UART1_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (UART1_TX_ENABLED) || (UART1_HD_ENABLED)*/

#if( (UART1_RX_ENABLED) || (UART1_HD_ENABLED) )
    #if(UART1_RX_INTERRUPT_ENABLED)
        #define UART1_RX_VECT_NUM            (uint8)UART1_RXInternalInterrupt__INTC_NUMBER
        #define UART1_RX_PRIOR_NUM           (uint8)UART1_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* UART1_RX_INTERRUPT_ENABLED */
    #define UART1_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define UART1_RX_STS_BREAK_SHIFT             (0x01u)
    #define UART1_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define UART1_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define UART1_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define UART1_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define UART1_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define UART1_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define UART1_RX_STS_MRKSPC           (uint8)(0x01u << UART1_RX_STS_MRKSPC_SHIFT)
    #define UART1_RX_STS_BREAK            (uint8)(0x01u << UART1_RX_STS_BREAK_SHIFT)
    #define UART1_RX_STS_PAR_ERROR        (uint8)(0x01u << UART1_RX_STS_PAR_ERROR_SHIFT)
    #define UART1_RX_STS_STOP_ERROR       (uint8)(0x01u << UART1_RX_STS_STOP_ERROR_SHIFT)
    #define UART1_RX_STS_OVERRUN          (uint8)(0x01u << UART1_RX_STS_OVERRUN_SHIFT)
    #define UART1_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << UART1_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define UART1_RX_STS_ADDR_MATCH       (uint8)(0x01u << UART1_RX_STS_ADDR_MATCH_SHIFT)
    #define UART1_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << UART1_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define UART1_RX_HW_MASK                     (0x7Fu)
#endif /* End (UART1_RX_ENABLED) || (UART1_HD_ENABLED) */

/* Control Register definitions */
#define UART1_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define UART1_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define UART1_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define UART1_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define UART1_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define UART1_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define UART1_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define UART1_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define UART1_CTRL_HD_SEND               (uint8)(0x01u << UART1_CTRL_HD_SEND_SHIFT)
#define UART1_CTRL_HD_SEND_BREAK         (uint8)(0x01u << UART1_CTRL_HD_SEND_BREAK_SHIFT)
#define UART1_CTRL_MARK                  (uint8)(0x01u << UART1_CTRL_MARK_SHIFT)
#define UART1_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << UART1_CTRL_PARITY_TYPE0_SHIFT)
#define UART1_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << UART1_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define UART1_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define UART1_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define UART1_SEND_BREAK                         (0x00u)
#define UART1_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define UART1_REINIT                             (0x02u)
#define UART1_SEND_WAIT_REINIT                   (0x03u)

#define UART1_OVER_SAMPLE_8                      (8u)
#define UART1_OVER_SAMPLE_16                     (16u)

#define UART1_BIT_CENTER                         (UART1_OVER_SAMPLE_COUNT - 2u)

#define UART1_FIFO_LENGTH                        (4u)
#define UART1_NUMBER_OF_START_BIT                (1u)
#define UART1_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define UART1_TXBITCTR_BREAKBITS8X   ((UART1_BREAK_BITS_TX * UART1_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define UART1_TXBITCTR_BREAKBITS ((UART1_BREAK_BITS_TX * UART1_OVER_SAMPLE_COUNT) - 1u)

#define UART1_HALF_BIT_COUNT   \
                            (((UART1_OVER_SAMPLE_COUNT / 2u) + (UART1_USE23POLLING * 1u)) - 2u)
#if (UART1_OVER_SAMPLE_COUNT == UART1_OVER_SAMPLE_8)
    #define UART1_HD_TXBITCTR_INIT   (((UART1_BREAK_BITS_TX + \
                            UART1_NUMBER_OF_START_BIT) * UART1_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define UART1_RXBITCTR_INIT  ((((UART1_BREAK_BITS_RX + UART1_NUMBER_OF_START_BIT) \
                            * UART1_OVER_SAMPLE_COUNT) + UART1_HALF_BIT_COUNT) - 1u)

#else /* UART1_OVER_SAMPLE_COUNT == UART1_OVER_SAMPLE_16 */
    #define UART1_HD_TXBITCTR_INIT   ((8u * UART1_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define UART1_RXBITCTR_INIT      (((7u * UART1_OVER_SAMPLE_COUNT) - 1u) + \
                                                      UART1_HALF_BIT_COUNT)
#endif /* End UART1_OVER_SAMPLE_COUNT */

#define UART1_HD_RXBITCTR_INIT                   UART1_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 UART1_initVar;
#if (UART1_TX_INTERRUPT_ENABLED && UART1_TX_ENABLED)
    extern volatile uint8 UART1_txBuffer[UART1_TX_BUFFER_SIZE];
    extern volatile uint8 UART1_txBufferRead;
    extern uint8 UART1_txBufferWrite;
#endif /* (UART1_TX_INTERRUPT_ENABLED && UART1_TX_ENABLED) */
#if (UART1_RX_INTERRUPT_ENABLED && (UART1_RX_ENABLED || UART1_HD_ENABLED))
    extern uint8 UART1_errorStatus;
    extern volatile uint8 UART1_rxBuffer[UART1_RX_BUFFER_SIZE];
    extern volatile uint8 UART1_rxBufferRead;
    extern volatile uint8 UART1_rxBufferWrite;
    extern volatile uint8 UART1_rxBufferLoopDetect;
    extern volatile uint8 UART1_rxBufferOverflow;
    #if (UART1_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 UART1_rxAddressMode;
        extern volatile uint8 UART1_rxAddressDetected;
    #endif /* (UART1_RXHW_ADDRESS_ENABLED) */
#endif /* (UART1_RX_INTERRUPT_ENABLED && (UART1_RX_ENABLED || UART1_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define UART1__B_UART__AM_SW_BYTE_BYTE 1
#define UART1__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define UART1__B_UART__AM_HW_BYTE_BY_BYTE 3
#define UART1__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define UART1__B_UART__AM_NONE 0

#define UART1__B_UART__NONE_REVB 0
#define UART1__B_UART__EVEN_REVB 1
#define UART1__B_UART__ODD_REVB 2
#define UART1__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define UART1_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define UART1_NUMBER_OF_STOP_BITS    (1u)

#if (UART1_RXHW_ADDRESS_ENABLED)
    #define UART1_RX_ADDRESS_MODE    (0u)
    #define UART1_RX_HW_ADDRESS1     (0u)
    #define UART1_RX_HW_ADDRESS2     (0u)
#endif /* (UART1_RXHW_ADDRESS_ENABLED) */

#define UART1_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << UART1_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << UART1_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << UART1_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << UART1_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << UART1_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << UART1_RX_STS_BREAK_SHIFT) \
                                        | (0 << UART1_RX_STS_OVERRUN_SHIFT))

#define UART1_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << UART1_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << UART1_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << UART1_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << UART1_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define UART1_CONTROL_REG \
                            (* (reg8 *) UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define UART1_CONTROL_PTR \
                            (  (reg8 *) UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(UART1_TX_ENABLED)
    #define UART1_TXDATA_REG          (* (reg8 *) UART1_BUART_sTX_TxShifter_u0__F0_REG)
    #define UART1_TXDATA_PTR          (  (reg8 *) UART1_BUART_sTX_TxShifter_u0__F0_REG)
    #define UART1_TXDATA_AUX_CTL_REG  (* (reg8 *) UART1_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define UART1_TXDATA_AUX_CTL_PTR  (  (reg8 *) UART1_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define UART1_TXSTATUS_REG        (* (reg8 *) UART1_BUART_sTX_TxSts__STATUS_REG)
    #define UART1_TXSTATUS_PTR        (  (reg8 *) UART1_BUART_sTX_TxSts__STATUS_REG)
    #define UART1_TXSTATUS_MASK_REG   (* (reg8 *) UART1_BUART_sTX_TxSts__MASK_REG)
    #define UART1_TXSTATUS_MASK_PTR   (  (reg8 *) UART1_BUART_sTX_TxSts__MASK_REG)
    #define UART1_TXSTATUS_ACTL_REG   (* (reg8 *) UART1_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define UART1_TXSTATUS_ACTL_PTR   (  (reg8 *) UART1_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(UART1_TXCLKGEN_DP)
        #define UART1_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define UART1_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define UART1_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define UART1_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define UART1_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define UART1_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define UART1_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define UART1_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define UART1_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define UART1_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) UART1_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* UART1_TXCLKGEN_DP */

#endif /* End UART1_TX_ENABLED */

#if(UART1_HD_ENABLED)

    #define UART1_TXDATA_REG             (* (reg8 *) UART1_BUART_sRX_RxShifter_u0__F1_REG )
    #define UART1_TXDATA_PTR             (  (reg8 *) UART1_BUART_sRX_RxShifter_u0__F1_REG )
    #define UART1_TXDATA_AUX_CTL_REG     (* (reg8 *) UART1_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define UART1_TXDATA_AUX_CTL_PTR     (  (reg8 *) UART1_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define UART1_TXSTATUS_REG           (* (reg8 *) UART1_BUART_sRX_RxSts__STATUS_REG )
    #define UART1_TXSTATUS_PTR           (  (reg8 *) UART1_BUART_sRX_RxSts__STATUS_REG )
    #define UART1_TXSTATUS_MASK_REG      (* (reg8 *) UART1_BUART_sRX_RxSts__MASK_REG )
    #define UART1_TXSTATUS_MASK_PTR      (  (reg8 *) UART1_BUART_sRX_RxSts__MASK_REG )
    #define UART1_TXSTATUS_ACTL_REG      (* (reg8 *) UART1_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define UART1_TXSTATUS_ACTL_PTR      (  (reg8 *) UART1_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End UART1_HD_ENABLED */

#if( (UART1_RX_ENABLED) || (UART1_HD_ENABLED) )
    #define UART1_RXDATA_REG             (* (reg8 *) UART1_BUART_sRX_RxShifter_u0__F0_REG )
    #define UART1_RXDATA_PTR             (  (reg8 *) UART1_BUART_sRX_RxShifter_u0__F0_REG )
    #define UART1_RXADDRESS1_REG         (* (reg8 *) UART1_BUART_sRX_RxShifter_u0__D0_REG )
    #define UART1_RXADDRESS1_PTR         (  (reg8 *) UART1_BUART_sRX_RxShifter_u0__D0_REG )
    #define UART1_RXADDRESS2_REG         (* (reg8 *) UART1_BUART_sRX_RxShifter_u0__D1_REG )
    #define UART1_RXADDRESS2_PTR         (  (reg8 *) UART1_BUART_sRX_RxShifter_u0__D1_REG )
    #define UART1_RXDATA_AUX_CTL_REG     (* (reg8 *) UART1_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define UART1_RXBITCTR_PERIOD_REG    (* (reg8 *) UART1_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define UART1_RXBITCTR_PERIOD_PTR    (  (reg8 *) UART1_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define UART1_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) UART1_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define UART1_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) UART1_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define UART1_RXBITCTR_COUNTER_REG   (* (reg8 *) UART1_BUART_sRX_RxBitCounter__COUNT_REG )
    #define UART1_RXBITCTR_COUNTER_PTR   (  (reg8 *) UART1_BUART_sRX_RxBitCounter__COUNT_REG )

    #define UART1_RXSTATUS_REG           (* (reg8 *) UART1_BUART_sRX_RxSts__STATUS_REG )
    #define UART1_RXSTATUS_PTR           (  (reg8 *) UART1_BUART_sRX_RxSts__STATUS_REG )
    #define UART1_RXSTATUS_MASK_REG      (* (reg8 *) UART1_BUART_sRX_RxSts__MASK_REG )
    #define UART1_RXSTATUS_MASK_PTR      (  (reg8 *) UART1_BUART_sRX_RxSts__MASK_REG )
    #define UART1_RXSTATUS_ACTL_REG      (* (reg8 *) UART1_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define UART1_RXSTATUS_ACTL_PTR      (  (reg8 *) UART1_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (UART1_RX_ENABLED) || (UART1_HD_ENABLED) */

#if(UART1_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define UART1_INTCLOCK_CLKEN_REG     (* (reg8 *) UART1_IntClock__PM_ACT_CFG)
    #define UART1_INTCLOCK_CLKEN_PTR     (  (reg8 *) UART1_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define UART1_INTCLOCK_CLKEN_MASK    UART1_IntClock__PM_ACT_MSK
#endif /* End UART1_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(UART1_TX_ENABLED)
    #define UART1_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End UART1_TX_ENABLED */

#if(UART1_HD_ENABLED)
    #define UART1_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End UART1_HD_ENABLED */

#if( (UART1_RX_ENABLED) || (UART1_HD_ENABLED) )
    #define UART1_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (UART1_RX_ENABLED) || (UART1_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define UART1_WAIT_1_MS      UART1_BL_CHK_DELAY_MS   

#define UART1_TXBUFFERSIZE   UART1_TX_BUFFER_SIZE
#define UART1_RXBUFFERSIZE   UART1_RX_BUFFER_SIZE

#if (UART1_RXHW_ADDRESS_ENABLED)
    #define UART1_RXADDRESSMODE  UART1_RX_ADDRESS_MODE
    #define UART1_RXHWADDRESS1   UART1_RX_HW_ADDRESS1
    #define UART1_RXHWADDRESS2   UART1_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define UART1_RXAddressMode  UART1_RXADDRESSMODE
#endif /* (UART1_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define UART1_initvar                    UART1_initVar

#define UART1_RX_Enabled                 UART1_RX_ENABLED
#define UART1_TX_Enabled                 UART1_TX_ENABLED
#define UART1_HD_Enabled                 UART1_HD_ENABLED
#define UART1_RX_IntInterruptEnabled     UART1_RX_INTERRUPT_ENABLED
#define UART1_TX_IntInterruptEnabled     UART1_TX_INTERRUPT_ENABLED
#define UART1_InternalClockUsed          UART1_INTERNAL_CLOCK_USED
#define UART1_RXHW_Address_Enabled       UART1_RXHW_ADDRESS_ENABLED
#define UART1_OverSampleCount            UART1_OVER_SAMPLE_COUNT
#define UART1_ParityType                 UART1_PARITY_TYPE

#if( UART1_TX_ENABLED && (UART1_TXBUFFERSIZE > UART1_FIFO_LENGTH))
    #define UART1_TXBUFFER               UART1_txBuffer
    #define UART1_TXBUFFERREAD           UART1_txBufferRead
    #define UART1_TXBUFFERWRITE          UART1_txBufferWrite
#endif /* End UART1_TX_ENABLED */
#if( ( UART1_RX_ENABLED || UART1_HD_ENABLED ) && \
     (UART1_RXBUFFERSIZE > UART1_FIFO_LENGTH) )
    #define UART1_RXBUFFER               UART1_rxBuffer
    #define UART1_RXBUFFERREAD           UART1_rxBufferRead
    #define UART1_RXBUFFERWRITE          UART1_rxBufferWrite
    #define UART1_RXBUFFERLOOPDETECT     UART1_rxBufferLoopDetect
    #define UART1_RXBUFFER_OVERFLOW      UART1_rxBufferOverflow
#endif /* End UART1_RX_ENABLED */

#ifdef UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define UART1_CONTROL                UART1_CONTROL_REG
#endif /* End UART1_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(UART1_TX_ENABLED)
    #define UART1_TXDATA                 UART1_TXDATA_REG
    #define UART1_TXSTATUS               UART1_TXSTATUS_REG
    #define UART1_TXSTATUS_MASK          UART1_TXSTATUS_MASK_REG
    #define UART1_TXSTATUS_ACTL          UART1_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(UART1_TXCLKGEN_DP)
        #define UART1_TXBITCLKGEN_CTR        UART1_TXBITCLKGEN_CTR_REG
        #define UART1_TXBITCLKTX_COMPLETE    UART1_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define UART1_TXBITCTR_PERIOD        UART1_TXBITCTR_PERIOD_REG
        #define UART1_TXBITCTR_CONTROL       UART1_TXBITCTR_CONTROL_REG
        #define UART1_TXBITCTR_COUNTER       UART1_TXBITCTR_COUNTER_REG
    #endif /* UART1_TXCLKGEN_DP */
#endif /* End UART1_TX_ENABLED */

#if(UART1_HD_ENABLED)
    #define UART1_TXDATA                 UART1_TXDATA_REG
    #define UART1_TXSTATUS               UART1_TXSTATUS_REG
    #define UART1_TXSTATUS_MASK          UART1_TXSTATUS_MASK_REG
    #define UART1_TXSTATUS_ACTL          UART1_TXSTATUS_ACTL_REG
#endif /* End UART1_HD_ENABLED */

#if( (UART1_RX_ENABLED) || (UART1_HD_ENABLED) )
    #define UART1_RXDATA                 UART1_RXDATA_REG
    #define UART1_RXADDRESS1             UART1_RXADDRESS1_REG
    #define UART1_RXADDRESS2             UART1_RXADDRESS2_REG
    #define UART1_RXBITCTR_PERIOD        UART1_RXBITCTR_PERIOD_REG
    #define UART1_RXBITCTR_CONTROL       UART1_RXBITCTR_CONTROL_REG
    #define UART1_RXBITCTR_COUNTER       UART1_RXBITCTR_COUNTER_REG
    #define UART1_RXSTATUS               UART1_RXSTATUS_REG
    #define UART1_RXSTATUS_MASK          UART1_RXSTATUS_MASK_REG
    #define UART1_RXSTATUS_ACTL          UART1_RXSTATUS_ACTL_REG
#endif /* End  (UART1_RX_ENABLED) || (UART1_HD_ENABLED) */

#if(UART1_INTERNAL_CLOCK_USED)
    #define UART1_INTCLOCK_CLKEN         UART1_INTCLOCK_CLKEN_REG
#endif /* End UART1_INTERNAL_CLOCK_USED */

#define UART1_WAIT_FOR_COMLETE_REINIT    UART1_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_UART1_H */


/* [] END OF FILE */
