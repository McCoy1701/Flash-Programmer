#ifndef __MAIN_H__
#define __MAIN_H__

#define WEB 15
#define OEB 16
#define CEB 17

uint8_t* readFile( const char* filename, size_t *size );

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

void progressbar( int progress, int total );

#endif

