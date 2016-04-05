// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __i2c_power_test__act8600_power_system__
#define __i2c_power_test__act8600_power_system__

#include <power_subsystem_interface.h>
#include <mbed.h>
#include <queue_interrupt.h>

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
     * **Use this class with extreme causion!**
     */
    class ACT8600PowerSystem : public IPowerSubSystem
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
            APCH_2  = 0xA8, /**<  */
            APCH_4  = 0xAA, /**<  */
            OTG     = 0xB0, /**< USB OTG Power control */
            OTG_EXT = 0xB2  /**<  */
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
            FAULT_MASK_CONTROL = 0x02, /**< Select the fault mask bit */
            POWER_OK_STATUS = 0x01 /**< Select the Power-OK status bit */
        };
        
        
        enum OTGBitmask
        {
            ONQ1 = 0x80,    /**< Turn on/off Q1 switch */
            Q1OK = 0x10     /**< Check that Q1 is turned on and OK */
        };
        
        /**
         * @brief Voltage Settings for the Low Noise LDO regulators
         * Use these settings for REG5 - REG8
         */
        enum LDOVoltageSettings
        {
            V0_6 = 0x00,
            V2_5 = 0x31,
            V3_1 = 0x37,
            V3_3 = 0x39
        };
        
        
        enum ACTChargeState
        {
            PRECONDITION = 3,
            FAST_CHARGE = 2,
            END_OF_CHARGE = 1,
            SUSPENDED = 0
        };
        
        mbed::I2C i2c;
        
    protected:
        
        static const char ACT8600I2CAddress = 0x5A;
        static const PinName InterruptPin = (const PinName)CYREG_PRT5_PC2;
        
        QueueInterrupt powerInterrupt;

        /**
         * Read a register from the ACT8600 chip
         * @param regAddr The address of the register
         * @param regData A pointer to the moemory position where the register content is written
         * @return `true` on success, `false` otherwise
         */
        bool readRegister(int8_t regAddr, uint8_t *regData);
        
        /**
         * Write to a register on the ACT8600 chip
         * @param regAddr The address of the register
         * @param regData The new content of the register (data to be written)
         * @return `true` on success, `false` otherwise
         */
        bool writeRegister(int8_t regAddr, uint8_t regData);


        /** enable fault interrupts for REG1 */
        void enableFaultMask(bool enable = true);

        void powerInterruptHandler();
        
    public:
        
        ACT8600PowerSystem();
        
        
        uint8_t SystemStatus();
        ChargeState ChargeStatus();
        bool setUSBOTGPower(bool on);
        bool USBOTGPower();
        
        uint8_t USBOTG();

        bool IsPowerFenced();
        void setPowerFence(bool active);

        void setPowerFencePeripherals(bool off);
        bool PowerFencePeriperals();
        
        void powerOffUnused();
        
        void onSystemPowerOnReset();
        void onSystemEnterSleep();
        void onSystemWakeFromSleep();
    };
    
} }

#endif /* defined(__i2c_power_test__act8600_power_system__) */
