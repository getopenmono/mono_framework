//
//  PinNames.h
//  spiTest
//
//  Created by Kristoffer Andersen on 26/05/15.
//
//

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
    
    // Auxillary pins
    SW_USER = CYREG_PRT4_PC6,                   /**< User button pin, active low */
    BUZZER = CYREG_PRT5_PC3,                    /**< Buzzer PWM signal pin */
    EXPANSION_5V_nENABLE = CYREG_PRT12_PC0,     /**< Enable 5V level on exp. cnnctr. I/O, active low */
    
    //Redpine wireless pins
    RP_SPI_HARD_WIRE,                           /**< Enum for the wireless module SPI */
    RP_SPI_CLK = RP_SPI_HARD_WIRE,              
    RP_SPI_MOSI = RP_SPI_HARD_WIRE,
    RP_SPI_MISO = RP_SPI_HARD_WIRE,
    RP_SPI_CS = CYREG_PRT2_PC6,                 /**< Chip select for wireless module SPI, active low */
    RP_nRESET = CYREG_PRT5_PC0,                 /**< Reset line for wireless module, active low */
    RP_INTERRUPT = CYREG_PRT2_PC5,              /**< Wireless module interrupt on data, input! */
    
    //SD Card SPI pins
    SD_SPI_HARD_WIRE = RP_SPI_HARD_WIRE,        /**< Enum for SD card SPI pins */
    SD_SPI_CLK = SD_SPI_HARD_WIRE,
    SD_SPI_MOSI = SD_SPI_HARD_WIRE,
    SD_SPI_MISO = SD_SPI_HARD_WIRE,
    SD_SPI_CS = CYREG_PRT2_PC3,                 /**< SD Card chip select */
    SD_INSERT_SW = CYREG_PRT5_PC1,              /**<  */
    
    //Display SPI pins
    TFT_SPI_HARD_WIRE,
    TFT_SPI_CLK = TFT_SPI_HARD_WIRE,
    TFT_SPI_MOSI = TFT_SPI_HARD_WIRE,
    TFT_SPI_MISO = TFT_SPI_HARD_WIRE,
    TFT_SPI_CS = TFT_SPI_HARD_WIRE,
    
    //Display pins
    TFT_LED_PWR = CYREG_PRT5_PC5,
    TFT_REGISTER_SELECT = CYREG_PRT12_PC1,
//    #warning "TFT_RESET SIGNAL REROUTED!!"
//    TFT_RESET = CYREG_PRT3_PC1,
    TFT_RESET = CYREG_PRT4_PC4,
    TFT_TEARING_EFFECT = CYREG_PRT3_PC3,
    TFT_IM0 = CYREG_PRT2_PC0,
    TFT_TOUCH_Y1 = CYREG_PRT6_PC0,
    TFT_TOUCH_X1 = CYREG_PRT6_PC1,
    TFT_TOUCH_Y2 = CYREG_PRT6_PC2,
    TFT_TOUCH_X2 = CYREG_PRT6_PC3,
    
    //Mini jack connector
#ifdef MONO_PCB_V2
    JC_TIP =   CYREG_PRT12_PC3,
    JC_RING1 = CYREG_PRT0_PC2,
    JC_RING2 = CYREG_PRT0_PC1,
#else
    JC_TIP =   CYREG_PRT0_PC1,
    JC_RING1 = CYREG_PRT0_PC2,
    JC_RING2 = CYREG_PRT12_PC3,
#endif
    
    USBTX,
    USBRX,
    
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
