//
//  act8600_power_system.h
//  mono
//
//  Created by Kristoffer Andersen on 08/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef __i2c_power_test__act8600_power_system__
#define __i2c_power_test__act8600_power_system__

#include <power_system_interface.h>
#include <mbed.h>

namespace mono { namespace power {
    
    /**
     * Power system class for the ACT8600 power IC by ActiveSemi, used in mono
     * This class defines the operations and functions the IC support, as well as
     * handle the routines related to power up and power down.
     *
     *  === WARNING ===
     * **This class has methods to raise the supply voltage for the Cypress MCU
     * up to 40V! This will fry the MCU and brick you system!**
     *
     * **Use this class with extreme care!**
     */
    class ACT8600PowerSystem : public IPowerSystem
    {
    public:
        
        /**
         * @brief List of registers in the ACT8600 chip
         * 
         * Active-Semi has chosen of name register with adjacent address with the
         * same name. Register SYS has addresses 0x00 and 0x01. Hare the additional
         * address is denoted SYS_EXT.
         */
        enum RegisterAddresses
        {
            SYS     = 0x00, /**< VSYS Voltage Level, Status, Monitor, Threshold, time */
            SYS_EXT = 0x01, /**< Thermal Interrupt Status */
            REG1    = 0x10, /**< Regulator 1 Output Voltage Selection */
            REG1_EXT= 0x12, /**< Regulator 1 Enable Bit, Phase, Fault Mask, Power-OK */
            REG2    = 0x20, /**<  */
            REG2_EXT= 0x22, /**<  */
            REG3    = 0x30, /**<  */
            REG3_EXT= 0x32, /**<  */
            REG4    = 0x40, /**<  */
            REG4_EXT= 0x41, /**<  */
            REG5    = 0x50, /**< Regulator 5 Output Voltage Selection */
            REG5_EXT= 0x51, /**< Regulator 5 Enable Bit, Discharge Control */
            REG6    = 0x60, /**<  */
            REG6_EXT= 0x61, /**<  */
            REG7    = 0x70, /**<  */
            REG7_EXT= 0x71, /**<  */
            REG8    = 0x80, /**<  */
            REG8_EXT= 0x81, /**<  */
            REG910  = 0x91, /**<  */
            APCH    = 0xA1, /**<  */
            APCH_EXT= 0xA8  /**<  */
        };
        
        /**
         * Bit masks for the step down regulators registers
         * 
         */
        enum RegulatorMasks
        {
            VOLTAGE_SELECTION = 0x3F, /**< Select the output voltage bits */
            ENABLE = 0x80, /**< Select the regulator enable bit */
            PHASE_CONTROL = 0x04, /**< Some regulator has a phase control bit */
            OUTPUT_DISCHARGE_CONTROL = 0x04, /**< Some regulators has a discharge control bit */
            FAULT_MASK_CONTROL = 0x02, /**< Select the fulat mask bit */
            POWER_OK_STATUS = 0x01 /**< Select the Power-OK status bit */
        };
        
        /**
         * @brief Voltage Settings for the Low Noise LDO regulators
         * Use these settings for REG5 - REG8
         */
        enum LDOVoltageSettings
        {
            V0_6 = 0x00,
            V2_5 = 0x31,
            V3_3 = 0x39
        };
        
        
        
    protected:
        
        static const char ACT8600I2CAddress = 0x5A;
        
        mbed::I2C i2c;
        
        /**
         * Read a register from the ACT8600 chip
         * @param regAddr The address of the register
         * @param regData A pointer to the moemory position where the register content is written
         * @return `true` on success, `false` otherwise
         */
        bool readRegister(int8_t regAddr, int8_t *regData);
        
        /**
         * Write to a register on the ACT8600 chip
         * @param regAddr The address of the register
         * @param regData The new content of the register (data to be written)
         * @return `true` on success, `false` otherwise
         */
        bool writeRegister(int8_t regAddr, int8_t regData);
        
    public:
        
        ACT8600PowerSystem();
        
        void onSystemPowerOnReset();
        void onSystemEnterSleep();
        void onSystemWakeFromSleep();
    };
    
} }

#endif /* defined(__i2c_power_test__act8600_power_system__) */
