#include <jakestering.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "main.h"

struct timeval start, end;
double executionTime;

uint8_t* bytes;
size_t size;

const int ADDR[ 15 ] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 };
const int DATA[ 8 ]  = { 18, 19, 20, 21, 22, 23, 24, 25 };

int main( int argc, char* argv[] )
{
  gettimeofday( &start, NULL );

  setupIO();
  
  initFlashProgrammer();
  
  for ( int i = 1; i < argc; i++ )
  {
    if (strcmp( argv[ i ], "-w" ) == 0 )
    {
      bytes = readFile( argv[ i + 1 ], &size );
      
      printf( "writing %zu bytes to flash device\n", size );

      unsigned int addr = 0x00;

      for ( int i = 0; i < size; i++ )
      {
        byteProgram( addr, bytes[ i ] );
        
        progressbar( i, size );
    //  printf( " addr: %04x, data: %02x, size: %zu\n ", addr, bytes[ i ], size );
   
        if ( addr++ > 0x7fff )
        {
          addr = 0x00;
        }
      }

      printf( "\nFinished! Wrote %zu bytes\n", size );
      
      addr = 0x00;
    }

    if (strcmp( argv[ i ], "-e" ) == 0 )
    {
      flashErase();
    }
    
    if (strcmp( argv[ i ], "-v" ) == 0 )
    {
      testId();
    }
    
    if (strcmp( argv[ i ], "-r" ) == 0 )
    {
      int count = atoi( argv[ i + 1 ] );
      
      uint8_t *arrayTemp = ( uint8_t* )malloc( sizeof( uint8_t ) * count );
      
      if ( arrayTemp == NULL )
      {
        printf( "failed to allocate memory for array" );
        exit(0);
      }

      for (int i = 0; i < count; i ++ )
      {
        arrayTemp[ i ] = readFlash( i );
        //printf("addr: %04x, data: %02x\n", i, temp);
      }
      
      for ( int i = 0; i < count; i += 16 )
      {
        printf( "%04x: | ", i );

        for ( int j = 0; j < 16; j += 2 )
        {
          if ( i + j < count )
          {
            printf( "%02x%02x", arrayTemp[ ( i + j ) + 1 ], arrayTemp[ i + j ] );
          }
          else
          {
            printf( "  " );
          }

          if ( j < 14 )
          {
            printf( " " );
          }
        }

        printf( "\n" );
      }

      free(arrayTemp);
    }

    if ( strcmp( argv[ i ], "-c" ) == 0 )
    {
      bytes = readFile( argv[ i + 1 ], &size );
      
      printf( "\nChecking validity of %zu bytes\n", size );
      
      unsigned int addr = 0;

      int checked = 0;

      for ( int i = 0; i < size; i++ )
      {
        unsigned int tempByte = readFlash( addr );

        if ( tempByte == bytes[ i ] )
        {
          checked++;
        }
       
        else
        {
          printf( "failed to program Addr: %04x, Data: %02x, Failed Data: %02x\n", addr, bytes[i], tempByte );
        }

        progressbar( i, size );
        printf( "                                                          %d/%zu\r", checked, size );
        
        if ( addr ++ > 0x7fff ) addr = 0x00;
      }
    }
  }

  gettimeofday( &end, NULL );
  
  executionTime = ( end.tv_sec - start.tv_sec ) + ( end.tv_usec - start.tv_usec ) / 1000000.0f;

  printf( " Time taken to program: %f\n ", executionTime );

  if (bytes != NULL) free( bytes );
}

//
//
//
//

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
 
  digitalWrite( WEB, HIGH );
  digitalWrite( OEB, HIGH );
  digitalWrite( CEB, LOW  );

  writeFlash( 0x5555, 0xaa );
  writeFlash( 0x2aaa, 0x55 );
  writeFlash( 0x5555, 0xa0 );
  writeFlash( addr  , byte );

  delayMicro( 21 );

  digitalWrite( CEB, HIGH );
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
  
  delayMicro( 1 ); 
  
  digitalWrite( WEB, LOW  );
  
  writeData( data );
  
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

//
//
//
//

uint8_t* readFile( const char* filename, size_t *size )
{
  FILE* file;
  
  file = fopen( filename, "rb" );

  if ( file == NULL )
  {
    printf( " failed to load file: %s\n ", filename );
    return NULL;
  }

  fseek( file, 0, SEEK_END );
  long fileSize = ftell( file );
  rewind( file );

  *size = fileSize;

  uint8_t *bytes = ( uint8_t* )malloc( fileSize );

  if ( bytes == NULL )
  {
    printf( " bytes memory not allocated\n " );
    free( bytes );
    fclose( file );
    return NULL;
  }

  if (fread( bytes, sizeof( uint8_t ), *size, file ) != *size )
  {
    printf( " failed to read bytes\n " );
    free( bytes );
    fclose( file );
    return NULL;
  }

  fclose( file );

  return bytes;
}

//
//
//
//

void progressbar( int progress, int total )
{
  const int barWidth = 50;
  float percentage = ( float )progress / total;
  int progressLength = percentage * barWidth;
  
  printf( "[" );
  for ( int i = 0; i < barWidth; i++ )
  {
    if ( i < progressLength )
    {
      printf( "*" );
    }
    else
    {
      printf( " " );
    }
  }
  printf( "] \r" );
  fflush( stdout );
}
