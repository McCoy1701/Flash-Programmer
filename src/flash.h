#ifndef __FLASH_H__
#define __FLASH_H__

/*#define ADDR_0  0
#define ADDR_1  1
#define ADDR_2  2
#define ADDR_3  3
#define ADDR_4  4
#define ADDR_5  5
#define ADDR_6  6
#define ADDR_7  7
#define ADDR_8  8
#define ADDR_9  9
#define ADDR_10 10
#define ADDR_11 11
#define ADDR_12 12
#define ADDR_13 13
#define ADDR_14 14*/

#define WEB 15
#define OEB 16
#define CEB 17

/*#define DATA_0 18
#define DATA_1 19
#define DATA_2 20
#define DATA_3 21
#define DATA_4 22
#define DATA_5 23
#define DATA_6 24
#define DATA_7 25*/

void initFlashProgrammer( void );

void byteProgram( unsigned int addr, unsigned int byte );

void chipErase( void );

void softwareIdEntry( void );

void softwareIdExit( void );

void writeChip( unsigned int addr, unsigned int data );

void writeAddr( unsigned int addr );

void writeData( unsigned int data );

unsigned int readData( void );

void setAddrOutput( void );

void setDataOutput( void );

void setDataInput( void );

#endif

