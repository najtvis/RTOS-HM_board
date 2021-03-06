#include "system.h"
#include "config.h"

#ifndef COMPLEX_STRUCT
#define COMPLEX_STRUCT

typedef struct               // one point is 4 bytes large
{
	long real;
	long imag;
} complex;

#endif


#ifndef SPI_MEMORY
#define SPI_MEMORY

void spi_mem_write_page(unsigned int address, unsigned int no_pages);
void spi_mem_write_word(unsigned int address, unsigned int data);
void spi_mem_write_byte(unsigned int address, unsigned char data);
void spi_mem_write_complex(unsigned int address, complex data);


void spi_mem_read_page(unsigned int address, unsigned char no_pages);
unsigned int spi_mem_read_word(unsigned int address);
unsigned char spi_mem_read_byte(unsigned int address);
complex spi_mem_read_complex(unsigned int address);

void memory_send(unsigned char c);
unsigned char memory_read(void);
unsigned char memory_read_status(void);
bool memory_ready_write(void);
void memory_unprotect(void);
void spi_mem_init(void);

// definition of opcodes for SPI memory
#define SPI_READ 0x03		// read data from memory
#define SPI_WRITE 0x02		// write data to memory
#define SPI_WREN 0x06		// set the write enable latch - enable write operations
#define SPI_WRDI 0x04		// reset the write enable latch - disable write operations
#define SPI_RDSR 0x05		// read status register
#define SPI_WRSR 0x01		// write status register

// Macro used to drive the SPIC /SS signal low in order to select the slave
#define SET_MEM_CS_LOW ioport_set_pin_low(MEM_CS);
// Macro used to drive the SPIC /SS signal high in order to deselect the slave
#define SET_MEM_CS_HIGH ioport_set_pin_high(MEM_CS);

// Macro used to drive the SPI memory /WP signal low in order to write protect
#define SET_WP_LOW ioport_set_pin_low(MEM_WP);
// Macro used to drive the SPI memory /WP signal high in order to write unprotect
#define SET_WP_HIGH ioport_set_pin_low(MEM_WP);

#endif