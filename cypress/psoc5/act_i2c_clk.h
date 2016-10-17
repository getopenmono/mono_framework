/*******************************************************************************
* File Name: act_i2c_clk.h  
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

#if !defined(CY_PINS_act_i2c_clk_H) /* Pins act_i2c_clk_H */
#define CY_PINS_act_i2c_clk_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "act_i2c_clk_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 act_i2c_clk__PORT == 15 && ((act_i2c_clk__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    act_i2c_clk_Write(uint8 value) ;
void    act_i2c_clk_SetDriveMode(uint8 mode) ;
uint8   act_i2c_clk_ReadDataReg(void) ;
uint8   act_i2c_clk_Read(void) ;
uint8   act_i2c_clk_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define act_i2c_clk_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define act_i2c_clk_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define act_i2c_clk_DM_RES_UP          PIN_DM_RES_UP
#define act_i2c_clk_DM_RES_DWN         PIN_DM_RES_DWN
#define act_i2c_clk_DM_OD_LO           PIN_DM_OD_LO
#define act_i2c_clk_DM_OD_HI           PIN_DM_OD_HI
#define act_i2c_clk_DM_STRONG          PIN_DM_STRONG
#define act_i2c_clk_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define act_i2c_clk_MASK               act_i2c_clk__MASK
#define act_i2c_clk_SHIFT              act_i2c_clk__SHIFT
#define act_i2c_clk_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define act_i2c_clk_PS                     (* (reg8 *) act_i2c_clk__PS)
/* Data Register */
#define act_i2c_clk_DR                     (* (reg8 *) act_i2c_clk__DR)
/* Port Number */
#define act_i2c_clk_PRT_NUM                (* (reg8 *) act_i2c_clk__PRT) 
/* Connect to Analog Globals */                                                  
#define act_i2c_clk_AG                     (* (reg8 *) act_i2c_clk__AG)                       
/* Analog MUX bux enable */
#define act_i2c_clk_AMUX                   (* (reg8 *) act_i2c_clk__AMUX) 
/* Bidirectional Enable */                                                        
#define act_i2c_clk_BIE                    (* (reg8 *) act_i2c_clk__BIE)
/* Bit-mask for Aliased Register Access */
#define act_i2c_clk_BIT_MASK               (* (reg8 *) act_i2c_clk__BIT_MASK)
/* Bypass Enable */
#define act_i2c_clk_BYP                    (* (reg8 *) act_i2c_clk__BYP)
/* Port wide control signals */                                                   
#define act_i2c_clk_CTL                    (* (reg8 *) act_i2c_clk__CTL)
/* Drive Modes */
#define act_i2c_clk_DM0                    (* (reg8 *) act_i2c_clk__DM0) 
#define act_i2c_clk_DM1                    (* (reg8 *) act_i2c_clk__DM1)
#define act_i2c_clk_DM2                    (* (reg8 *) act_i2c_clk__DM2) 
/* Input Buffer Disable Override */
#define act_i2c_clk_INP_DIS                (* (reg8 *) act_i2c_clk__INP_DIS)
/* LCD Common or Segment Drive */
#define act_i2c_clk_LCD_COM_SEG            (* (reg8 *) act_i2c_clk__LCD_COM_SEG)
/* Enable Segment LCD */
#define act_i2c_clk_LCD_EN                 (* (reg8 *) act_i2c_clk__LCD_EN)
/* Slew Rate Control */
#define act_i2c_clk_SLW                    (* (reg8 *) act_i2c_clk__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define act_i2c_clk_PRTDSI__CAPS_SEL       (* (reg8 *) act_i2c_clk__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define act_i2c_clk_PRTDSI__DBL_SYNC_IN    (* (reg8 *) act_i2c_clk__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define act_i2c_clk_PRTDSI__OE_SEL0        (* (reg8 *) act_i2c_clk__PRTDSI__OE_SEL0) 
#define act_i2c_clk_PRTDSI__OE_SEL1        (* (reg8 *) act_i2c_clk__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define act_i2c_clk_PRTDSI__OUT_SEL0       (* (reg8 *) act_i2c_clk__PRTDSI__OUT_SEL0) 
#define act_i2c_clk_PRTDSI__OUT_SEL1       (* (reg8 *) act_i2c_clk__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define act_i2c_clk_PRTDSI__SYNC_OUT       (* (reg8 *) act_i2c_clk__PRTDSI__SYNC_OUT) 


#if defined(act_i2c_clk__INTSTAT)  /* Interrupt Registers */

    #define act_i2c_clk_INTSTAT                (* (reg8 *) act_i2c_clk__INTSTAT)
    #define act_i2c_clk_SNAP                   (* (reg8 *) act_i2c_clk__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_act_i2c_clk_H */


/* [] END OF FILE */
