#include <stdio.h>

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
  printf( "] \r" );
  fflush( stdout );
}

