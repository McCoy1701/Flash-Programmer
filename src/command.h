#ifndef __COMMAND_H__
#define __COMMAND_H__

uint8_t* readCommandLine( int argc, char** argv,  size_t* size );

uint8_t* readFile( const char* filename, size_t *size );

#endif

