/*******************************************************************************
* File Name: R_MOSI.c  
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
#include "R_MOSI.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 R_MOSI__PORT == 15 && ((R_MOSI__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: R_MOSI_Write
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
void R_MOSI_Write(uint8 value) 
{
    uint8 staticBits = (R_MOSI_DR & (uint8)(~R_MOSI_MASK));
    R_MOSI_DR = staticBits | ((uint8)(value << R_MOSI_SHIFT) & R_MOSI_MASK);
}


/*******************************************************************************
* Function Name: R_MOSI_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  R_MOSI_DM_STRONG     Strong Drive 
*  R_MOSI_DM_OD_HI      Open Drain, Drives High 
*  R_MOSI_DM_OD_LO      Open Drain, Drives Low 
*  R_MOSI_DM_RES_UP     Resistive Pull Up 
*  R_MOSI_DM_RES_DWN    Resistive Pull Down 
*  R_MOSI_DM_RES_UPDWN  Resistive Pull Up/Down 
*  R_MOSI_DM_DIG_HIZ    High Impedance Digital 
*  R_MOSI_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void R_MOSI_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(R_MOSI_0, mode);
}


/*******************************************************************************
* Function Name: R_MOSI_Read
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
*  Macro R_MOSI_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 R_MOSI_Read(void) 
{
    return (R_MOSI_PS & R_MOSI_MASK) >> R_MOSI_SHIFT;
}


/*******************************************************************************
* Function Name: R_MOSI_ReadDataReg
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
uint8 R_MOSI_ReadDataReg(void) 
{
    return (R_MOSI_DR & R_MOSI_MASK) >> R_MOSI_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(R_MOSI_INTSTAT) 

    /*******************************************************************************
    * Function Name: R_MOSI_ClearInterrupt
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
    uint8 R_MOSI_ClearInterrupt(void) 
    {
        return (R_MOSI_INTSTAT & R_MOSI_MASK) >> R_MOSI_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
