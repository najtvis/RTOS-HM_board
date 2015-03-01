#include "spi_memory_23lcxx.h"
#include "config.h"
#include "spi.h"
#include "spi_master.h"


char buffer_memory[32];		// buffer for saving and loading data from SPI memory

struct spi_device spi_device_conf = {
	.id = IOPORT_CREATE_PIN(PORTC, 4)
};

void spi_mem_init(void)
{
	
	
	ioport_configure_port_pin(&SPI_PORT, PIN4_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);		// CS
	ioport_configure_port_pin(&SPI_PORT, PIN5_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);		// MOSI
	ioport_configure_port_pin(&SPI_PORT, PIN6_bm, IOPORT_DIR_INPUT);							// MISO
	ioport_configure_port_pin(&SPI_PORT, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);		// SCK
	

	spi_master_init(&SPI_MEM_INTERFACE);
	spi_master_setup_device(&SPI_MEM_INTERFACE, &spi_device_conf, SPI_MODE_0, 2000000, 0);
	spi_enable(&SPI_MEM_INTERFACE);
	
}

// void spi_mem_write_page(unsigned long address, unsigned int no_pages)
// {
// unsigned char i;
// 	SET_MEM_CS_LOW
// 	memory_send(SPI_WREN);		// write enable command
// 	SET_MEM_CS_HIGH
// 	
// 	SET_MEM_CS_LOW
// 	memory_send(SPI_WRITE);	// write command
// 	memory_send((address & 0xFF0000) >> 16);
// 	memory_send((address & 0x00FF00) >> 8);
// 	memory_send((address & 0x0000FF));
// 	
// 	for (i = 3; i<no_pages; ++i)
// 	{
// 		memory_send(buffer_memory[i]);
// 	}
// 	SET_MEM_CS_HIGH
// 	
// 	buffer_memory[0] = (address & 0xFF0000) >> 16;
// 	buffer_memory[1] = (address & 0x00FF00) >> 8;
// 	buffer_memory[2] = (address & 0x0000FF);
// 	
// 	for (i = 3; i<no_pages+3; ++i)
// 	{
// 		memory_send(buffer_memory[i]);
// 	}
// 	
// 
// }

void spi_mem_write_word(unsigned long address, unsigned int data)
{
	//memcpy(buffer_memory,&address,4);
	buffer_memory[0] = SPI_WRITE;
 	buffer_memory[1] = ((address & 0xFF0000) >> 16);
 	buffer_memory[2] = ((address & 0x00FF00) >> 8);
 	buffer_memory[3] = ((address & 0x0000FF));

	//memcpy(buffer_memory+4,&data,2);
	buffer_memory[4] = data>>8;
	buffer_memory[5] = data;
	
	spi_select_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	spi_write_packet(&SPI_MEM_INTERFACE, buffer_memory, 6);
	spi_deselect_device(&SPI_MEM_INTERFACE, &spi_device_conf);

 
}

void spi_mem_write_byte(unsigned long address, unsigned char data)
{
//	memcpy(buffer_memory,&address,4);
	buffer_memory[0] = SPI_WRITE;
 	buffer_memory[1] = ((address & 0xFF0000) >> 16);
 	buffer_memory[2] = ((address & 0x00FF00) >> 8);
 	buffer_memory[3] = ((address & 0x0000FF));

	//memcpy(buffer_memory+4,&data,1);
	buffer_memory[4] = data;
	
	spi_select_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	spi_write_packet(&SPI_MEM_INTERFACE, buffer_memory, 5);
	spi_deselect_device(&SPI_MEM_INTERFACE, &spi_device_conf);

}

void spi_mem_write_complex(unsigned long address, complex data)
{
	//memcpy(buffer_memory,&address,4);
	buffer_memory[0] = SPI_WRITE;
 	buffer_memory[1] = ((address & 0xFF0000) >> 16);
 	buffer_memory[2] = ((address & 0x00FF00) >> 8);
 	buffer_memory[3] = ((address & 0x0000FF));
	
	
			
	spi_select_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	spi_write_packet(&SPI_MEM_INTERFACE, buffer_memory, 4);
	memcpy(buffer_memory,&data,8);
	spi_write_packet(&SPI_MEM_INTERFACE, buffer_memory, 8);
	spi_deselect_device(&SPI_MEM_INTERFACE, &spi_device_conf);

	
}

// void spi_mem_read_page(unsigned long address, unsigned char no_pages)
// {
// unsigned char i;
// 	SET_MEM_CS_LOW
// 	memory_send(SPI_READ);
// 	memory_send((address & 0xFF0000) >> 16);
// 	memory_send((address & 0x00FF00) >> 8);
// 	memory_send((address & 0x0000FF));
// 	
// 
// 	for (i = 0; i<no_pages; ++i)
// 	{
// 		buffer_memory[i] = memory_read();
// 	}
// 	SET_MEM_CS_HIGH	
// 	
// }

unsigned int spi_mem_read_word(unsigned long address)
{
	unsigned int data;
	
	//memcpy(buffer_memory,&address,4);
	buffer_memory[0] = SPI_READ;
	buffer_memory[1] = ((address & 0xFF0000) >> 16);
	buffer_memory[2] = ((address & 0x00FF00) >> 8);
	buffer_memory[3] = ((address & 0x0000FF));
	
	spi_select_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	spi_write_packet(&SPI_MEM_INTERFACE, buffer_memory, 4);
	spi_read_packet(&SPI_MEM_INTERFACE, buffer_memory, 2);
	spi_deselect_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	
	//memcpy(&data,buffer_memory,2);
	data = (int)(buffer_memory[0]<<8);
	data |= (int) buffer_memory[1];
	
	return data;
 }

char spi_mem_read_byte(unsigned long address)
{
	//unsigned char data;
	
//	memcpy(buffer_memory,&address,4);
	buffer_memory[0] = SPI_READ;
 	buffer_memory[1] = ((address & 0xFF0000) >> 16);
 	buffer_memory[2] = ((address & 0x00FF00) >> 8);
 	buffer_memory[3] = ((address & 0x0000FF));
	
	spi_select_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	spi_write_packet(&SPI_MEM_INTERFACE, buffer_memory, 4);
	spi_read_packet(&SPI_MEM_INTERFACE, buffer_memory, 1);
	spi_deselect_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	
	//memcpy(&data,buffer_memory,1);
	
	return buffer_memory[0];
}

complex spi_mem_read_complex(unsigned long address)
{
	complex data;
	//memcpy(buffer_memory,&address,4);
	buffer_memory[0] = SPI_READ;
 	buffer_memory[1] = ((address & 0xFF0000) >> 16);
 	buffer_memory[2] = ((address & 0x00FF00) >> 8);
 	buffer_memory[3] = ((address & 0x0000FF));
	
	spi_select_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	spi_write_packet(&SPI_MEM_INTERFACE, buffer_memory, 4);
	spi_read_packet(&SPI_MEM_INTERFACE, buffer_memory, 8);
	spi_deselect_device(&SPI_MEM_INTERFACE, &spi_device_conf);
	
	memcpy(&data,buffer_memory,8);
			
	return data;
}
// 
// 
// 
// unsigned char memory_read_status(void)
// {
// unsigned char data;
// data = 0;
// 
// 	SET_MEM_CS_LOW
// 	memory_send(SPI_RDSR);
// 	data = memory_read();
// 	SET_MEM_CS_HIGH	
// 	return data;
// }
// 
// bool memory_ready_write(void)
// {
// 	if ((memory_read_status() & 0x01) == 0)
// 		return true;
// 	else
// 		return false;
// }
// 
// void memory_unprotect(void)
// {
// 	SET_MEM_CS_LOW
// 	//_delay_us(2);
// 	memory_send(SPI_WREN);		// write enable command
// 	//_delay_us(2);
// 	SET_MEM_CS_HIGH
// 	
// 	//while(memory_ready_write() == false);
// 	
// 	SET_MEM_CS_LOW
// 	//_delay_us(2);
// 	memory_send(SPI_WRSR);
// 	memory_send(0x00);
// 
// 	SET_MEM_CS_HIGH	
// }

