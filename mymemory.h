/*  
  Submission: CGS A5-A1  
  Student Name: Wiktoria Szitenhelm
  Student ID: 52092974  
    
  mymemory.h
  describes structures for memory management
*/

#ifndef MYMEMORY_H
#define MYMEMORY_H

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MAXMEM       1024
#define MAXSEGMENTS   100

// This typedef defines a byte
typedef unsigned char Byte ;

// This is our memory, we declare it as extern so that it becomes global
extern Byte mymemory [ MAXMEM ] ;


// The segment descriptor, a node in a linked list
typedef struct segmentdescriptor {
   Byte     allocated ;
   void   * start ;
   size_t   size  ;
   struct segmentdescriptor * next ;
} Segment_t ;

// This is our memory, we declare it as extern so that it becomes global
extern Segment_t * segmenttable;
extern void ** ptrlist[MAXSEGMENTS];


// Main functions
void        initialize () ;
void      * mymalloc   ( size_t  ) ;
void        myfree     ( void *  ) ;
void        mydefrag   ( void ** ) ;
void free_the_pointer(char **) ;

// Functions to manage segmentation table
Segment_t * findFree    ( Segment_t *, size_t      ) ;
void        insertAfter ( Segment_t *, Segment_t * ) ;
Segment_t * findSegment ( Segment_t * , void *  ) ;

// Helper function
int isPrintable ( int c ) ;

// Funtion to print memory
void printmemory () ;
// Funtion to print segments
void printsegmenttable() ;

// Helper functions to implement mydefrag()
//int delSegment ( Segment_t * , void *  );
void * moveSegment ( Segment_t * ,void *   ) ;

void update_segmenttable_address(void **  ) ;

// Second implementations of helping functions for defrag 
int delSegment ( Segment_t * , Segment_t *  );
//void * moveSegment ( Segment_t * , Segment_t *   ) ;
#endif