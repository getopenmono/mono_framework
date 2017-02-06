

#include "mbed_assert.h"
#include "analogin_api.h"
#include "wait_api.h"

#include "cmsis.h"

static analogin_t *lastUsedPin = 0;
static char adcStarted = 0;

static inline uint32_t amuxAddrFromPin(PinName pin)
{
    return (pin & (~0x07)) + 0x10C;
}

static inline uint8_t portBitPattern(PinName pin)
{
    return 1 << (pin & 0x07);
}

static inline char isRAmuxBus(PinName pin)
{
    if (pin < CYDEV_IO_PC_PRT12_BASE)
        return (pin & 0x08) == 0x08;
    else if (pin >= CYDEV_IO_PC_PRT15_BASE) // PRT 15
    {
        if (pin <= CYDEV_IO_PC_PRT15_PC3)
            return 1;
        else
            return 0;
    }
    else if (pin >= CYDEV_IO_PC_PRT12_BASE) // PRT 12
    {
        if ((pin & 0x7) >= 2 && (pin & 0x7) <= 5)
            return 0; // is L mux pin
        else
            return 1; // is R mux pin
    }
    else {
        return 0; // something is wrong here!
    }
}

void analogin_init(analogin_t *obj, PinName pin) {
    obj->hwPin = pin;
    obj->joinAmuxBusses = isRAmuxBus(pin);
    obj->portRegister = amuxAddrFromPin(pin);
    obj->portBitPattern = portBitPattern(pin);

    CyPins_SetPinDriveMode(pin, CY_PINS_DM_ALG_HIZ);

    if (!adcStarted) {
        ADC_SAR_1_Start();
        adcStarted = 1;
    }
}

uint16_t analogin_read_u16(analogin_t *obj) {
    if (lastUsedPin != obj)
    {
        //teardown existing route
        if (lastUsedPin != 0)
            CY_SET_REG8(lastUsedPin->portRegister, 0);

        //setup the new route
        CY_SET_REG8(obj->portRegister, obj->portBitPattern);
        CY_SET_REG8(CYREG_BUS_SW3, obj->joinAmuxBusses);
        lastUsedPin = obj;
        wait_us(20); // wait for signal to settle
    }
        
    ADC_SAR_1_StartConvert();
    ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT);

    return ADC_SAR_1_GetResult16() << 4; // scale up to 16 bit from 12 bit ADC
}

float analogin_read(analogin_t *obj) {
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFFF); // 12 bits range
}
