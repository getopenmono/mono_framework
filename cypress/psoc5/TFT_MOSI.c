/*******************************************************************************
* File Name: TFT_MOSI.c  
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
#include "TFT_MOSI.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 TFT_MOSI__PORT == 15 && ((TFT_MOSI__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: TFT_MOSI_Write
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
void TFT_MOSI_Write(uint8 value) 
{
    uint8 staticBits = (TFT_MOSI_DR & (uint8)(~TFT_MOSI_MASK));
    TFT_MOSI_DR = staticBits | ((uint8)(value << TFT_MOSI_SHIFT) & TFT_MOSI_MASK);
}


/*******************************************************************************
* Function Name: TFT_MOSI_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  TFT_MOSI_DM_STRONG     Strong Drive 
*  TFT_MOSI_DM_OD_HI      Open Drain, Drives High 
*  TFT_MOSI_DM_OD_LO      Open Drain, Drives Low 
*  TFT_MOSI_DM_RES_UP     Resistive Pull Up 
*  TFT_MOSI_DM_RES_DWN    Resistive Pull Down 
*  TFT_MOSI_DM_RES_UPDWN  Resistive Pull Up/Down 
*  TFT_MOSI_DM_DIG_HIZ    High Impedance Digital 
*  TFT_MOSI_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void TFT_MOSI_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(TFT_MOSI_0, mode);
}


/*******************************************************************************
* Function Name: TFT_MOSI_Read
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
*  Macro TFT_MOSI_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 TFT_MOSI_Read(void) 
{
    return (TFT_MOSI_PS & TFT_MOSI_MASK) >> TFT_MOSI_SHIFT;
}


/*******************************************************************************
* Function Name: TFT_MOSI_ReadDataReg
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
uint8 TFT_MOSI_ReadDataReg(void) 
{
    return (TFT_MOSI_DR & TFT_MOSI_MASK) >> TFT_MOSI_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(TFT_MOSI_INTSTAT) 

    /*******************************************************************************
    * Function Name: TFT_MOSI_ClearInterrupt
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
    uint8 TFT_MOSI_ClearInterrupt(void) 
    {
        return (TFT_MOSI_INTSTAT & TFT_MOSI_MASK) >> TFT_MOSI_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
