/*
 * wifi.h
 *
 * Created: 2/20/2024 6:04:46 PM
 *  Author: kcons
 */ 


#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>

#define WIFI_USART					USART0
#define WIFI_USART_ID				ID_USART0
#define WIFI_USART_BAUDRATE			115200
#define WIFI_USART_HANDLER			USART0_Handler
#define WIFI_USART_IRQn				USART0_IRQn
#define WIFI_USART_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define WIFI_USART_PARITY			US_MR_PAR_NO
#define WIFI_USART_STOP_BITS		US_MR_NBSTOP_1_BIT
#define WIFI_USART_MODE				US_MR_CHMODE_NORMAL

/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

/** All interrupt mask. */
#define ALL_INTERRUPT_MASK  0xffffffff

#define WIFI_COMM_PIN_NUM			PIO_PB10
#define WIFI_COMM_PIO				PIOB
#define WIFI_COMM_ID				ID_PIOB
#define WIFI_COMM_ATTR				PIO_IT_RISE_EDGE

#define WIFI_PROV_PIN_NUM			PIO_PB2
#define WIFI_PROV_PIO				PIOB
#define WIFI_PROV_ID				ID_PIOB
#define WIFI_PROV_ATTR				PIO_IT_RISE_EDGE

#define MAX_INPUT_WIFI 1000
volatile char input_line_wifi[MAX_INPUT_WIFI];
volatile uint32_t received_byte_wifi;
volatile bool new_rx_wifi;
volatile unsigned int input_pos_wifi;
volatile bool wifi_comm_success;

void configure_usart_wifi(void);
void process_incoming_byte_wifi(uint8_t inByte);
void process_data_wifi(void);
void configure_wifi_comm_pin(void);

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO         (PIO_PA12_IDX)
#define SPI_MISO_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO         (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO         (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)

/** SPI chip select 0 pin definition. (Only one configuration is possible) */
#define SPI_NPCS0_GPIO         (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

/* Chip select. */
#define SPI_CHIP_SEL 0
#define SPI_CHIP_PCS spi_get_pcs(SPI_CHIP_SEL)

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 0//1

/* Delay before SPCK. */
#define SPI_DLYBS 0x40

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x10

volatile uint32_t transfer_index;
volatile uint32_t transfer_len;

void spi_prep_transfer(void);
void spi_peripheral_initialize(void);
void configure_spi();

#endif /* WIFI_H_ */