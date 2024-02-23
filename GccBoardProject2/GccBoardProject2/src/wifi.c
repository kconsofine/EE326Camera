/*
 * wifi.c
 *
 * Created: 2/20/2024 6:04:35 PM
 *  Author: kcons
 */ 


#include "wifi.h"
#include "camera.h"

volatile uint32_t received_byte_wifi = 0;
volatile bool new_rx_wifi = false;
volatile unsigned int input_pos_wifi = 0;
volatile bool wifi_comm_success = false;
volatile uint32_t transfer_index = 0;
volatile uint32_t transfer_len = 0;
volatile bool wifi_prov_mode = false;


void WIFI_USART_HANDLER(void)
{
	uint32_t ul_status;

	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(WIFI_USART, &received_byte_wifi);
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}

static void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	wifi_comm_success = true;
	process_data_wifi();
	for (int jj=0;jj<MAX_INPUT_WIFI;jj++) input_line_wifi[jj] = 0;
	input_pos_wifi = 0;
}

void process_incoming_byte_wifi(uint8_t in_byte)
{
	input_line_wifi[input_pos_wifi++] = in_byte;
}

void process_data_wifi() // probably need to screw with this some more
{
	if (strstr(input_line_wifi, "SUCCESS")) {
		ioport_toggle_pin_level(LED_PIN);
	}
}

void wifi_provision_handler(uint32_t ul_id, uint32_t ul_mask)
{
	// to do
	unused(ul_id);
	unused(ul_mask);
	wifi_prov_mode = true;
}

void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	
	const sam_usart_opt_t usart_console_settings = {
		WIFI_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(WIFI_USART_ID);

	/* Configure USART. */
	usart_init_rs232(WIFI_USART, &usart_console_settings, sysclk_get_peripheral_hz());

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);
	
	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
	
	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);
}

void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_NUM, WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_NUM);
}

void configure_wifi_provision_pin(void) // prov button was pushed, go to interrup wifi_prov_response_handler
{
		/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_PROV_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_PROV_PIO, WIFI_PROV_ID, WIFI_PROV_PIN_NUM, WIFI_PROV_ATTR, wifi_provision_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_PROV_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_PROV_PIO, WIFI_PROV_PIN_NUM);
}

void SPI_Handler(void)
{
	uint32_t new_cmd = 0;
	static uint16_t data;
	uint8_t uc_pcs;

	if (spi_read_status(SPI) & SPI_SR_RDRF) {
		spi_read(SPI, &data, &uc_pcs);
		
		if (transfer_len--) {
			spi_write(SPI, transfer_index++, 0, 0);
		}
	}
}

void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t wifi_usart_settings = {
		WIFI_USART_BAUDRATE,
		WIFI_USART_CHAR_LENGTH,
		WIFI_USART_PARITY,
		WIFI_USART_STOP_BITS,
		WIFI_USART_MODE,
		/* This field is only used in IrDA mode. */
		0
	};
	
	/* Get system clock. */
	ul_sysclk = sysclk_get_cpu_hz();
	
	pmc_enable_periph_clk(WIFI_USART_ID);
	
	usart_init_rs232(WIFI_USART,&wifi_usart_settings,ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);

	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}

void configure_spi()
{
	gpio_configure_pin(SPI_MISO_GPIO, SPI_MISO_FLAGS);
	gpio_configure_pin(SPI_MOSI_GPIO, SPI_MOSI_FLAGS);
	gpio_configure_pin(SPI_SPCK_GPIO, SPI_SPCK_FLAGS);
	gpio_configure_pin(SPI_NPCS0_GPIO, SPI_NPCS0_FLAGS);
	
	/* Configure SPI interrupts for slave only. */
	NVIC_DisableIRQ(SPI_IRQn);
	NVIC_ClearPendingIRQ(SPI_IRQn);
	NVIC_SetPriority(SPI_IRQn, 0);
	NVIC_EnableIRQ(SPI_IRQn);
}

void spi_peripheral_initialize(void)
{
	spi_enable_clock(SPI);
	spi_disable(SPI);
	spi_reset(SPI);
	spi_set_slave_mode(SPI);
	spi_disable_mode_fault_detect(SPI);
	spi_set_peripheral_chip_select_value(SPI, SPI_CHIP_PCS);
	spi_set_clock_polarity(SPI, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI, SPI_CHIP_SEL, SPI_CSR_BITS_8_BIT);
	spi_enable_interrupt(SPI, SPI_IER_RDRF);
	spi_enable(SPI);
}

void spi_prep_transfer(void)
{
	transfer_len = 100;
	transfer_index = 0;
}

void write_wifi_command(char* comm, uint8_t cnts)
{
	usart_write_line(WIFI_USART,comm);
	counts = 0;
	while (counts < cnts)
	{
		if (wifi_comm_success)
		{
			wifi_comm_success = false; 
			break;
		}
	}
}

void write_image_to_web(void)
{
	uint8_t length;
	length = find_image_length();
	if (length == 0)
		{return;} //ensure the image is valid ahead of time
	char* message[100];
	sprintf(message, "image_transfer %d",length);
	write_wifi_command(message, 3);
	while (!wifi_comm_success){} // wait till ESP done
	return;
	
	
}