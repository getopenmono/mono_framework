/*******************************************************************************
* File Name: I2C_SDA.h  
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

#if !defined(CY_PINS_I2C_SDA_H) /* Pins I2C_SDA_H */
#define CY_PINS_I2C_SDA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "I2C_SDA_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 I2C_SDA__PORT == 15 && ((I2C_SDA__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    I2C_SDA_Write(uint8 value) ;
void    I2C_SDA_SetDriveMode(uint8 mode) ;
uint8   I2C_SDA_ReadDataReg(void) ;
uint8   I2C_SDA_Read(void) ;
uint8   I2C_SDA_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define I2C_SDA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define I2C_SDA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define I2C_SDA_DM_RES_UP          PIN_DM_RES_UP
#define I2C_SDA_DM_RES_DWN         PIN_DM_RES_DWN
#define I2C_SDA_DM_OD_LO           PIN_DM_OD_LO
#define I2C_SDA_DM_OD_HI           PIN_DM_OD_HI
#define I2C_SDA_DM_STRONG          PIN_DM_STRONG
#define I2C_SDA_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define I2C_SDA_MASK               I2C_SDA__MASK
#define I2C_SDA_SHIFT              I2C_SDA__SHIFT
#define I2C_SDA_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define I2C_SDA_PS                     (* (reg8 *) I2C_SDA__PS)
/* Data Register */
#define I2C_SDA_DR                     (* (reg8 *) I2C_SDA__DR)
/* Port Number */
#define I2C_SDA_PRT_NUM                (* (reg8 *) I2C_SDA__PRT) 
/* Connect to Analog Globals */                                                  
#define I2C_SDA_AG                     (* (reg8 *) I2C_SDA__AG)                       
/* Analog MUX bux enable */
#define I2C_SDA_AMUX                   (* (reg8 *) I2C_SDA__AMUX) 
/* Bidirectional Enable */                                                        
#define I2C_SDA_BIE                    (* (reg8 *) I2C_SDA__BIE)
/* Bit-mask for Aliased Register Access */
#define I2C_SDA_BIT_MASK               (* (reg8 *) I2C_SDA__BIT_MASK)
/* Bypass Enable */
#define I2C_SDA_BYP                    (* (reg8 *) I2C_SDA__BYP)
/* Port wide control signals */                                                   
#define I2C_SDA_CTL                    (* (reg8 *) I2C_SDA__CTL)
/* Drive Modes */
#define I2C_SDA_DM0                    (* (reg8 *) I2C_SDA__DM0) 
#define I2C_SDA_DM1                    (* (reg8 *) I2C_SDA__DM1)
#define I2C_SDA_DM2                    (* (reg8 *) I2C_SDA__DM2) 
/* Input Buffer Disable Override */
#define I2C_SDA_INP_DIS                (* (reg8 *) I2C_SDA__INP_DIS)
/* LCD Common or Segment Drive */
#define I2C_SDA_LCD_COM_SEG            (* (reg8 *) I2C_SDA__LCD_COM_SEG)
/* Enable Segment LCD */
#define I2C_SDA_LCD_EN                 (* (reg8 *) I2C_SDA__LCD_EN)
/* Slew Rate Control */
#define I2C_SDA_SLW                    (* (reg8 *) I2C_SDA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define I2C_SDA_PRTDSI__CAPS_SEL       (* (reg8 *) I2C_SDA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define I2C_SDA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) I2C_SDA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define I2C_SDA_PRTDSI__OE_SEL0        (* (reg8 *) I2C_SDA__PRTDSI__OE_SEL0) 
#define I2C_SDA_PRTDSI__OE_SEL1        (* (reg8 *) I2C_SDA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define I2C_SDA_PRTDSI__OUT_SEL0       (* (reg8 *) I2C_SDA__PRTDSI__OUT_SEL0) 
#define I2C_SDA_PRTDSI__OUT_SEL1       (* (reg8 *) I2C_SDA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define I2C_SDA_PRTDSI__SYNC_OUT       (* (reg8 *) I2C_SDA__PRTDSI__SYNC_OUT) 


#if defined(I2C_SDA__INTSTAT)  /* Interrupt Registers */

    #define I2C_SDA_INTSTAT                (* (reg8 *) I2C_SDA__INTSTAT)
    #define I2C_SDA_SNAP                   (* (reg8 *) I2C_SDA__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_I2C_SDA_H */


/* [] END OF FILE */
