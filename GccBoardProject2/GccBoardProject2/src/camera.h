/*
 * camera.h
 *
 * Created: 2/20/2024 6:05:30 PM
 *  Author: kcons
 */ 

#ifndef CAMERA_H
#define CAMERA_H

#include <asf.h>
#include <string.h>

volatile uint8_t g_p_uc_cap_dest_buf[BUFF_SIZE]; // buffer to hold images
static uint8_t find_image_length(void);
static uint8_t start_capture(void); 

#define OV2640_VSYNC_PIO				OV_VSYNC_PIO
#define OV2640_VSYNC_ID					OV_VSYNC_ID
#define OV2640_VSYNC_MASK              OV_VSYNC_MASK
#define OV2640_VSYNC_TYPE              OV_VSYNC_TYPE

#define OV_VSYNC_GPIO                  PIO_PA15_IDX
#define OV_VSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_VSYNC_MASK                  PIO_PA15 // have to change this pin to the real value
#define OV_VSYNC_PIO                   PIOA
#define OV_VSYNC_ID                    ID_PIOA
#define OV_VSYNC_TYPE                  PIO_PULLUP

#define OV_POWER_PIO                   PIOC // suspicious
#define OV_POWER_MASK                  PIO_PC10 // this probably isnt PIOC

/** OV Data Bus pins */
#define OV_DATA_BUS_D0                 PIO_PA24_IDX //changed these to D0-D7, could be a problem in the future. change pin definitions
#define OV_DATA_BUS_D1                 PIO_PA25_IDX
#define OV_DATA_BUS_D2                 PIO_PA26_IDX
#define OV_DATA_BUS_D3                 PIO_PA27_IDX
#define OV_DATA_BUS_D4                 PIO_PA28_IDX
#define OV_DATA_BUS_D5                 PIO_PA29_IDX
#define OV_DATA_BUS_D6                 PIO_PA30_IDX
#define OV_DATA_BUS_D7                 PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV2640_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT

#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 clock pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA,	PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 Data pins definition */
#define TWI0_DATA_GPIO                 PIO_PA3_IDX
#define TWI0_DATA_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_DATA_MASK                 PIO_PA3
#define TWI0_DATA_PIO                  PIOA
#define TWI0_DATA_ID                   ID_PIOA
#define TWI0_DATA_TYPE                 PIO_PERIPH_A
#define TWI0_DATA_ATTR                 PIO_DEFAULT

/** TWI0 clock pins definition */
#define TWI0_CLK_GPIO                  PIO_PA4_IDX
#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_CLK_MASK                  PIO_PA4
#define TWI0_CLK_PIO                   PIOA
#define TWI0_CLK_ID                    ID_PIOA
#define TWI0_CLK_TYPE                  PIO_PERIPH_A
#define TWI0_CLK_ATTR                  PIO_DEFAULT

/** TWI0 pins */
#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0

#define ID_BOARD_TWI		             ID_TWI0
#define BOARD_TWI			             TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn

#define TWI_CLK     (400000UL)

#endif