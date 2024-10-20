#include <jakestering.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "flash.h"

const int ADDR[ 15 ] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };
const int DATA[ 8 ]  = { 0, 1, 2, 3, 4, 5, 6, 7 };

void initFlashProgrammer( void )
{
  setAddrOutput();
  setDataInput();
  
  writeAddr( 0x0000 );
  writeData( 0x00 );

  pinMode( WEB, OUTPUT );
  pinMode( OEB, OUTPUT );
  pinMode( CEB, OUTPUT );

  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, HIGH );
  digitalWrite( CEB, HIGH );
}

void byteProgram( unsigned int addr, unsigned int byte )
{
  if (byte == 0xff ) return; 

  setDataOutput();
 
  //digitalWrite( WEB, HIGH );
  //digitalWrite( OEB, HIGH );
  //digitalWrite( CEB, LOW  );

  writeFlash( 0x5555, 0xaa );
  writeFlash( 0x2aaa, 0x55 );
  writeFlash( 0x5555, 0xa0 );
  writeFlash( addr  , byte );

  delayMicro( 21 );

  //digitalWrite( CEB, HIGH );
}

void flashErase( void )
{
  printf("erasing flash...\n");

  setDataOutput();

  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, HIGH );
  digitalWrite( CEB, LOW  );

  writeFlash( 0x5555, 0xaa );
  writeFlash( 0x2aaa, 0x55 );
  writeFlash( 0x5555, 0x80 );
  writeFlash( 0x5555, 0xaa );
  writeFlash( 0x2aaa, 0x55 );
  writeFlash( 0x5555, 0x10 );
  
  delay( 200 );
  digitalWrite( CEB, HIGH );
}

void softwareIdEntry( void )
{
  setDataOutput();

  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, HIGH );
  digitalWrite( CEB, LOW  );

  writeFlash( 0x5555, 0xaa );
  writeFlash( 0x2aaa, 0x55 );
  writeFlash( 0x5555, 0x90 );
  
  digitalWrite( CEB, HIGH );
}

void softwareIdExit( void )
{
  setDataOutput();

  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, HIGH );
  digitalWrite( CEB, LOW  );
  
  writeFlash( 0x5555, 0xaa );
  writeFlash( 0x2aaa, 0x55 );
  writeFlash( 0x5555, 0xf0 );
  
  digitalWrite( CEB, HIGH );
}

unsigned int testId( void )
{
  softwareIdExit();
  softwareIdEntry();

  unsigned int temp = readFlash( 0x0000 );

  if (temp != 0xbf )
  {
    printf( "read wrong id: %02x\n", temp );
    exit(0);
  }

  temp = readFlash( 0x0001 );

  switch ( temp )
  {
    case 0xb5:
      printf( "SST39SF010A detected ( 128k * 8 )\n" );
      break;

    case 0xb6:
      printf( "SST39SF020A detected ( 256k * 8 )\n" );
      break;

    case 0xb7:
      printf( "SST39SF040 detected ( 512k * 8 )\n" );
      break;

    default:
      printf( "unknown chip: %02x\n", temp );
      exit( 0 );
      break;
  }

  softwareIdExit();
  return temp;
}

void writeFlash( unsigned int addr, unsigned int data )
{
  writeAddr( addr );
  
//  delayMicro( 1 ); 
  
  digitalWrite( WEB, LOW  );
  
  writeData( data );
  
//  delayMicro( 1 );
  
  digitalWrite( WEB, HIGH );
}

void writeAddr( unsigned int addr )
{
  digitalWrite( ADDR[ 0 ] , 1 & ( addr >> 0  ) );
  digitalWrite( ADDR[ 1 ] , 1 & ( addr >> 1  ) );
  digitalWrite( ADDR[ 2 ] , 1 & ( addr >> 2  ) );
  digitalWrite( ADDR[ 3 ] , 1 & ( addr >> 3  ) );
  digitalWrite( ADDR[ 4 ] , 1 & ( addr >> 4  ) );
  digitalWrite( ADDR[ 5 ] , 1 & ( addr >> 5  ) );
  digitalWrite( ADDR[ 6 ] , 1 & ( addr >> 6  ) );
  digitalWrite( ADDR[ 7 ] , 1 & ( addr >> 7  ) );
  digitalWrite( ADDR[ 8 ] , 1 & ( addr >> 8  ) );
  digitalWrite( ADDR[ 9 ] , 1 & ( addr >> 9  ) );
  digitalWrite( ADDR[ 10 ], 1 & ( addr >> 10 ) );
  digitalWrite( ADDR[ 11 ], 1 & ( addr >> 11 ) );
  digitalWrite( ADDR[ 12 ], 1 & ( addr >> 12 ) );
  digitalWrite( ADDR[ 13 ], 1 & ( addr >> 13 ) );
  digitalWrite( ADDR[ 14 ], 1 & ( addr >> 14 ) );
}

void writeData( unsigned int data )
{
  digitalWrite( DATA[ 0 ], ( data & 1   ) ? 1 : 0 );
  digitalWrite( DATA[ 1 ], ( data & 2   ) ? 1 : 0 );
  digitalWrite( DATA[ 2 ], ( data & 4   ) ? 1 : 0 );
  digitalWrite( DATA[ 3 ], ( data & 8   ) ? 1 : 0 );
  digitalWrite( DATA[ 4 ], ( data & 16  ) ? 1 : 0 );
  digitalWrite( DATA[ 5 ], ( data & 32  ) ? 1 : 0 );
  digitalWrite( DATA[ 6 ], ( data & 64  ) ? 1 : 0 );
  digitalWrite( DATA[ 7 ], ( data & 128 ) ? 1 : 0 );
}

unsigned int readFlash( unsigned int addr )
{
  writeAddr( addr );
  delayMicro( 1 );
  
  setDataInput();
  delayMicro( 1 );

  digitalWrite( CEB, LOW );
  digitalWrite( OEB, LOW );
  delayMicro( 1 );

  unsigned int result = readData();
  
  digitalWrite( CEB, LOW );
  digitalWrite( OEB, LOW );
  delayMicro( 1 );

  return result;
}

unsigned int readData( void )
{
  setDataInput();

  unsigned int result = 0;
  
  if ( digitalRead( DATA[0] ) ) result |= 1;
  if ( digitalRead( DATA[1] ) ) result |= 2;
  if ( digitalRead( DATA[2] ) ) result |= 4;
  if ( digitalRead( DATA[3] ) ) result |= 8;
  if ( digitalRead( DATA[4] ) ) result |= 16;
  if ( digitalRead( DATA[5] ) ) result |= 32;
  if ( digitalRead( DATA[6] ) ) result |= 64;
  if ( digitalRead( DATA[7] ) ) result |= 128;
	
  return result;
}

void setAddrOutput( void )
{
  for (int i = 0; i < 15; i++ )
  {
    pinMode( ADDR[ i ], OUTPUT );
    digitalWrite( ADDR[ i ], LOW );
  }

}

void setDataOutput( void )
{
  for (int i = 0; i < 8; i++ )
  {
    pinMode( DATA[ i ], OUTPUT );
    digitalWrite( DATA[ i ], LOW );
  }
}

void setDataInput( void )
{
  for (int i = 0; i < 8; i++ )
  {
    pinMode( DATA[ i ], INPUT );
  }
}

