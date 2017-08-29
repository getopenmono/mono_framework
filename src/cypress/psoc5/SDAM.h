/*******************************************************************************
* File Name: SDAM.h  
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

#if !defined(CY_PINS_SDAM_H) /* Pins SDAM_H */
#define CY_PINS_SDAM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SDAM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SDAM__PORT == 15 && ((SDAM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    SDAM_Write(uint8 value);
void    SDAM_SetDriveMode(uint8 mode);
uint8   SDAM_ReadDataReg(void);
uint8   SDAM_Read(void);
void    SDAM_SetInterruptMode(uint16 position, uint16 mode);
uint8   SDAM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SDAM_SetDriveMode() function.
     *  @{
     */
        #define SDAM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SDAM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SDAM_DM_RES_UP          PIN_DM_RES_UP
        #define SDAM_DM_RES_DWN         PIN_DM_RES_DWN
        #define SDAM_DM_OD_LO           PIN_DM_OD_LO
        #define SDAM_DM_OD_HI           PIN_DM_OD_HI
        #define SDAM_DM_STRONG          PIN_DM_STRONG
        #define SDAM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define SDAM_MASK               SDAM__MASK
#define SDAM_SHIFT              SDAM__SHIFT
#define SDAM_WIDTH              1u

/* Interrupt constants */
#if defined(SDAM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SDAM_SetInterruptMode() function.
     *  @{
     */
        #define SDAM_INTR_NONE      (uint16)(0x0000u)
        #define SDAM_INTR_RISING    (uint16)(0x0001u)
        #define SDAM_INTR_FALLING   (uint16)(0x0002u)
        #define SDAM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define SDAM_INTR_MASK      (0x01u) 
#endif /* (SDAM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SDAM_PS                     (* (reg8 *) SDAM__PS)
/* Data Register */
#define SDAM_DR                     (* (reg8 *) SDAM__DR)
/* Port Number */
#define SDAM_PRT_NUM                (* (reg8 *) SDAM__PRT) 
/* Connect to Analog Globals */                                                  
#define SDAM_AG                     (* (reg8 *) SDAM__AG)                       
/* Analog MUX bux enable */
#define SDAM_AMUX                   (* (reg8 *) SDAM__AMUX) 
/* Bidirectional Enable */                                                        
#define SDAM_BIE                    (* (reg8 *) SDAM__BIE)
/* Bit-mask for Aliased Register Access */
#define SDAM_BIT_MASK               (* (reg8 *) SDAM__BIT_MASK)
/* Bypass Enable */
#define SDAM_BYP                    (* (reg8 *) SDAM__BYP)
/* Port wide control signals */                                                   
#define SDAM_CTL                    (* (reg8 *) SDAM__CTL)
/* Drive Modes */
#define SDAM_DM0                    (* (reg8 *) SDAM__DM0) 
#define SDAM_DM1                    (* (reg8 *) SDAM__DM1)
#define SDAM_DM2                    (* (reg8 *) SDAM__DM2) 
/* Input Buffer Disable Override */
#define SDAM_INP_DIS                (* (reg8 *) SDAM__INP_DIS)
/* LCD Common or Segment Drive */
#define SDAM_LCD_COM_SEG            (* (reg8 *) SDAM__LCD_COM_SEG)
/* Enable Segment LCD */
#define SDAM_LCD_EN                 (* (reg8 *) SDAM__LCD_EN)
/* Slew Rate Control */
#define SDAM_SLW                    (* (reg8 *) SDAM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SDAM_PRTDSI__CAPS_SEL       (* (reg8 *) SDAM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SDAM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SDAM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SDAM_PRTDSI__OE_SEL0        (* (reg8 *) SDAM__PRTDSI__OE_SEL0) 
#define SDAM_PRTDSI__OE_SEL1        (* (reg8 *) SDAM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SDAM_PRTDSI__OUT_SEL0       (* (reg8 *) SDAM__PRTDSI__OUT_SEL0) 
#define SDAM_PRTDSI__OUT_SEL1       (* (reg8 *) SDAM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SDAM_PRTDSI__SYNC_OUT       (* (reg8 *) SDAM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(SDAM__SIO_CFG)
    #define SDAM_SIO_HYST_EN        (* (reg8 *) SDAM__SIO_HYST_EN)
    #define SDAM_SIO_REG_HIFREQ     (* (reg8 *) SDAM__SIO_REG_HIFREQ)
    #define SDAM_SIO_CFG            (* (reg8 *) SDAM__SIO_CFG)
    #define SDAM_SIO_DIFF           (* (reg8 *) SDAM__SIO_DIFF)
#endif /* (SDAM__SIO_CFG) */

/* Interrupt Registers */
#if defined(SDAM__INTSTAT)
    #define SDAM_INTSTAT            (* (reg8 *) SDAM__INTSTAT)
    #define SDAM_SNAP               (* (reg8 *) SDAM__SNAP)
    
	#define SDAM_0_INTTYPE_REG 		(* (reg8 *) SDAM__0__INTTYPE)
#endif /* (SDAM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SDAM_H */


/* [] END OF FILE */
