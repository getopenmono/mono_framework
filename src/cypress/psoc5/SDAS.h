/*******************************************************************************
* File Name: SDAS.h  
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

#if !defined(CY_PINS_SDAS_H) /* Pins SDAS_H */
#define CY_PINS_SDAS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SDAS_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SDAS__PORT == 15 && ((SDAS__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    SDAS_Write(uint8 value);
void    SDAS_SetDriveMode(uint8 mode);
uint8   SDAS_ReadDataReg(void);
uint8   SDAS_Read(void);
void    SDAS_SetInterruptMode(uint16 position, uint16 mode);
uint8   SDAS_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SDAS_SetDriveMode() function.
     *  @{
     */
        #define SDAS_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SDAS_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SDAS_DM_RES_UP          PIN_DM_RES_UP
        #define SDAS_DM_RES_DWN         PIN_DM_RES_DWN
        #define SDAS_DM_OD_LO           PIN_DM_OD_LO
        #define SDAS_DM_OD_HI           PIN_DM_OD_HI
        #define SDAS_DM_STRONG          PIN_DM_STRONG
        #define SDAS_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define SDAS_MASK               SDAS__MASK
#define SDAS_SHIFT              SDAS__SHIFT
#define SDAS_WIDTH              1u

/* Interrupt constants */
#if defined(SDAS__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SDAS_SetInterruptMode() function.
     *  @{
     */
        #define SDAS_INTR_NONE      (uint16)(0x0000u)
        #define SDAS_INTR_RISING    (uint16)(0x0001u)
        #define SDAS_INTR_FALLING   (uint16)(0x0002u)
        #define SDAS_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define SDAS_INTR_MASK      (0x01u) 
#endif /* (SDAS__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SDAS_PS                     (* (reg8 *) SDAS__PS)
/* Data Register */
#define SDAS_DR                     (* (reg8 *) SDAS__DR)
/* Port Number */
#define SDAS_PRT_NUM                (* (reg8 *) SDAS__PRT) 
/* Connect to Analog Globals */                                                  
#define SDAS_AG                     (* (reg8 *) SDAS__AG)                       
/* Analog MUX bux enable */
#define SDAS_AMUX                   (* (reg8 *) SDAS__AMUX) 
/* Bidirectional Enable */                                                        
#define SDAS_BIE                    (* (reg8 *) SDAS__BIE)
/* Bit-mask for Aliased Register Access */
#define SDAS_BIT_MASK               (* (reg8 *) SDAS__BIT_MASK)
/* Bypass Enable */
#define SDAS_BYP                    (* (reg8 *) SDAS__BYP)
/* Port wide control signals */                                                   
#define SDAS_CTL                    (* (reg8 *) SDAS__CTL)
/* Drive Modes */
#define SDAS_DM0                    (* (reg8 *) SDAS__DM0) 
#define SDAS_DM1                    (* (reg8 *) SDAS__DM1)
#define SDAS_DM2                    (* (reg8 *) SDAS__DM2) 
/* Input Buffer Disable Override */
#define SDAS_INP_DIS                (* (reg8 *) SDAS__INP_DIS)
/* LCD Common or Segment Drive */
#define SDAS_LCD_COM_SEG            (* (reg8 *) SDAS__LCD_COM_SEG)
/* Enable Segment LCD */
#define SDAS_LCD_EN                 (* (reg8 *) SDAS__LCD_EN)
/* Slew Rate Control */
#define SDAS_SLW                    (* (reg8 *) SDAS__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SDAS_PRTDSI__CAPS_SEL       (* (reg8 *) SDAS__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SDAS_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SDAS__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SDAS_PRTDSI__OE_SEL0        (* (reg8 *) SDAS__PRTDSI__OE_SEL0) 
#define SDAS_PRTDSI__OE_SEL1        (* (reg8 *) SDAS__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SDAS_PRTDSI__OUT_SEL0       (* (reg8 *) SDAS__PRTDSI__OUT_SEL0) 
#define SDAS_PRTDSI__OUT_SEL1       (* (reg8 *) SDAS__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SDAS_PRTDSI__SYNC_OUT       (* (reg8 *) SDAS__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(SDAS__SIO_CFG)
    #define SDAS_SIO_HYST_EN        (* (reg8 *) SDAS__SIO_HYST_EN)
    #define SDAS_SIO_REG_HIFREQ     (* (reg8 *) SDAS__SIO_REG_HIFREQ)
    #define SDAS_SIO_CFG            (* (reg8 *) SDAS__SIO_CFG)
    #define SDAS_SIO_DIFF           (* (reg8 *) SDAS__SIO_DIFF)
#endif /* (SDAS__SIO_CFG) */

/* Interrupt Registers */
#if defined(SDAS__INTSTAT)
    #define SDAS_INTSTAT            (* (reg8 *) SDAS__INTSTAT)
    #define SDAS_SNAP               (* (reg8 *) SDAS__SNAP)
    
	#define SDAS_0_INTTYPE_REG 		(* (reg8 *) SDAS__0__INTTYPE)
#endif /* (SDAS__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SDAS_H */


/* [] END OF FILE */
