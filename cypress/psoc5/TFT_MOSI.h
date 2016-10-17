/*******************************************************************************
* File Name: TFT_MOSI.h  
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

#if !defined(CY_PINS_TFT_MOSI_H) /* Pins TFT_MOSI_H */
#define CY_PINS_TFT_MOSI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TFT_MOSI_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TFT_MOSI__PORT == 15 && ((TFT_MOSI__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    TFT_MOSI_Write(uint8 value) ;
void    TFT_MOSI_SetDriveMode(uint8 mode) ;
uint8   TFT_MOSI_ReadDataReg(void) ;
uint8   TFT_MOSI_Read(void) ;
uint8   TFT_MOSI_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define TFT_MOSI_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define TFT_MOSI_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define TFT_MOSI_DM_RES_UP          PIN_DM_RES_UP
#define TFT_MOSI_DM_RES_DWN         PIN_DM_RES_DWN
#define TFT_MOSI_DM_OD_LO           PIN_DM_OD_LO
#define TFT_MOSI_DM_OD_HI           PIN_DM_OD_HI
#define TFT_MOSI_DM_STRONG          PIN_DM_STRONG
#define TFT_MOSI_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define TFT_MOSI_MASK               TFT_MOSI__MASK
#define TFT_MOSI_SHIFT              TFT_MOSI__SHIFT
#define TFT_MOSI_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TFT_MOSI_PS                     (* (reg8 *) TFT_MOSI__PS)
/* Data Register */
#define TFT_MOSI_DR                     (* (reg8 *) TFT_MOSI__DR)
/* Port Number */
#define TFT_MOSI_PRT_NUM                (* (reg8 *) TFT_MOSI__PRT) 
/* Connect to Analog Globals */                                                  
#define TFT_MOSI_AG                     (* (reg8 *) TFT_MOSI__AG)                       
/* Analog MUX bux enable */
#define TFT_MOSI_AMUX                   (* (reg8 *) TFT_MOSI__AMUX) 
/* Bidirectional Enable */                                                        
#define TFT_MOSI_BIE                    (* (reg8 *) TFT_MOSI__BIE)
/* Bit-mask for Aliased Register Access */
#define TFT_MOSI_BIT_MASK               (* (reg8 *) TFT_MOSI__BIT_MASK)
/* Bypass Enable */
#define TFT_MOSI_BYP                    (* (reg8 *) TFT_MOSI__BYP)
/* Port wide control signals */                                                   
#define TFT_MOSI_CTL                    (* (reg8 *) TFT_MOSI__CTL)
/* Drive Modes */
#define TFT_MOSI_DM0                    (* (reg8 *) TFT_MOSI__DM0) 
#define TFT_MOSI_DM1                    (* (reg8 *) TFT_MOSI__DM1)
#define TFT_MOSI_DM2                    (* (reg8 *) TFT_MOSI__DM2) 
/* Input Buffer Disable Override */
#define TFT_MOSI_INP_DIS                (* (reg8 *) TFT_MOSI__INP_DIS)
/* LCD Common or Segment Drive */
#define TFT_MOSI_LCD_COM_SEG            (* (reg8 *) TFT_MOSI__LCD_COM_SEG)
/* Enable Segment LCD */
#define TFT_MOSI_LCD_EN                 (* (reg8 *) TFT_MOSI__LCD_EN)
/* Slew Rate Control */
#define TFT_MOSI_SLW                    (* (reg8 *) TFT_MOSI__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TFT_MOSI_PRTDSI__CAPS_SEL       (* (reg8 *) TFT_MOSI__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TFT_MOSI_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TFT_MOSI__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TFT_MOSI_PRTDSI__OE_SEL0        (* (reg8 *) TFT_MOSI__PRTDSI__OE_SEL0) 
#define TFT_MOSI_PRTDSI__OE_SEL1        (* (reg8 *) TFT_MOSI__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TFT_MOSI_PRTDSI__OUT_SEL0       (* (reg8 *) TFT_MOSI__PRTDSI__OUT_SEL0) 
#define TFT_MOSI_PRTDSI__OUT_SEL1       (* (reg8 *) TFT_MOSI__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TFT_MOSI_PRTDSI__SYNC_OUT       (* (reg8 *) TFT_MOSI__PRTDSI__SYNC_OUT) 


#if defined(TFT_MOSI__INTSTAT)  /* Interrupt Registers */

    #define TFT_MOSI_INTSTAT                (* (reg8 *) TFT_MOSI__INTSTAT)
    #define TFT_MOSI_SNAP                   (* (reg8 *) TFT_MOSI__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TFT_MOSI_H */


/* [] END OF FILE */
