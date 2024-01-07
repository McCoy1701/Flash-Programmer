#include <jakestering.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "command.h"

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
