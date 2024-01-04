#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

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
  printf( "] %d%%\r", ( int )percentage * 100 );
  fflush( stdout );
}

