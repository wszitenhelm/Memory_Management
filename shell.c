/*  
  Submission: CGS A5-A1  
  Student Name: Wiktoria Szitenhelm
  Student ID: 52092974  
    
  shell.c
  Main part of the application.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymemory.h"
#include "mymemory.c"


int main()
{

  printf ( "shell> start\n");
  
  //initialization
  initialize();
  printf("\n\nMEMORY AFTER INITIALIZATION\n");
  printmemory();
  printsegmenttable();
  printf("\n\n");
   
  ptrlist[0] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[0], "this tes1");
  printf( "shell> content of allocated memory: %s\n", ptrlist[0] ) ;

  ptrlist[1] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[1], "this tes2");
  printf( "shell> content of allocated memory: %s\n", ptrlist[1] ) ;
  
  ptrlist[2] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[2], "this tes3");
  printf( "shell> content of allocated memory: %s\n", ptrlist[2] ) ;
  
  
  ptrlist[3] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[3], "this tes4");
  printf( "shell> content of allocated memory: %s\n", ptrlist[3] ) ;
  
  ptrlist[4] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[4], "this tes5");
  printf( "shell> content of allocated memory: %s\n", ptrlist[4] ) ;
  
  ptrlist[5] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[5], "this te99");
  printf( "shell> content of allocated memory: %s\n", ptrlist[5] ) ;

  ptrlist[6] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[6], "this te99");
  printf( "shell> content of allocated memory: %s\n", ptrlist[6] ) ;

  
  //printing after assignement
  printf("\n\nMEMORY AFTER ASSIGNEMENT\n");
  printmemory();
  printsegmenttable();
  printf("\n\n");
  
  myfree(ptrlist[0]);
  
  myfree(ptrlist[2]);
  
  myfree(ptrlist[3]);

  //printing after deleting
  printf("\n\nMEMORY AFTER DELETING\n");
  printmemory();
  printsegmenttable();
  printf("\n\n");
 
  mydefrag(ptrlist);

  printf("\n\nMEMORY AFTER DEFRAGMENTATION\n");
  printmemory();
  
  printsegmenttable();
  printf("\n\n");

  //adding more to see if the linked list works
  ptrlist[7] = (char *) mymalloc ( 10 ) ;
  strcpy (ptrlist[7], "this tes7");
  printf( "shell> content of allocated memory: %s\n", ptrlist[7] ) ;
  //test printing
  printf("\n\nMEMORY AFTER FINAL TESTING\n");
  printmemory();
  printsegmenttable();
  printf("\n\n");

  ptrlist[2] = (char *) mymalloc ( 9 ) ;
  strcpy (ptrlist[2], "this tes3");
  printf( "shell> content of allocated memory: %s\n", ptrlist[2] ) ;
  
  //test printing
  printf("\n\nMEMORY AFTER FINAL TESTING\n");
  printmemory();
  printsegmenttable();
  printf("\n\n"); 
  return 0;
}
