#include "system.h"
#include "config.h"
#include "spi.h"

#ifndef SPI_MEMORY
#define SPI_MEMORY

typedef struct               // one point is 8 bytes large
{
	float real;
	float imag;
} complex;


void spi_mem_write_page(unsigned long address, unsigned int no_pages);
void spi_mem_write_word(unsigned long address, unsigned int data);
void spi_mem_write_byte(unsigned long address, unsigned char data);
void spi_mem_write_complex(unsigned long address, complex data);


void spi_mem_read_page(unsigned long address, unsigned char no_pages);
unsigned int spi_mem_read_word(unsigned long address);
char spi_mem_read_byte(unsigned long address);
complex spi_mem_read_complex(unsigned long address);


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

#endif