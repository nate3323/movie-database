/**
   @file input.c
   @author Nathan Cornelison dncornel
   Manages the input for the movie database
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "input.h"

/**
   This function reads in a line from the given file into a string it returns
   @param the file to read
   @return the string read from the file
 */  
char *readLine( FILE *fp ) {
   int capacity = ARRAY_SIZE_STARTER;
   int len = 0;
   char *line = (char *)malloc( capacity * sizeof( char ));
   char val = '\0';
   while( fscanf( fp, "%c", &val) == 1 ) {
      if( val == '\n' ) {
         break;
      }
      if( len >= capacity ) {
         capacity *= ARG_DIV;
         line = (char *)realloc( line, capacity * sizeof( char ));
      }
      
      assert( len < capacity );
      line[ len++ ] = val;
   }
   if(val == '\0') {
      return NULL;
   }
   if( len >= capacity ) {
         capacity *= ARG_DIV;
         line = (char *)realloc( line, capacity * sizeof( char ));
   }
   line[ len ] = '\0';
   return line;
}