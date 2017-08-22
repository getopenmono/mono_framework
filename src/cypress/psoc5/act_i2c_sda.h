/*******************************************************************************
* File Name: act_i2c_sda.h  
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

#if !defined(CY_PINS_act_i2c_sda_H) /* Pins act_i2c_sda_H */
#define CY_PINS_act_i2c_sda_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "act_i2c_sda_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 act_i2c_sda__PORT == 15 && ((act_i2c_sda__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    act_i2c_sda_Write(uint8 value);
void    act_i2c_sda_SetDriveMode(uint8 mode);
uint8   act_i2c_sda_ReadDataReg(void);
uint8   act_i2c_sda_Read(void);
void    act_i2c_sda_SetInterruptMode(uint16 position, uint16 mode);
uint8   act_i2c_sda_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the act_i2c_sda_SetDriveMode() function.
     *  @{
     */
        #define act_i2c_sda_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define act_i2c_sda_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define act_i2c_sda_DM_RES_UP          PIN_DM_RES_UP
        #define act_i2c_sda_DM_RES_DWN         PIN_DM_RES_DWN
        #define act_i2c_sda_DM_OD_LO           PIN_DM_OD_LO
        #define act_i2c_sda_DM_OD_HI           PIN_DM_OD_HI
        #define act_i2c_sda_DM_STRONG          PIN_DM_STRONG
        #define act_i2c_sda_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define act_i2c_sda_MASK               act_i2c_sda__MASK
#define act_i2c_sda_SHIFT              act_i2c_sda__SHIFT
#define act_i2c_sda_WIDTH              1u

/* Interrupt constants */
#if defined(act_i2c_sda__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in act_i2c_sda_SetInterruptMode() function.
     *  @{
     */
        #define act_i2c_sda_INTR_NONE      (uint16)(0x0000u)
        #define act_i2c_sda_INTR_RISING    (uint16)(0x0001u)
        #define act_i2c_sda_INTR_FALLING   (uint16)(0x0002u)
        #define act_i2c_sda_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define act_i2c_sda_INTR_MASK      (0x01u) 
#endif /* (act_i2c_sda__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define act_i2c_sda_PS                     (* (reg8 *) act_i2c_sda__PS)
/* Data Register */
#define act_i2c_sda_DR                     (* (reg8 *) act_i2c_sda__DR)
/* Port Number */
#define act_i2c_sda_PRT_NUM                (* (reg8 *) act_i2c_sda__PRT) 
/* Connect to Analog Globals */                                                  
#define act_i2c_sda_AG                     (* (reg8 *) act_i2c_sda__AG)                       
/* Analog MUX bux enable */
#define act_i2c_sda_AMUX                   (* (reg8 *) act_i2c_sda__AMUX) 
/* Bidirectional Enable */                                                        
#define act_i2c_sda_BIE                    (* (reg8 *) act_i2c_sda__BIE)
/* Bit-mask for Aliased Register Access */
#define act_i2c_sda_BIT_MASK               (* (reg8 *) act_i2c_sda__BIT_MASK)
/* Bypass Enable */
#define act_i2c_sda_BYP                    (* (reg8 *) act_i2c_sda__BYP)
/* Port wide control signals */                                                   
#define act_i2c_sda_CTL                    (* (reg8 *) act_i2c_sda__CTL)
/* Drive Modes */
#define act_i2c_sda_DM0                    (* (reg8 *) act_i2c_sda__DM0) 
#define act_i2c_sda_DM1                    (* (reg8 *) act_i2c_sda__DM1)
#define act_i2c_sda_DM2                    (* (reg8 *) act_i2c_sda__DM2) 
/* Input Buffer Disable Override */
#define act_i2c_sda_INP_DIS                (* (reg8 *) act_i2c_sda__INP_DIS)
/* LCD Common or Segment Drive */
#define act_i2c_sda_LCD_COM_SEG            (* (reg8 *) act_i2c_sda__LCD_COM_SEG)
/* Enable Segment LCD */
#define act_i2c_sda_LCD_EN                 (* (reg8 *) act_i2c_sda__LCD_EN)
/* Slew Rate Control */
#define act_i2c_sda_SLW                    (* (reg8 *) act_i2c_sda__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define act_i2c_sda_PRTDSI__CAPS_SEL       (* (reg8 *) act_i2c_sda__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define act_i2c_sda_PRTDSI__DBL_SYNC_IN    (* (reg8 *) act_i2c_sda__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define act_i2c_sda_PRTDSI__OE_SEL0        (* (reg8 *) act_i2c_sda__PRTDSI__OE_SEL0) 
#define act_i2c_sda_PRTDSI__OE_SEL1        (* (reg8 *) act_i2c_sda__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define act_i2c_sda_PRTDSI__OUT_SEL0       (* (reg8 *) act_i2c_sda__PRTDSI__OUT_SEL0) 
#define act_i2c_sda_PRTDSI__OUT_SEL1       (* (reg8 *) act_i2c_sda__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define act_i2c_sda_PRTDSI__SYNC_OUT       (* (reg8 *) act_i2c_sda__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(act_i2c_sda__SIO_CFG)
    #define act_i2c_sda_SIO_HYST_EN        (* (reg8 *) act_i2c_sda__SIO_HYST_EN)
    #define act_i2c_sda_SIO_REG_HIFREQ     (* (reg8 *) act_i2c_sda__SIO_REG_HIFREQ)
    #define act_i2c_sda_SIO_CFG            (* (reg8 *) act_i2c_sda__SIO_CFG)
    #define act_i2c_sda_SIO_DIFF           (* (reg8 *) act_i2c_sda__SIO_DIFF)
#endif /* (act_i2c_sda__SIO_CFG) */

/* Interrupt Registers */
#if defined(act_i2c_sda__INTSTAT)
    #define act_i2c_sda_INTSTAT            (* (reg8 *) act_i2c_sda__INTSTAT)
    #define act_i2c_sda_SNAP               (* (reg8 *) act_i2c_sda__SNAP)
    
	#define act_i2c_sda_0_INTTYPE_REG 		(* (reg8 *) act_i2c_sda__0__INTTYPE)
#endif /* (act_i2c_sda__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_act_i2c_sda_H */


/* [] END OF FILE */
