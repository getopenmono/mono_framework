/*******************************************************************************
* File Name: Buzzer.c  
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
#include "Buzzer.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Buzzer__PORT == 15 && ((Buzzer__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Buzzer_Write
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
void Buzzer_Write(uint8 value) 
{
    uint8 staticBits = (Buzzer_DR & (uint8)(~Buzzer_MASK));
    Buzzer_DR = staticBits | ((uint8)(value << Buzzer_SHIFT) & Buzzer_MASK);
}


/*******************************************************************************
* Function Name: Buzzer_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Buzzer_DM_STRONG     Strong Drive 
*  Buzzer_DM_OD_HI      Open Drain, Drives High 
*  Buzzer_DM_OD_LO      Open Drain, Drives Low 
*  Buzzer_DM_RES_UP     Resistive Pull Up 
*  Buzzer_DM_RES_DWN    Resistive Pull Down 
*  Buzzer_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Buzzer_DM_DIG_HIZ    High Impedance Digital 
*  Buzzer_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Buzzer_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Buzzer_0, mode);
}


/*******************************************************************************
* Function Name: Buzzer_Read
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
*  Macro Buzzer_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Buzzer_Read(void) 
{
    return (Buzzer_PS & Buzzer_MASK) >> Buzzer_SHIFT;
}


/*******************************************************************************
* Function Name: Buzzer_ReadDataReg
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
uint8 Buzzer_ReadDataReg(void) 
{
    return (Buzzer_DR & Buzzer_MASK) >> Buzzer_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Buzzer_INTSTAT) 

    /*******************************************************************************
    * Function Name: Buzzer_ClearInterrupt
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
    uint8 Buzzer_ClearInterrupt(void) 
    {
        return (Buzzer_INTSTAT & Buzzer_MASK) >> Buzzer_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
