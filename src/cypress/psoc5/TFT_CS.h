/*******************************************************************************
* File Name: TFT_CS.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_TFT_CS_H) /* Pins TFT_CS_H */
#define CY_PINS_TFT_CS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TFT_CS_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TFT_CS__PORT == 15 && ((TFT_CS__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TFT_CS_Write(uint8 value);
void    TFT_CS_SetDriveMode(uint8 mode);
uint8   TFT_CS_ReadDataReg(void);
uint8   TFT_CS_Read(void);
void    TFT_CS_SetInterruptMode(uint16 position, uint16 mode);
uint8   TFT_CS_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TFT_CS_SetDriveMode() function.
     *  @{
     */
        #define TFT_CS_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TFT_CS_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TFT_CS_DM_RES_UP          PIN_DM_RES_UP
        #define TFT_CS_DM_RES_DWN         PIN_DM_RES_DWN
        #define TFT_CS_DM_OD_LO           PIN_DM_OD_LO
        #define TFT_CS_DM_OD_HI           PIN_DM_OD_HI
        #define TFT_CS_DM_STRONG          PIN_DM_STRONG
        #define TFT_CS_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TFT_CS_MASK               TFT_CS__MASK
#define TFT_CS_SHIFT              TFT_CS__SHIFT
#define TFT_CS_WIDTH              1u

/* Interrupt constants */
#if defined(TFT_CS__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TFT_CS_SetInterruptMode() function.
     *  @{
     */
        #define TFT_CS_INTR_NONE      (uint16)(0x0000u)
        #define TFT_CS_INTR_RISING    (uint16)(0x0001u)
        #define TFT_CS_INTR_FALLING   (uint16)(0x0002u)
        #define TFT_CS_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TFT_CS_INTR_MASK      (0x01u) 
#endif /* (TFT_CS__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TFT_CS_PS                     (* (reg8 *) TFT_CS__PS)
/* Data Register */
#define TFT_CS_DR                     (* (reg8 *) TFT_CS__DR)
/* Port Number */
#define TFT_CS_PRT_NUM                (* (reg8 *) TFT_CS__PRT) 
/* Connect to Analog Globals */                                                  
#define TFT_CS_AG                     (* (reg8 *) TFT_CS__AG)                       
/* Analog MUX bux enable */
#define TFT_CS_AMUX                   (* (reg8 *) TFT_CS__AMUX) 
/* Bidirectional Enable */                                                        
#define TFT_CS_BIE                    (* (reg8 *) TFT_CS__BIE)
/* Bit-mask for Aliased Register Access */
#define TFT_CS_BIT_MASK               (* (reg8 *) TFT_CS__BIT_MASK)
/* Bypass Enable */
#define TFT_CS_BYP                    (* (reg8 *) TFT_CS__BYP)
/* Port wide control signals */                                                   
#define TFT_CS_CTL                    (* (reg8 *) TFT_CS__CTL)
/* Drive Modes */
#define TFT_CS_DM0                    (* (reg8 *) TFT_CS__DM0) 
#define TFT_CS_DM1                    (* (reg8 *) TFT_CS__DM1)
#define TFT_CS_DM2                    (* (reg8 *) TFT_CS__DM2) 
/* Input Buffer Disable Override */
#define TFT_CS_INP_DIS                (* (reg8 *) TFT_CS__INP_DIS)
/* LCD Common or Segment Drive */
#define TFT_CS_LCD_COM_SEG            (* (reg8 *) TFT_CS__LCD_COM_SEG)
/* Enable Segment LCD */
#define TFT_CS_LCD_EN                 (* (reg8 *) TFT_CS__LCD_EN)
/* Slew Rate Control */
#define TFT_CS_SLW                    (* (reg8 *) TFT_CS__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TFT_CS_PRTDSI__CAPS_SEL       (* (reg8 *) TFT_CS__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TFT_CS_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TFT_CS__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TFT_CS_PRTDSI__OE_SEL0        (* (reg8 *) TFT_CS__PRTDSI__OE_SEL0) 
#define TFT_CS_PRTDSI__OE_SEL1        (* (reg8 *) TFT_CS__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TFT_CS_PRTDSI__OUT_SEL0       (* (reg8 *) TFT_CS__PRTDSI__OUT_SEL0) 
#define TFT_CS_PRTDSI__OUT_SEL1       (* (reg8 *) TFT_CS__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TFT_CS_PRTDSI__SYNC_OUT       (* (reg8 *) TFT_CS__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TFT_CS__SIO_CFG)
    #define TFT_CS_SIO_HYST_EN        (* (reg8 *) TFT_CS__SIO_HYST_EN)
    #define TFT_CS_SIO_REG_HIFREQ     (* (reg8 *) TFT_CS__SIO_REG_HIFREQ)
    #define TFT_CS_SIO_CFG            (* (reg8 *) TFT_CS__SIO_CFG)
    #define TFT_CS_SIO_DIFF           (* (reg8 *) TFT_CS__SIO_DIFF)
#endif /* (TFT_CS__SIO_CFG) */

/* Interrupt Registers */
#if defined(TFT_CS__INTSTAT)
    #define TFT_CS_INTSTAT            (* (reg8 *) TFT_CS__INTSTAT)
    #define TFT_CS_SNAP               (* (reg8 *) TFT_CS__SNAP)
    
	#define TFT_CS_0_INTTYPE_REG 		(* (reg8 *) TFT_CS__0__INTTYPE)
#endif /* (TFT_CS__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TFT_CS_H */


/* [] END OF FILE */
