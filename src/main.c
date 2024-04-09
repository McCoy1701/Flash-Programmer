#include <jakestering.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "flash.h"
#include "commandLineRead.h"

struct timeval start, end;
double executionTime;

uint8_t* bytes;
size_t size;

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

      digitalWrite( CEB, LOW );
      digitalWrite( OEB, HIGH );
      
      for ( int i = 0; i < size; i++ )
      {
        byteProgram( addr, bytes[ i ] );
        
        //progressbar( i, size );
        
        printf( "bytes written: %d/%zu\r", addr, size );
        
        fflush( stdout ); 
        
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
      }
      
      for ( int i = 0; i < count; i += 16 )
      {
        printf( "%04x: | ", i );

        for ( int j = 0; j < 16; j += 2 )
        {
          if ( i + j < count )
          {
            printf( "%02x%02x", arrayTemp[ ( i + j ) ], arrayTemp[ ( i + j ) + 1 ] );
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
      
      unsigned int addr = 0;

      int checked = 0;

      for ( int i = 0; i < size; i++ )
      {
        unsigned int tempByte = readFlash( addr );

        printf( "Checking validity of %d / %zu bytes\r", checked, size );
        
        fflush( stdout );
        
        if ( tempByte == bytes[ i ] )
        {
          checked++;
        }
       
        else
        {
          printf( "failed to program Addr: %04x, Data: %02x, Failed Data: %02x\n", addr, bytes[i], tempByte );
        }

        if ( addr ++ > 0x7fff ) addr = 0x00;
      }

      printf( "\n" );
    }

    if (strcmp( argv[ i ], "-h" ) == 0 )
    {
      printf("Help - SST30SFXXX Flash Programmer\n");
      printf("  -w file   : Write a binary file to flash device\n");
      printf("  -r amount : Read amount of bytes( -r 150 )\n");
      printf("  -v        : Check the version of Flash\n");
      printf("  -c file   : Verify the contents of Flash are the same as the file\n");
      printf("  -e        : Erase the whole Flash\n");
    }
  }

  gettimeofday( &end, NULL );
  
  executionTime = ( end.tv_sec - start.tv_sec ) + ( end.tv_usec - start.tv_usec ) / 1000000.0f;

  printf( "Time taken to program: %f\n", executionTime );

  if (bytes != NULL) free( bytes );
}

