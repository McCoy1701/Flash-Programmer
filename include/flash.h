#ifndef __FLASH_H__
#define __FLASH_H__

#define OEB 25
#define WEB 24
#define CEB 23

void initFlashProgrammer( void );

void byteProgram( unsigned int addr, unsigned int byte );

void flashErase( void );

void softwareIdEntry( void );

void softwareIdExit( void );

unsigned int testId( void );

void writeFlash( unsigned int addr, unsigned int data );

void writeAddr( unsigned int addr );

void writeData( unsigned int data );

unsigned int readFlash( unsigned int addr );

unsigned int readData( void );

void setAddrOutput( void );

void setDataOutput( void );

void setDataInput( void );

#endif

