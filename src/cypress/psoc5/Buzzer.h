/*******************************************************************************
* File Name: Buzzer.h  
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

#if !defined(CY_PINS_Buzzer_H) /* Pins Buzzer_H */
#define CY_PINS_Buzzer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Buzzer_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Buzzer__PORT == 15 && ((Buzzer__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Buzzer_Write(uint8 value);
void    Buzzer_SetDriveMode(uint8 mode);
uint8   Buzzer_ReadDataReg(void);
uint8   Buzzer_Read(void);
void    Buzzer_SetInterruptMode(uint16 position, uint16 mode);
uint8   Buzzer_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Buzzer_SetDriveMode() function.
     *  @{
     */
        #define Buzzer_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Buzzer_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Buzzer_DM_RES_UP          PIN_DM_RES_UP
        #define Buzzer_DM_RES_DWN         PIN_DM_RES_DWN
        #define Buzzer_DM_OD_LO           PIN_DM_OD_LO
        #define Buzzer_DM_OD_HI           PIN_DM_OD_HI
        #define Buzzer_DM_STRONG          PIN_DM_STRONG
        #define Buzzer_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Buzzer_MASK               Buzzer__MASK
#define Buzzer_SHIFT              Buzzer__SHIFT
#define Buzzer_WIDTH              1u

/* Interrupt constants */
#if defined(Buzzer__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Buzzer_SetInterruptMode() function.
     *  @{
     */
        #define Buzzer_INTR_NONE      (uint16)(0x0000u)
        #define Buzzer_INTR_RISING    (uint16)(0x0001u)
        #define Buzzer_INTR_FALLING   (uint16)(0x0002u)
        #define Buzzer_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Buzzer_INTR_MASK      (0x01u) 
#endif /* (Buzzer__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Buzzer_PS                     (* (reg8 *) Buzzer__PS)
/* Data Register */
#define Buzzer_DR                     (* (reg8 *) Buzzer__DR)
/* Port Number */
#define Buzzer_PRT_NUM                (* (reg8 *) Buzzer__PRT) 
/* Connect to Analog Globals */                                                  
#define Buzzer_AG                     (* (reg8 *) Buzzer__AG)                       
/* Analog MUX bux enable */
#define Buzzer_AMUX                   (* (reg8 *) Buzzer__AMUX) 
/* Bidirectional Enable */                                                        
#define Buzzer_BIE                    (* (reg8 *) Buzzer__BIE)
/* Bit-mask for Aliased Register Access */
#define Buzzer_BIT_MASK               (* (reg8 *) Buzzer__BIT_MASK)
/* Bypass Enable */
#define Buzzer_BYP                    (* (reg8 *) Buzzer__BYP)
/* Port wide control signals */                                                   
#define Buzzer_CTL                    (* (reg8 *) Buzzer__CTL)
/* Drive Modes */
#define Buzzer_DM0                    (* (reg8 *) Buzzer__DM0) 
#define Buzzer_DM1                    (* (reg8 *) Buzzer__DM1)
#define Buzzer_DM2                    (* (reg8 *) Buzzer__DM2) 
/* Input Buffer Disable Override */
#define Buzzer_INP_DIS                (* (reg8 *) Buzzer__INP_DIS)
/* LCD Common or Segment Drive */
#define Buzzer_LCD_COM_SEG            (* (reg8 *) Buzzer__LCD_COM_SEG)
/* Enable Segment LCD */
#define Buzzer_LCD_EN                 (* (reg8 *) Buzzer__LCD_EN)
/* Slew Rate Control */
#define Buzzer_SLW                    (* (reg8 *) Buzzer__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Buzzer_PRTDSI__CAPS_SEL       (* (reg8 *) Buzzer__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Buzzer_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Buzzer__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Buzzer_PRTDSI__OE_SEL0        (* (reg8 *) Buzzer__PRTDSI__OE_SEL0) 
#define Buzzer_PRTDSI__OE_SEL1        (* (reg8 *) Buzzer__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Buzzer_PRTDSI__OUT_SEL0       (* (reg8 *) Buzzer__PRTDSI__OUT_SEL0) 
#define Buzzer_PRTDSI__OUT_SEL1       (* (reg8 *) Buzzer__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Buzzer_PRTDSI__SYNC_OUT       (* (reg8 *) Buzzer__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Buzzer__SIO_CFG)
    #define Buzzer_SIO_HYST_EN        (* (reg8 *) Buzzer__SIO_HYST_EN)
    #define Buzzer_SIO_REG_HIFREQ     (* (reg8 *) Buzzer__SIO_REG_HIFREQ)
    #define Buzzer_SIO_CFG            (* (reg8 *) Buzzer__SIO_CFG)
    #define Buzzer_SIO_DIFF           (* (reg8 *) Buzzer__SIO_DIFF)
#endif /* (Buzzer__SIO_CFG) */

/* Interrupt Registers */
#if defined(Buzzer__INTSTAT)
    #define Buzzer_INTSTAT            (* (reg8 *) Buzzer__INTSTAT)
    #define Buzzer_SNAP               (* (reg8 *) Buzzer__SNAP)
    
	#define Buzzer_0_INTTYPE_REG 		(* (reg8 *) Buzzer__0__INTTYPE)
#endif /* (Buzzer__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Buzzer_H */


/* [] END OF FILE */
