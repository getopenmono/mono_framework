/*******************************************************************************
* File Name: act_i2c_sda.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
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

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 act_i2c_sda__PORT == 15 && ((act_i2c_sda__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    act_i2c_sda_Write(uint8 value) ;
void    act_i2c_sda_SetDriveMode(uint8 mode) ;
uint8   act_i2c_sda_ReadDataReg(void) ;
uint8   act_i2c_sda_Read(void) ;
uint8   act_i2c_sda_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define act_i2c_sda_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define act_i2c_sda_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define act_i2c_sda_DM_RES_UP          PIN_DM_RES_UP
#define act_i2c_sda_DM_RES_DWN         PIN_DM_RES_DWN
#define act_i2c_sda_DM_OD_LO           PIN_DM_OD_LO
#define act_i2c_sda_DM_OD_HI           PIN_DM_OD_HI
#define act_i2c_sda_DM_STRONG          PIN_DM_STRONG
#define act_i2c_sda_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define act_i2c_sda_MASK               act_i2c_sda__MASK
#define act_i2c_sda_SHIFT              act_i2c_sda__SHIFT
#define act_i2c_sda_WIDTH              1u


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


#if defined(act_i2c_sda__INTSTAT)  /* Interrupt Registers */

    #define act_i2c_sda_INTSTAT                (* (reg8 *) act_i2c_sda__INTSTAT)
    #define act_i2c_sda_SNAP                   (* (reg8 *) act_i2c_sda__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_act_i2c_sda_H */


/* [] END OF FILE */
