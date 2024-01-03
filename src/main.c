#include <jakestering.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "command.h"
#include "flash.h"

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
      printf( " size: %zu\n ", size );
    }
  }

  unsigned int addr = 0x00;

  for ( int i = 0; i < size; i++ )
  {
    byteProgram( addr, bytes[ i ] );
     
    if ( ++addr >= 0x7fff )
    {
      addr = 0x00;
    }

    printf( " addr: %04x, data: %02x, size: %zu\n ", addr, bytes[i], size );
  }
  
  gettimeofday( &end, NULL );
  
  executionTime = ( end.tv_sec - start.tv_sec ) + ( end.tv_usec - start.tv_usec ) / 1000000.0f;

  printf( " Time taken to program: %f\n ", executionTime );

  free( bytes );
}
