/*******************************************************************************
* File Name: TFT_CS.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "TFT_CS.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 TFT_CS__PORT == 15 && ((TFT_CS__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: TFT_CS_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void TFT_CS_Write(uint8 value) 
{
    uint8 staticBits = (TFT_CS_DR & (uint8)(~TFT_CS_MASK));
    TFT_CS_DR = staticBits | ((uint8)(value << TFT_CS_SHIFT) & TFT_CS_MASK);
}


/*******************************************************************************
* Function Name: TFT_CS_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  TFT_CS_DM_STRONG     Strong Drive 
*  TFT_CS_DM_OD_HI      Open Drain, Drives High 
*  TFT_CS_DM_OD_LO      Open Drain, Drives Low 
*  TFT_CS_DM_RES_UP     Resistive Pull Up 
*  TFT_CS_DM_RES_DWN    Resistive Pull Down 
*  TFT_CS_DM_RES_UPDWN  Resistive Pull Up/Down 
*  TFT_CS_DM_DIG_HIZ    High Impedance Digital 
*  TFT_CS_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void TFT_CS_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(TFT_CS_0, mode);
}


/*******************************************************************************
* Function Name: TFT_CS_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro TFT_CS_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 TFT_CS_Read(void) 
{
    return (TFT_CS_PS & TFT_CS_MASK) >> TFT_CS_SHIFT;
}


/*******************************************************************************
* Function Name: TFT_CS_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 TFT_CS_ReadDataReg(void) 
{
    return (TFT_CS_DR & TFT_CS_MASK) >> TFT_CS_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(TFT_CS_INTSTAT) 

    /*******************************************************************************
    * Function Name: TFT_CS_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 TFT_CS_ClearInterrupt(void) 
    {
        return (TFT_CS_INTSTAT & TFT_CS_MASK) >> TFT_CS_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
