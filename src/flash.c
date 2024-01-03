#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jakestering.h>

#include "flash.h"

const int ADDR[ 15 ] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 };
const int DATA[ 8 ]  = { 18, 19, 20, 21, 22, 23, 24, 25 };

void initFlashProgrammer( void )
{
  setAddrOutput();
  setDataOutput();

  pinMode( WEB, OUTPUT );
  pinMode( OEB, OUTPUT );
  pinMode( CEB, OUTPUT );

  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, HIGH );
  digitalWrite( CEB, LOW  );
}

void byteProgram( unsigned int addr, unsigned int byte )
{
  setDataOutput();

  writeChip( 0x5555, 0xaa );
  writeChip( 0x2aaa, 0x55 );
  writeChip( 0x5555, 0xa0 );
  writeChip( addr  , byte );

  delayMicro( 20 );
}

void chipErase( void )
{
  setDataOutput();

  writeChip( 0x5555, 0xaa );
  writeChip( 0x2aaa, 0x55 );
  writeChip( 0x5555, 0x80 );
  writeChip( 0x5555, 0xaa );
  writeChip( 0x2aaa, 0x55 );
  writeChip( 0x5555, 0x10 );

  delay( 100 );
}

void softwareIdEntry( void )
{
  setDataOutput();

  writeChip( 0x5555, 0xaa );
  writeChip( 0x2aaa, 0x55 );
  writeChip( 0x5555, 0x90 );

  delayMicro( 1 );
}

void softwareIdExit( void )
{
  setDataOutput();

  writeChip( 0x5555, 0xaa );
  writeChip( 0x2aaa, 0x55 );
  writeChip( 0x5555, 0xf0 );

  delayMicro( 1 );
}

void writeChip( unsigned int addr, unsigned int data )
{
  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, HIGH );

  writeAddr( addr );
  writeData( data );

  digitalWrite( WEB, LOW  );
  delayMicro( 1 );
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
  digitalWrite( DATA[ 0 ], 1 & ( data >> 0 ) );
  digitalWrite( DATA[ 1 ], 1 & ( data >> 1 ) );
  digitalWrite( DATA[ 2 ], 1 & ( data >> 2 ) );
  digitalWrite( DATA[ 3 ], 1 & ( data >> 3 ) );
  digitalWrite( DATA[ 4 ], 1 & ( data >> 4 ) );
  digitalWrite( DATA[ 5 ], 1 & ( data >> 5 ) );
  digitalWrite( DATA[ 6 ], 1 & ( data >> 6 ) );
  digitalWrite( DATA[ 7 ], 1 & ( data >> 7 ) );
}

unsigned int readData( void )
{
  setDataInput();
  
  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, LOW  );

  unsigned int temp;

  for ( int i = 0; i < 8; i++ )
  {
    temp = digitalRead( DATA[i] ) << i;
  }

  return temp;
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

