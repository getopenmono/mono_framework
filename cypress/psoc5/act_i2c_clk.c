/*******************************************************************************
* File Name: act_i2c_clk.c  
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
#include "act_i2c_clk.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 act_i2c_clk__PORT == 15 && ((act_i2c_clk__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: act_i2c_clk_Write
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
void act_i2c_clk_Write(uint8 value) 
{
    uint8 staticBits = (act_i2c_clk_DR & (uint8)(~act_i2c_clk_MASK));
    act_i2c_clk_DR = staticBits | ((uint8)(value << act_i2c_clk_SHIFT) & act_i2c_clk_MASK);
}


/*******************************************************************************
* Function Name: act_i2c_clk_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  act_i2c_clk_DM_STRONG     Strong Drive 
*  act_i2c_clk_DM_OD_HI      Open Drain, Drives High 
*  act_i2c_clk_DM_OD_LO      Open Drain, Drives Low 
*  act_i2c_clk_DM_RES_UP     Resistive Pull Up 
*  act_i2c_clk_DM_RES_DWN    Resistive Pull Down 
*  act_i2c_clk_DM_RES_UPDWN  Resistive Pull Up/Down 
*  act_i2c_clk_DM_DIG_HIZ    High Impedance Digital 
*  act_i2c_clk_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void act_i2c_clk_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(act_i2c_clk_0, mode);
}


/*******************************************************************************
* Function Name: act_i2c_clk_Read
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
*  Macro act_i2c_clk_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 act_i2c_clk_Read(void) 
{
    return (act_i2c_clk_PS & act_i2c_clk_MASK) >> act_i2c_clk_SHIFT;
}


/*******************************************************************************
* Function Name: act_i2c_clk_ReadDataReg
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
uint8 act_i2c_clk_ReadDataReg(void) 
{
    return (act_i2c_clk_DR & act_i2c_clk_MASK) >> act_i2c_clk_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(act_i2c_clk_INTSTAT) 

    /*******************************************************************************
    * Function Name: act_i2c_clk_ClearInterrupt
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
    uint8 act_i2c_clk_ClearInterrupt(void) 
    {
        return (act_i2c_clk_INTSTAT & act_i2c_clk_MASK) >> act_i2c_clk_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
