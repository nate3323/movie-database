/**
   @file database
   @author Nathan Cornelison dncornel
   This component maintains databases and provides the functions to print out all or parts
   of the database.
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <string.h>
 #include "input.h"
 #include "database.h"
 
/**
   Dynamically allocates storage for the database, initializes its fields and returns
   a pointer to it
   @return a pointer to the Database
 */
Database *makeDatabase() {+

   Database *dat = (Database *)malloc( sizeof( Database ));
   dat->count = 0;
   dat->capacity = ARRAY_SIZE_STARTER;
   dat->movies = (Movie **)malloc( ( dat->capacity ) * sizeof( Movie *));
   for( int i = 0; i < dat->capacity; i++ ) {
      (dat->movies)[ i ] = (Movie *)malloc( sizeof( Movie ) );
   }
   return dat;
}

/**
   Static function used in qsort to sort by ID. It returns positive if the first movie has a
   larger ID, negative if it has a smaller ID, and 0 otherwise.
   @param the first movie to compare
   @param the second movie to compare
   @return the compare value
 */
static int sortID(const void *movie, const void *data ) {
   const Movie* const* m1 = movie;
   const Movie* const* m2 = data;
   const int i = (*m1)->id;
   const int j = (*m2)->id;
   
   if( i < j ) {
      return -1;
   }
   if( i > j ) {
      return 1;
   }
   return 0;
}

/**
   This static method returns a positive integer value if movie was made after movie 2 or if it
   was made the same year but has a larger ID, negative if it was made before or was made in the,
   same year but had a smaller ID, and zero otherwise.
   @param movie the first movie to compare
   @param the second movie to compare
   @return the compare value
 */
static int sortYear(const void *movie, const void *data ) {
   const Movie* const* m1 = movie;
   const Movie* const* m2 = data;
   const int i = (*m1)->id;
   const int j = (*m2)->id;
   const int y1 = (*m1)->year;
   const int y2 = (*m2)->year;
   
   if( y1 > y2 ) {
      return 1;
   }
   if( y1 < y2 ) {
      return -1;
   }
   if( y1 == y2 ) {
      if( i > j ) {
         return 1;
      }
      if( i < j ) {
         return -1;
      }
   }
   return 0;
}

/**
   Static function used in listAll as a placeholder method
   @param the movie to check;
   @param the data to check against
   @return the compare value
 */
static bool testAll( Movie const *movie, void const *data ) {
   return true;
}


/**
   Static function used in listYear
   @param movie the movie to check
   @param an array of 2 years to check to see if a movie was made within that time frame
   @return the compare value
 */
static bool testYear( Movie const *movie, void const *data ) {
   Movie const *m1 = movie;
   int const *i = data;
   if( m1->year >= i[ 0 ] && m1->year <= i[ 1 ] ) {
      return true;
   }
   return false;
}

/**
   Static function used in listTitle
   @param movie the movie to check
   @param data The string that will either be in or not be in the movie's title
   @return the compare value
 */
static bool testTitle( Movie const *movie, void const *data ) {
   char const *line = data;
   if( strstr( movie->title, line) ) {
      return true;
   }
   return false;
}


/**
   This is a static function that is used in various functionn to actually report the list of
   movies in the right format.
   @param dat the database to list
   @param test the test to decide what movies to list
   @param data the data to sort the list with
 */
static void listDatabase( Database *dat, bool (*test)( Movie const *movie, void const *data)
, void const *data ) {
   Movie **list = ( Movie **)malloc( dat->capacity * sizeof( Movie * ) );
   for( int k = 0; k < dat->capacity; k++ ) {
      list[ k ]  = ( Movie *)malloc( sizeof( Movie ) );
   }
   int listed = 0;
   for( int i = 0; i < dat->count; i++) {
      Movie *m = (dat->movies)[ i ];
      if( test( m, data ) ) {
         list[ listed ] = m;
         listed++;
      }
   }
   if( listed != 0) {
      printf("    ID                                  Title Year Len\n");
      if( test == testYear) {
         qsort(list, listed, sizeof( list[ 0 ] ), sortYear );
      } else {
         qsort( list, listed, sizeof( list[ 0 ] ), sortID );
      }
      
      for( int j = 0; j < listed; j++) {
         Movie *m = list[ j ];
         printf("%6d", (m->id) );
         printf(" %38s", (m->title) );
         printf(" %4d", (m->year) );
         printf(" %3d\n", (m->length) );
      }
   } else {
      printf("No matching movies\n");
   }
   free( list );
}

/**
   This funciton frees the memory used to store the database, including 
   freeing space for all movies, freeing the resizable array of movie 
   pointers &freeing spacefor the database struct itself
   @param the database to free
 */
void freeDatabase( Database *dat ) {
   free( (dat->movies) );
   free( dat );
}

/**
   This function reads all the movies from a movie list file with the given name.
   It makes an instance of the Movie struct for each one & stores a pointer to
   that movie in the resizable array.
   @param dat the given database
   @param filename the name of the file to be read into the database
 */
void readDatabase( Database *dat, char const *filename ) {
    FILE *fp = fopen( filename, "r" );
    if( !fp ) {
       fprintf( stderr, "Can't open file: %s\n", filename);
       exit( EXIT_FAILURE );
    }
    char *line = readLine( fp );
    while( line != NULL ) {
       if( ( dat->count ) >= ( dat->capacity ) ) {
         (dat->capacity) *= ARG_DIV;
         ( dat->movies ) = (Movie **)realloc( (dat->movies), (dat->capacity) * sizeof( Movie * ));
         for( int i = (dat->count); i < (dat->capacity); i++ ) {
            (dat->movies)[ i ] = (Movie *)malloc( sizeof( Movie ) );
         }
       }
       Movie *m = (dat->movies)[ dat->count ];
       int position = 0;
       int pos = 0;
       int ident = 0;
       if( sscanf( line, "%d\t%n", &ident, &pos) != 1 ) {
          fprintf( stderr, "Invalid movie list: %s\n", filename);
          exit( EXIT_FAILURE );
       }
       int j = 0;
       while( j < dat->count ) {
          if( ident == (dat->movies)[ j ]->id ) {
             fprintf( stderr, "Duplicate movie id: %d\n", ident);
             exit( EXIT_FAILURE );
          }
          j++;
       }
       m->id = ident;
       position += pos;
       int i = 0;
       char c = '\0';
       sscanf(line + position, "%c", &c);
       while(c != '\t' && i != TITLE_LENGTH ) {
          (m->title)[ i ] = c;
          i++;
          position++;
          if( sscanf(line + position, "%c", &c) != 1) {
             fprintf( stderr, "Invalid movie list: %s\n", filename);
             exit( EXIT_FAILURE );
          }
       }
       if( i == TITLE_LENGTH &&  c != '\t' ) {
          (m->title)[ i - ARG_DIV ] = '.';
          (m->title)[ i - 1 ] = '.';
          while(c != '\t') {
             sscanf(line + position, "%c", &c);
             position++;
          }
       }
       (m->title)[ i ] = '\0'; 
       if( sscanf( line + position, "%d\t%n", &(m->year), &pos) != 1 ) {
          fprintf( stderr, "Invalid movie list: %s\n", filename);
          exit( EXIT_FAILURE );
       }
       position += pos;
       if( sscanf( line + position, "%d\t%n", &(m->length), &pos) != 1 ) {
          fprintf( stderr, "Invalid movie list: %s\n", filename);
          exit( EXIT_FAILURE );
       }
       position += pos;
       ( dat->count ) += 1;
       line = readLine( fp );
    }
    
    
    fclose( fp );
}
 
/**
   This function lists all the movies in the database, sorted by ID number. The
   movies component can call this in response to the user entering the database
   command
   @param dat the database to list
 */
void listAll( Database *dat ) {
   listDatabase( dat, testAll, NULL);
}
 
/**
   This function lists all the movies with a year between the given min and max 
   values (inclusive). Your movies component can call this when the user enters
   the year command. In the output, movies should be sorted by year, and by ID
   if they have the same year.
   @param dat a pointer to the given database
   @param min the minimum year to sort by
   @param max the maximum year to sort by
 */
void listYear( Database *dat, int min, int max ) {
    int i[ ARRAY_SIZE_STARTER ] = { min, max };
    listDatabase( dat, testYear, i);
}
 
/**
   This function lists all the movies where the given title string occurs in the movie's
   title field. Lists in order of ID number
   @param dat the database to search
   @param title the string to search for within the movie titles
 */
void listTitle( Database *dat, char const *title ) {
       listDatabase( dat, testTitle, title);
}
 
/**
   This function lists all the movies with the given genre(s)
   @param dat the given database
   @param genre the genre to list
 */
void listGenre( Database *dat, char const *genre ) {
    
}

