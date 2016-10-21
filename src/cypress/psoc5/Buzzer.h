/*******************************************************************************
* File Name: Buzzer.h  
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

#if !defined(CY_PINS_Buzzer_H) /* Pins Buzzer_H */
#define CY_PINS_Buzzer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Buzzer_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Buzzer__PORT == 15 && ((Buzzer__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Buzzer_Write(uint8 value) ;
void    Buzzer_SetDriveMode(uint8 mode) ;
uint8   Buzzer_ReadDataReg(void) ;
uint8   Buzzer_Read(void) ;
uint8   Buzzer_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Buzzer_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Buzzer_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Buzzer_DM_RES_UP          PIN_DM_RES_UP
#define Buzzer_DM_RES_DWN         PIN_DM_RES_DWN
#define Buzzer_DM_OD_LO           PIN_DM_OD_LO
#define Buzzer_DM_OD_HI           PIN_DM_OD_HI
#define Buzzer_DM_STRONG          PIN_DM_STRONG
#define Buzzer_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Buzzer_MASK               Buzzer__MASK
#define Buzzer_SHIFT              Buzzer__SHIFT
#define Buzzer_WIDTH              1u


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


#if defined(Buzzer__INTSTAT)  /* Interrupt Registers */

    #define Buzzer_INTSTAT                (* (reg8 *) Buzzer__INTSTAT)
    #define Buzzer_SNAP                   (* (reg8 *) Buzzer__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Buzzer_H */


/* [] END OF FILE */
