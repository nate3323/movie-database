/**
   @file movies.c
   @author Nathan Cornelison dncornel
   The main program that accepts user input and prints output to standard output based on the
   input. Accepts the commands: database, year, title, genre, add, remove, list, and quit.
   Holds the database and watchlist for the program.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "database.h"

/**
   Prints a message for the user stating a command was invalid.
 */
static void invalid() {
   printf("Invalid command\n");
}

/**
   This static helper method removes a movie from the watchlist. Prints an error
   response if the watch does not have the movie.
   @param watch is a pointer to the watchlist
   @param the id of the movie to be added
 */
static void rMovie( Database *watch, int ident ) {
   Movie *m = (Movie *)malloc( sizeof( Movie ) );
   int rm = 0;
   for( int i = 0; i < watch->count; i++) {
      m = (watch->movies)[ i ];
      if( m->id == ident ) {
         for( int j = i + 1; j < watch->count; j++) {
            (watch->movies)[ i ] = (watch->movies)[ j ];
            i++;
         }
         watch->count--;
         rm = 1;
      }
   }
   if( rm == 0 ) {
      printf("Movie %d is not on the watch list\n", ident);
   }
}


/**
   This static helper method adds a movie to the watchlist from the database. Prints an error
   response if the database does not have the movie or if the movie is already on the watchlist.
   @param dat is a pointer to the database
   @param watch is a pointer to the watchlist
   @param the id of the movie to be added
 */
static void addMovie( Database *dat, Database *watch, int ident ) {
   Movie *m = (Movie *)malloc( sizeof( Movie ) );
   for(int k = 0; k < dat->count ; k++ ) {
      m = (dat->movies)[ k ];
      if( m->id == ident ) {
         int add = 1;
         for( int l = 0; l < watch->count; l++ ) {
            if( (watch->movies)[ l ]->id == m->id ) {
               printf("Movie %d is already on the watch list\n", ident);
               add = 0;
            }
         }
         if( add == 1 ) {
            (watch->movies)[ watch->count ] = m;
         (watch->count)++;
         }
      }
   }
   if( ( watch->count ) >= ( watch->capacity ) ) {
      (watch->capacity) *= ARG_DIV;
      (watch->movies) = (Movie **)realloc(( watch->movies ), (watch->capacity) * sizeof(Movie *));
      for( int k = (watch->count); k < (watch->capacity); k++ ) {
         (watch->movies)[ k ] = (Movie *)malloc( sizeof( Movie ) );
      }
   }
   if( m->id == 0 ) {
      printf("Movie ID is not in the database\n");
   }
}

/**
   Runs the movie program, taking in a series of movielists via command-line arguments.
   Accepts a series of commands and exits on either the quit command or EOF.
   @param argc the number of provided arguments
   @param argv the array of arguments
   @return the success of the program
 */
int main( int argc, char *argv[]) {
   if( argc < ARG_DIV ) {
      fprintf( stderr, "usage: movies <movie-list>*\n");
      exit( EXIT_FAILURE );
   }
   Database *dat = makeDatabase();
   Database *watch = makeDatabase();
   int i = 1;
   while( i < argc) {
      readDatabase( dat, argv[i] );
      i++;
   }
   char cmd[ARRAY_SIZE_STARTER][ARRAY_SIZE_STARTER * ARG_DIV];
   int args = scanf("%s", cmd[ 0 ]);
   while( args != EOF) {
      if(strcmp("quit", cmd[ 0 ]) == 0) {
         printf("cmd> %s\n", cmd[ 0 ]);
         return EXIT_SUCCESS;
      } else if(strcmp("database", cmd[ 0 ]) == 0) {
         printf("cmd> %s\n", cmd[ 0 ]);
         listAll( dat );
      } else if(strcmp("year", cmd[ 0 ]) == 0) {
         args = scanf( "%s %s", cmd[ 1 ], cmd[ ARG_DIV ]);
         printf("cmd> %s", cmd[ 0 ]);
         int k = 0;
         int min = 0;
         int max = 0;
         if( args < ARG_DIV ) {
            printf("cmd>");
            while(k < args) {
               printf(" %s", cmd[ k ]);
               k++;
            }
            printf("\n");
            invalid();
         } else if( sscanf( cmd[ 1 ], "%d", &min ) != 1 ) {
            printf(" %s %s\n", cmd[ 1 ], cmd[ ARG_DIV ] );
            invalid();
         } else if( sscanf( cmd[ ARG_DIV ], "%d", &max ) != 1 ) {
            printf(" %s %s\n", cmd[ 1 ], cmd[ ARG_DIV ] );
            invalid();
         } else if( max < min ) {
            printf(" %s %s\n", cmd[ 1 ], cmd[ ARG_DIV ] );
            invalid();
         } else {
            printf( " %s %s\n", cmd[ 1 ], cmd[ ARG_DIV ] );
            listYear( dat, min, max );
         }
      } else if(strcmp("title", cmd[ 0 ]) == 0) {
         args = scanf(" %s", cmd[ 1 ] );
         if( args != 1) {
            printf("cmd> %s/n", cmd[ 0 ]);
            invalid();
         } else {
            printf("cmd> %s %s\n", cmd[ 0 ], cmd[ 1 ]);
            listTitle( dat, cmd[ 1 ] );
         }
      } else if(strcmp("genre", cmd[ 0 ]) == 0) {
         printf("cmd> %s\n", cmd[ 0 ]);
         listAll( dat );
      } else if(strcmp("add", cmd[ 0 ]) == 0) {
         int ident = 0;
         args = scanf(" %d\n", &ident);
         if( args != 1) {
            printf("cmd> %s %s\n", cmd[ 0 ], cmd[ 1 ] );
            invalid();
         } else {
            printf("cmd> %s %d\n", cmd[ 0 ], ident);
            addMovie(dat, watch, ident);
         }
      } else if(strcmp("remove", cmd[ 0 ]) == 0) {
         int ident = 0;
         args = scanf(" %d\n", &ident);
         if( args != 1) {
            printf("cmd> %s %s\n", cmd[ 0 ], cmd[ 1 ] );
            invalid();
         } else {
            printf("cmd> %s %d\n", cmd[ 0 ], ident);
            rMovie(watch, ident);
         }
      } else if(strcmp("list", cmd[ 0 ]) == 0) {
         printf("cmd> %s\n", cmd[ 0 ] );
         if( watch->count == 0 ) {
            printf("List is empty\n");
         } else {
            printf("    ID                                  Title Year Len\n");
            for( int j = 0; j < watch->count; j++) {
               Movie *m = (watch->movies)[ j ];
               printf("%6d", (m->id) );
               printf(" %38s", (m->title) );
               printf(" %4d", (m->year) );
               printf(" %3d\n", (m->length) );
            }
         }
      } else {
         printf("cmd>");
         char p = '\0';
         printf(" %s", cmd[ 0 ]);
         while(p != '\n') {
            scanf("%c", &p);
            printf("%c", p);
         }
         invalid();
      }
      printf("\n");
      args = scanf("%s", cmd[ 0 ] );
   }
   printf("cmd> ");
   freeDatabase( watch );
   freeDatabase( dat );
   return EXIT_SUCCESS;
}