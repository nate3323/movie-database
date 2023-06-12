/**
   @file database.h
   @author Nathan Cornelison dncornel
 */

#include <stdbool.h>

#define TITLE_LENGTH 38

struct Movie {
   int id;
   char title[ TITLE_LENGTH + 1 ];
   int year;
   int length;
   
};

typedef struct Movie Movie;

struct Database {
   int count;
   int capacity;
   Movie **movies;
};

typedef struct Database Database;

extern Database *makeDatabase();

extern void freeDatabase( Database *dat );

extern void readDatabase( Database *dat, char const *filename );

extern void listAll( Database *dat );

extern void listYear( Database *dat, int min, int max );

extern void listTitle( Database *dat, char const *title );

extern void listGenre( Database *dat, char const *genre );

//static void listDatabase( Database *dat, bool (*test)( Movie const *movie, void const *data)
//, void const *data );