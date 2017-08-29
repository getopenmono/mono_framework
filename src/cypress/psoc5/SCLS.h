/*******************************************************************************
* File Name: SCLS.h  
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

#if !defined(CY_PINS_SCLS_H) /* Pins SCLS_H */
#define CY_PINS_SCLS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SCLS_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SCLS__PORT == 15 && ((SCLS__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    SCLS_Write(uint8 value);
void    SCLS_SetDriveMode(uint8 mode);
uint8   SCLS_ReadDataReg(void);
uint8   SCLS_Read(void);
void    SCLS_SetInterruptMode(uint16 position, uint16 mode);
uint8   SCLS_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SCLS_SetDriveMode() function.
     *  @{
     */
        #define SCLS_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SCLS_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SCLS_DM_RES_UP          PIN_DM_RES_UP
        #define SCLS_DM_RES_DWN         PIN_DM_RES_DWN
        #define SCLS_DM_OD_LO           PIN_DM_OD_LO
        #define SCLS_DM_OD_HI           PIN_DM_OD_HI
        #define SCLS_DM_STRONG          PIN_DM_STRONG
        #define SCLS_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define SCLS_MASK               SCLS__MASK
#define SCLS_SHIFT              SCLS__SHIFT
#define SCLS_WIDTH              1u

/* Interrupt constants */
#if defined(SCLS__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SCLS_SetInterruptMode() function.
     *  @{
     */
        #define SCLS_INTR_NONE      (uint16)(0x0000u)
        #define SCLS_INTR_RISING    (uint16)(0x0001u)
        #define SCLS_INTR_FALLING   (uint16)(0x0002u)
        #define SCLS_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define SCLS_INTR_MASK      (0x01u) 
#endif /* (SCLS__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SCLS_PS                     (* (reg8 *) SCLS__PS)
/* Data Register */
#define SCLS_DR                     (* (reg8 *) SCLS__DR)
/* Port Number */
#define SCLS_PRT_NUM                (* (reg8 *) SCLS__PRT) 
/* Connect to Analog Globals */                                                  
#define SCLS_AG                     (* (reg8 *) SCLS__AG)                       
/* Analog MUX bux enable */
#define SCLS_AMUX                   (* (reg8 *) SCLS__AMUX) 
/* Bidirectional Enable */                                                        
#define SCLS_BIE                    (* (reg8 *) SCLS__BIE)
/* Bit-mask for Aliased Register Access */
#define SCLS_BIT_MASK               (* (reg8 *) SCLS__BIT_MASK)
/* Bypass Enable */
#define SCLS_BYP                    (* (reg8 *) SCLS__BYP)
/* Port wide control signals */                                                   
#define SCLS_CTL                    (* (reg8 *) SCLS__CTL)
/* Drive Modes */
#define SCLS_DM0                    (* (reg8 *) SCLS__DM0) 
#define SCLS_DM1                    (* (reg8 *) SCLS__DM1)
#define SCLS_DM2                    (* (reg8 *) SCLS__DM2) 
/* Input Buffer Disable Override */
#define SCLS_INP_DIS                (* (reg8 *) SCLS__INP_DIS)
/* LCD Common or Segment Drive */
#define SCLS_LCD_COM_SEG            (* (reg8 *) SCLS__LCD_COM_SEG)
/* Enable Segment LCD */
#define SCLS_LCD_EN                 (* (reg8 *) SCLS__LCD_EN)
/* Slew Rate Control */
#define SCLS_SLW                    (* (reg8 *) SCLS__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SCLS_PRTDSI__CAPS_SEL       (* (reg8 *) SCLS__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SCLS_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SCLS__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SCLS_PRTDSI__OE_SEL0        (* (reg8 *) SCLS__PRTDSI__OE_SEL0) 
#define SCLS_PRTDSI__OE_SEL1        (* (reg8 *) SCLS__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SCLS_PRTDSI__OUT_SEL0       (* (reg8 *) SCLS__PRTDSI__OUT_SEL0) 
#define SCLS_PRTDSI__OUT_SEL1       (* (reg8 *) SCLS__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SCLS_PRTDSI__SYNC_OUT       (* (reg8 *) SCLS__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(SCLS__SIO_CFG)
    #define SCLS_SIO_HYST_EN        (* (reg8 *) SCLS__SIO_HYST_EN)
    #define SCLS_SIO_REG_HIFREQ     (* (reg8 *) SCLS__SIO_REG_HIFREQ)
    #define SCLS_SIO_CFG            (* (reg8 *) SCLS__SIO_CFG)
    #define SCLS_SIO_DIFF           (* (reg8 *) SCLS__SIO_DIFF)
#endif /* (SCLS__SIO_CFG) */

/* Interrupt Registers */
#if defined(SCLS__INTSTAT)
    #define SCLS_INTSTAT            (* (reg8 *) SCLS__INTSTAT)
    #define SCLS_SNAP               (* (reg8 *) SCLS__SNAP)
    
	#define SCLS_0_INTTYPE_REG 		(* (reg8 *) SCLS__0__INTTYPE)
#endif /* (SCLS__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SCLS_H */


/* [] END OF FILE */
