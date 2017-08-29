/*******************************************************************************
* File Name: SCLM.h  
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

#if !defined(CY_PINS_SCLM_H) /* Pins SCLM_H */
#define CY_PINS_SCLM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SCLM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SCLM__PORT == 15 && ((SCLM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    SCLM_Write(uint8 value);
void    SCLM_SetDriveMode(uint8 mode);
uint8   SCLM_ReadDataReg(void);
uint8   SCLM_Read(void);
void    SCLM_SetInterruptMode(uint16 position, uint16 mode);
uint8   SCLM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SCLM_SetDriveMode() function.
     *  @{
     */
        #define SCLM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SCLM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SCLM_DM_RES_UP          PIN_DM_RES_UP
        #define SCLM_DM_RES_DWN         PIN_DM_RES_DWN
        #define SCLM_DM_OD_LO           PIN_DM_OD_LO
        #define SCLM_DM_OD_HI           PIN_DM_OD_HI
        #define SCLM_DM_STRONG          PIN_DM_STRONG
        #define SCLM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define SCLM_MASK               SCLM__MASK
#define SCLM_SHIFT              SCLM__SHIFT
#define SCLM_WIDTH              1u

/* Interrupt constants */
#if defined(SCLM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SCLM_SetInterruptMode() function.
     *  @{
     */
        #define SCLM_INTR_NONE      (uint16)(0x0000u)
        #define SCLM_INTR_RISING    (uint16)(0x0001u)
        #define SCLM_INTR_FALLING   (uint16)(0x0002u)
        #define SCLM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define SCLM_INTR_MASK      (0x01u) 
#endif /* (SCLM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SCLM_PS                     (* (reg8 *) SCLM__PS)
/* Data Register */
#define SCLM_DR                     (* (reg8 *) SCLM__DR)
/* Port Number */
#define SCLM_PRT_NUM                (* (reg8 *) SCLM__PRT) 
/* Connect to Analog Globals */                                                  
#define SCLM_AG                     (* (reg8 *) SCLM__AG)                       
/* Analog MUX bux enable */
#define SCLM_AMUX                   (* (reg8 *) SCLM__AMUX) 
/* Bidirectional Enable */                                                        
#define SCLM_BIE                    (* (reg8 *) SCLM__BIE)
/* Bit-mask for Aliased Register Access */
#define SCLM_BIT_MASK               (* (reg8 *) SCLM__BIT_MASK)
/* Bypass Enable */
#define SCLM_BYP                    (* (reg8 *) SCLM__BYP)
/* Port wide control signals */                                                   
#define SCLM_CTL                    (* (reg8 *) SCLM__CTL)
/* Drive Modes */
#define SCLM_DM0                    (* (reg8 *) SCLM__DM0) 
#define SCLM_DM1                    (* (reg8 *) SCLM__DM1)
#define SCLM_DM2                    (* (reg8 *) SCLM__DM2) 
/* Input Buffer Disable Override */
#define SCLM_INP_DIS                (* (reg8 *) SCLM__INP_DIS)
/* LCD Common or Segment Drive */
#define SCLM_LCD_COM_SEG            (* (reg8 *) SCLM__LCD_COM_SEG)
/* Enable Segment LCD */
#define SCLM_LCD_EN                 (* (reg8 *) SCLM__LCD_EN)
/* Slew Rate Control */
#define SCLM_SLW                    (* (reg8 *) SCLM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SCLM_PRTDSI__CAPS_SEL       (* (reg8 *) SCLM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SCLM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SCLM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SCLM_PRTDSI__OE_SEL0        (* (reg8 *) SCLM__PRTDSI__OE_SEL0) 
#define SCLM_PRTDSI__OE_SEL1        (* (reg8 *) SCLM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SCLM_PRTDSI__OUT_SEL0       (* (reg8 *) SCLM__PRTDSI__OUT_SEL0) 
#define SCLM_PRTDSI__OUT_SEL1       (* (reg8 *) SCLM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SCLM_PRTDSI__SYNC_OUT       (* (reg8 *) SCLM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(SCLM__SIO_CFG)
    #define SCLM_SIO_HYST_EN        (* (reg8 *) SCLM__SIO_HYST_EN)
    #define SCLM_SIO_REG_HIFREQ     (* (reg8 *) SCLM__SIO_REG_HIFREQ)
    #define SCLM_SIO_CFG            (* (reg8 *) SCLM__SIO_CFG)
    #define SCLM_SIO_DIFF           (* (reg8 *) SCLM__SIO_DIFF)
#endif /* (SCLM__SIO_CFG) */

/* Interrupt Registers */
#if defined(SCLM__INTSTAT)
    #define SCLM_INTSTAT            (* (reg8 *) SCLM__INTSTAT)
    #define SCLM_SNAP               (* (reg8 *) SCLM__SNAP)
    
	#define SCLM_0_INTTYPE_REG 		(* (reg8 *) SCLM__0__INTTYPE)
#endif /* (SCLM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SCLM_H */


/* [] END OF FILE */
