// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef spiTest_PinNames_h
#define spiTest_PinNames_h


#ifdef __cplusplus
extern "C" {
#endif
    
#include <project.h>
    
#ifdef __cplusplus
}
#endif


typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    HARD_WIRED,
    
    // MARK: Auxillary pins
    SW_USER = CYREG_PRT5_PC4,               /**< User button pin, active low */
    USER_SW = SW_USER,                      /**< User button pin, active low */
    BUZZER = CYREG_PRT5_PC3,                /**< Buzzer PWM signal pin */
    BZZR = BUZZER,                          /**< Buzzer PWM signal pin */
    EXPANSION_PWR_ENABLE = CYREG_PRT5_PC6,  /**< Enable the power for expansion connector (5V default)  */
    VAUX_EN = EXPANSION_PWR_ENABLE,         /**< Enable the power for expansion connector (5V default) */
    EXPANSION_3V3_ENABLE = CYREG_PRT5_PC5,  /**< Enable 3.3V on HIGH , LOW is 5V. */
    VAUX_SEL = EXPANSION_3V3_ENABLE,        /**< Enable 3.3V on HIGH , LOW is 5V. */
    TFT_RP_nRESET = CYREG_PRT2_PC0,         /**< Reset both TFT and wireless module. Active low. */

    // MARK: Redpine wireless pins
    RP_SPI_HARD_WIRE,               /**< Enum for the wireless module SPI */
    RP_SPI_CLK = RP_SPI_HARD_WIRE,              
    RP_SPI_MOSI = RP_SPI_HARD_WIRE,
    RP_SPI_MISO = RP_SPI_HARD_WIRE,
    RP_SPI_CS = RP_SPI_HARD_WIRE,   /**< Chip select for wireless module SPI, active low */
    RP_nRESET = HARD_WIRED,         /**< Reset line for wireless module, (open-drain) active low */
    RP_INTERRUPT = CYREG_PRT2_PC4,  /**< Wireless module interrupt on data, input! */
    RP_INT = RP_INTERRUPT,          /**< Wireless module interrupt on data, input! */
    
    // MARK: SD Card SPI pins
    SD_SPI_HARD_WIRE,               /**< Enum for SD card SPI pins */
    SD_SPI_CLK = SD_SPI_HARD_WIRE,
    SD_SPI_MOSI = SD_SPI_HARD_WIRE,
    SD_SPI_MISO = SD_SPI_HARD_WIRE,
    SD_SPI_CS = SD_SPI_HARD_WIRE,   /**< SD Card chip select, hard wired in firmware. */

    // MARK: Display SPI pins
    TFT_SPI_HARD_WIRE,
    TFT_SPI_CLK = TFT_SPI_HARD_WIRE,
    TFT_SPI_MOSI = TFT_SPI_HARD_WIRE,
    TFT_SPI_MISO = TFT_SPI_HARD_WIRE,
    TFT_SPI_CS = TFT_SPI_HARD_WIRE,
    
    // MARK: Display pins
    TFT_LED_PWR = CYREG_PRT5_PC1,           /**< TFT LED backlight PWM pin */
    TFT_REGISTER_SELECT = CYREG_PRT2_PC3,   /**< TFT cmd/data select pin. Only for 8-bit SPI mode. */
    TFT_RESET = CYREG_PRT2_PC0,             /**< Active low reset signal for TFT. Shared with wifi module */
    TFT_nRESET = TFT_RESET,                 /**< Active low reset signal for TFT. Shared with wifi module */
    TFT_TEARING_EFFECT = CYREG_IO_PC_PRT15_PC5, /**< TFT tearing effect / vsync interrupt */
    TFT_IM0 = HARD_WIRED,           /**< TFT interface mode select pin. Hard wired in PCBv3 */
    TFT_TOUCH_Y1 = CYREG_PRT1_PC7,  /**< TFT touch input Y1 */
    TFT_TOUCH_X1 = CYREG_PRT1_PC6,  /**< TFT touch input X1 */
    TFT_TOUCH_Y2 = CYREG_PRT1_PC5,  /**< TFT touch input Y2 */
    TFT_TOUCH_X2 = CYREG_PRT1_PC2,  /**< TFT touch input X2 */
    TFT_LED = TFT_LED_PWR,          /**< TFT LED backlight PWM pin */

    // MARK: Public touch input pins
    T_Xn = TFT_TOUCH_X1,   /**< Display touch input X- */
    T_Xp = TFT_TOUCH_X2,   /**< Display touch input X+ */
    T_Yn = TFT_TOUCH_Y1,   /**< Display touch input Y- */
    T_Yp = TFT_TOUCH_Y2,   /**< Display touch input Y+ */

    // MARK: Accelerometer pins
    ACCL_INT1 = CYREG_PRT12_PC0,/**< The interrupt signal from the acceleromter */

    // MARK: Mini jack connector and arduino pins
    J_TIP   = CYREG_PRT12_PC3,  /**< Pin is shared between JC_TIP and @ref D9 */
    J_RING1 = CYREG_PRT0_PC2,   /**< Pin is shared between JC_RING1 and @ref D10 */
    J_RING2 = CYREG_PRT0_PC1,   /**< Pin is shared between JC_RING2 and @ref D11 */
    JPI_nEN = CYREG_PRT12_PC2,  /**< Set `LOW` to charge battery from jack connector @ref J_TIP pin, through VBUS */
    JPO_nEN = CYREG_PRT12_PC1,  /**< Set `LOW` to connect VAUX to @ref J_TIP */
    JPD_EN  = CYREG_PRT0_PC0,   /**< Set `HIGH` to *short circuit* @ref J_RING1 to `GND` */

    SENSE = CYREG_PRT0_PC4,     /**< Not used. Modify PCB to use as current sense input */

    nIRQ = CYREG_PRT5_PC2,      /**< The active low interrupt signal from the power sub system */

    // MARK: Arduino Digital pins
    D0 = CYREG_PRT4_PC0,    /**< D0 on Shield Adaptor */
    D1 = CYREG_PRT4_PC1,    /**< D1 on Shield Adaptor */
    D2 = CYREG_PRT4_PC2,    /**< D2 on Shield Adaptor */
    D3 = CYREG_PRT4_PC3,    /**< D3 on Shield Adaptor */
    D4 = CYREG_PRT4_PC4,    /**< D4 on Shield Adaptor */
    D5 = CYREG_PRT4_PC5,    /**< D5 on Shield Adaptor */
    D6 = CYREG_PRT4_PC6,    /**< D6 on Shield Adaptor */
    D7 = CYREG_PRT4_PC7,    /**< D7 on Shield Adaptor */
    
    D8 = CYREG_PRT0_PC3,    /**< D8 on Shield Adaptor */

    D9 = CYREG_PRT12_PC3,   /**< Shared between D9 on Shield Adaptor and @ref JC_TIP */

    D10 = CYREG_PRT0_PC2,   /**< Shared between D10 on Shield Adaptor and @ref JC_RING1 */
    D11 = CYREG_PRT0_PC1,   /**< Shared between D11 on Shield Adaptor and @ref JC_RING2 */
    D12 = CYREG_PRT0_PC5,   /**< D12 on Shield Adaptor */
    D13 = CYREG_PRT0_PC6,   /**< D13 on Shield Adaptor */
    
    // MARK: Arduino Analog pins
    A0 = CYREG_PRT3_PC2,    /**< A0 on Shield Adaptor */
    A1 = CYREG_PRT3_PC1,    /**< A1 on Shield Adaptor */
    A2 = CYREG_PRT3_PC0,    /**< A2 on Shield Adaptor */
    A3 = CYREG_PRT3_PC3,    /**< A3 on Shield Adaptor */
    A4 = CYREG_PRT3_PC4,    /**< A4 on Shield Adaptor */
    A5 = CYREG_PRT3_PC5,    /**< A5 on Shield Adaptor */

    // MARK: USB Pseudo pins
    USBTX,
    USBRX,
    STDIO_UART_TX = USBTX,
    STDIO_UART_RX = USBRX,
    
    NC = -1
} PinName;

typedef enum {
    PullUp = 0,
    PullDown = 3,
    PullNone = 2,
    PullBoth = 5,
    Repeater = 1,
    OpenDrain = 4,
    PullDefault = PullDown
} PinMode;

#endif
