#include "spi_memory_25lcxx.h"
#include "config.h"


unsigned char buffer_memory[32];		// buffer for saving and loading from SPI memory

void spi_mem_init(void)
{
	
	// clk = F_CPU/2 approx. 8,235 MHz
	SPI_MEM_INTERFACE.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc | SPI_PRESCALER_DIV4_gc | SPI_CLK2X_bm;

	// SPIC interrupt: Disabled
	SPI_MEM_INTERFACE.INTCTRL = (SPI_MEM_INTERFACE.INTCTRL & (~SPI_INTLVL_gm)) | SPI_INTLVL_OFF_gc;


	SET_WP_HIGH;		// write unprotect
	SET_MEM_CS_HIGH;	// deselect SPI memory
}

void spi_mem_write_page(unsigned long address, unsigned int no_pages)
{
unsigned char i;
	SET_MEM_CS_LOW
	memory_send(SPI_WREN);		// write enable command
	SET_MEM_CS_HIGH
	
	SET_MEM_CS_LOW
	memory_send(SPI_WRITE);	// write command
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	
	for (i = 0; i<no_pages; ++i)
	{
		memory_send(buffer_memory[i]);
	}
	SET_MEM_CS_HIGH
}

void spi_mem_write_word(unsigned long address, unsigned int data)
{
	SET_MEM_CS_LOW
	memory_send(SPI_WREN);		// write enable command
	SET_MEM_CS_HIGH
	
	SET_MEM_CS_LOW
	memory_send(SPI_WRITE);	// write command
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	
	memory_send((data & 0xFF00) >> 8);
	memory_send((data & 0x00FF));
	
	SET_MEM_CS_HIGH

}

void spi_mem_write_byte(unsigned long address, unsigned char data)
{
	SET_MEM_CS_LOW
	memory_send(SPI_WREN);		// write enable command
	SET_MEM_CS_HIGH
	
	SET_MEM_CS_LOW
	memory_send(SPI_WRITE);	// write command
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	memory_send((data & 0x00FF));
	
	SET_MEM_CS_HIGH

}

void spi_mem_write_complex(unsigned long address, complex data)
{
	SET_MEM_CS_LOW
	memory_send(SPI_WREN);		// write enable command
	SET_MEM_CS_HIGH
	
	SET_MEM_CS_LOW
	memory_send(SPI_WRITE);	// write command
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	
	memory_send((data.real) >> 24);
	memory_send((data.real) >> 16);
	memory_send((data.real) >> 8);
	memory_send((data.real));
	
	memory_send((data.imag) >> 24);
	memory_send((data.imag) >> 16);
	memory_send((data.imag) >> 8);
	memory_send((data.imag));
	
	SET_MEM_CS_HIGH

}

void spi_mem_read_page(unsigned long address, unsigned char no_pages)
{
unsigned char i;
	SET_MEM_CS_LOW
	memory_send(SPI_READ);
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	

	for (i = 0; i<no_pages; ++i)
	{
		buffer_memory[i] = memory_read();
	}
	SET_MEM_CS_HIGH	
	
}

unsigned int spi_mem_read_word(unsigned long address)
{
	unsigned int data;
	SET_MEM_CS_LOW
	memory_send(SPI_READ);
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	
	data = memory_read()<<8;
	data |= memory_read();
	
	SET_MEM_CS_HIGH
	return data;
}

unsigned char spi_mem_read_byte(unsigned long address)
{
	unsigned char data;
	SET_MEM_CS_LOW
	memory_send(SPI_READ);
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	data = memory_read();
	
	SET_MEM_CS_HIGH
	return data;
}

complex spi_mem_read_complex(unsigned long address)
{
	complex data;
	long temp;
	char i;
	
	SET_MEM_CS_LOW
	memory_send(SPI_READ);
	memory_send((address & 0xFF0000) >> 16);
	memory_send((address & 0x00FF00) >> 8);
	memory_send((address & 0x0000FF));
	
	//data.real
	temp = memory_read();
	for(i=0;i<3;++i)
	{
		temp <<= 8;
		temp |= memory_read();
	}
	data.real = temp;
	
	temp = memory_read();
	for(i=0;i<3;++i)
	{
		temp <<= 8;
		temp |= memory_read();
	}	
	data.imag = temp;
	
	SET_MEM_CS_HIGH
	
	return data;
}

void memory_send(unsigned char c)
{
	// Transmit data in Master mode
	
	SPI_MEM_INTERFACE.DATA = c;
	// Wait for the data to be transmitted/received
	while ((SPI_MEM_INTERFACE.STATUS & SPI_IF_bm)==0);

}

unsigned char memory_read(void)
{
	// Transmit data in Master mode
	SPI_MEM_INTERFACE.DATA=0xFF;
	// Wait for the data to be transmitted/received
	while ((SPI_MEM_INTERFACE.STATUS & SPI_IF_bm)==0);
	return SPI_MEM_INTERFACE.DATA;

}

unsigned char memory_read_status(void)
{
unsigned char data;
data = 0;

	SET_MEM_CS_LOW
	memory_send(SPI_RDSR);
	data = memory_read();
	SET_MEM_CS_HIGH	
	return data;
}

bool memory_ready_write(void)
{
	if ((memory_read_status() & 0x01) == 0)
		return true;
	else
		return false;
}

void memory_unprotect(void)
{
	SET_MEM_CS_LOW
	//_delay_us(2);
	memory_send(SPI_WREN);		// write enable command
	//_delay_us(2);
	SET_MEM_CS_HIGH
	
	//while(memory_ready_write() == false);
	
	SET_MEM_CS_LOW
	//_delay_us(2);
	memory_send(SPI_WRSR);
	memory_send(0x00);

	SET_MEM_CS_HIGH	
}

