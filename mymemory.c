/*  
  Submission: CGS A5-A1  
  Student Name: Wiktoria Szitenhelm
  Student ID: 52092974  
    
  mymemory.c
  provides interface to memory management
*/

#include <stdio.h> //  It has the information for all input and output related functions.
#include <unistd.h> // It is the header file that provides access to the POSIX operating system API.
#include <string.h> // It defines one variable type, one macro, and various functions for manipulating arrays of characters.
#include <stdlib.h> // It defines four variable types, several macros, and various functions for performing general functions.
#include "mymemory.h" // It includes the header file which contains C declarations and macro definitions to be shared between several source files.


// Declaration of global variables so they can be accessed by all the functions.
Byte        mymemory [MAXMEM] ;  // mymemory is a large array of bytes of length MAXMEM.

Segment_t * segmenttable = NULL; // Segmenttable is a pointer variable that points to the list of segment descriptors 
                                 // and monitors which segments are free and which one are allocated.
int used_memory = 0;             // Used_memory informs about the amount of used space in mymemory.
int allocated_segments = 0;      // Allocated_segments represents number of segments in the segmentation table that are allocated.
void ** ptrlist [MAXSEGMENTS];    // Ptrlist is a list of the pointers that point to a mymemory address location.


void initialize () 
{  
  // This function initialises the segmentation table and the memory array.

  printf ( "initialize> start\n");

  // This for loop by traversing the array, sets the complete memory which has size of MAXMEM to 0.
  for (int i = 0; i<MAXMEM; i++) { 
    mymemory[i] = '\0';
    i++;
  }
  
  segmenttable = malloc( sizeof( Segment_t ) ); // Allocating memory of the size of type Segment_t
  // Initializing the first and the only one segment of the segmenttable table as for now 
  segmenttable->allocated = 0;      // As nothing is allocated there, "allocated" Boolean that represents 
                                    //if something is allocated is not, is set to 0 (FALSE). 
  segmenttable->start = &mymemory;  // Attribute "start" is set to &mymemory as first segment in segmenttable points to the start of mymemory. 
  segmenttable->size = MAXMEM;      // Attribute "size" is set to MAXMEM as this segment describes the whole memory which size is MAXMEM.
  allocated_segments++;             // After creating the first segment, incrementing the number of allocated segments.
  segmenttable->next = NULL;        // Pointing next to NULL.
  
  printf ( "initialize> end\n");
}


int isPrintable ( int c )
{ 
  /* This function checks if  characters are printable. 
     If a character is printable, then return the character, otherwise return a 0. */ 
  if ( c >= 0x20 && c <= 0x7e ) { 
    return c ;
  }
  return 0 ;
}


void printmemory ()
{ 
  /* This function traverses the memory array “mymemory” and prints the content of each byte. 
  It also checks whether characters are printable by calling function isprintable().
  For any non-prinatble characters print a '.'.
  A convenient form of printing memory is:
  it prints the content of 10 bytes per line in both hexadecimal (“%02x”) and after "|" as a character (“%c”).
  Also it prints the number of the first byte at the beginning of each line. */

  // helping variables 
  int i = 0, first_byte = 0, j, last_byte;  
  for (i = 0; i < MAXMEM; i++){           // for loop through the whole mymemory
    if (i % 10 == 0){                     // if statement that checks if byte divides 10
      first_byte = i;                     // Not to lose track of i, assigning its value to cnt and continuing with cnt instead.
      if (first_byte == 0){               // if yes checks if is equal to 0 
        printf("\n[   %d]",i);            // In order to leave needed space and provide equal printing on the left [   0], instead of just [0].
      } else if (first_byte < 100){       // Checks if it's smaller than 100.
        printf("\n[  %d]",i);
      } else if (first_byte < 1000){      // Checks if it's smaller than 1000.
        printf("\n[ %d]",i);
      } else {                      
        printf("\n[%d]",i);               // Not leaving extra space cause it is known that cnt >= 1000.
      }   
    last_byte = first_byte + 10;          // Assigning value to last byte to print by adding 10 to the first byte.
    while (first_byte < last_byte && MAXMEM != first_byte){    // Printing row of 10 bytes in a hexadecimal format.
      printf(" %02x", mymemory[first_byte]);                   // Using “%02x” to print hexadecimal format.
      first_byte++;
    } 
    printf(" | ");                         // Printing "|" to split hexadecimal form from characters form.
    }                                      // Finish of printing hexadecimal format and end of if statement.    
    if (isPrintable (mymemory[i]) == 0) {  // Checking if character is printable by calling isPrintable.
      printf(".");                         // If function isPrintable returns 0 then it is not Printable and prints "."
    } else {
      printf("%c", mymemory[i]);          // Otherwise prints character.
    }    
  }                                       // End of for loop traversing through mymemory
  printf("\n\n");                         // Prints an empty line.
} 


void printsegmenttable()
{ 
  /* This function traverses the segmentation table and prints out the information of each segment descriptor.
    Prints information if segment is allocated, its size and starting point. */ 

  Segment_t * descriptor = segmenttable;               // Creating a pointer to segmenttable.
  
  while (descriptor != NULL) {                         // While segment is not equal to NULL
    printf ("Segment:\n" ) ;
    printf ( "\tallocated = %s\n" , (descriptor->allocated == FALSE ? "FALSE" : "TRUE" ) ) ;  // Printing if allocated 
    printf ( "\tstart     = %p\n" , descriptor->start ) ;                                     // Printing starting point
    printf ( "\tsize      = %lu\n", descriptor->size  ) ;                                     // Printing size
    printf("\n");
    descriptor = descriptor->next; 
  }
} 


Segment_t * findFree ( Segment_t * list, size_t size )
{
  /*This function searches for a segment in the list that is free (allocated == FALSE), 
  and is at least as large as the required size expressed by parameter “size”. */ 
  
  Segment_t * segments = list;                    // Creating a pointer to list 
  
  while (segments != NULL) {                      // Checking if pointer is not equal to NULL
    if ( segments->allocated == 0 && segments->size >= size ) {  // Checking if segment to which pointer points is not allocated and big enough. 
      return segments;                                           // If yes then returning this pointer
  }
  segments = segments->next;                                     // If not pointing to next one to check next one
  }
  return NULL;                                                   // If no segment found return NULL.
}


Segment_t * findSegment ( Segment_t * list, void * ptr )
{ 
  /* This function searches for a segment in the list where list->start == ptr 
  If that ptr doesn not exist in a list, it returns NULL. */  
  
  Segment_t * current = list;       // Creating a pointer to list  
  
  while (current != NULL) {         // Start of while loop
    if (current->start == ptr) {
      return current;               // If wanted segment found, returns it
    }
    current = current->next;        // If not points to next segment
  }
  return NULL;
}


void * mymalloc ( size_t size )
{ 
  /* This function “allocates” memory of a particular size and returns a pointer to the first byte of the allocated segment in mymemory.
     Returns a pointer to a location in the memory array. This function is used in the same way as the C function malloc(). */
  
  printf ( "mymalloc> start\n");  

  if (used_memory + size > MAXMEM || allocated_segments > MAXSEGMENTS){ // Checking if there is free space in memory 
                                                                        // and if numbers of segments was not exceed.
    if (used_memory + size > MAXMEM){       // Checking if there is space in mymemory
      printf("No free memory to allocate segment! Maximum capacity of memory is %d \n", MAXMEM);
    }
    if (allocated_segments < MAXSEGMENTS) {           // Checking if number of allocated segments is smaller than maximum number of segments.                  
      printf("Numbers of segments was reached!\n");   // If not then allocating is impossible.
    }   
  return NULL; 
  }
  else {
    // Searching for a segment (seg_first) in the list that is free (allocated == FALSE), 
    // and is at least as large as the required size expressed by parameter “size”.
    Segment_t * seg_first = findFree(segmenttable,size); 
    
    if (seg_first == NULL) {                              
      printf("Segment of desired size is not availavle!\n");
      return NULL;                  // If such a segment does not exist then it returns NULL.
    }
    else {     
      // Checking if found segment is the same size as memory to allocate to prevent creating segment of size 0                     
      if (seg_first->size == size) {
        seg_first->allocated = 1;          // Setting allocated to 1 as this segment is allocated now.
        used_memory += size;               // Adding to used memory the size of added memmory.
        return seg_first->start;           // Return that it points to the start
        allocated_segments++;
      } 
      else {                               // If size of memory to allocate is smaller than size of the segment:
        used_memory += size;               // Adding to used memory the size of added memory.
        Segment_t * sub_second ;           // Declaration of second sub-segment.
        allocated_segments++;              // As new segment was created, incrementing  the number of allocated segments. 
        sub_second = malloc(sizeof(Segment_t)); // Allocates memory with the wanted size. 
        sub_second->allocated = 0;                      // Setting allocated to 0 as there is nothing allocated there.
        sub_second->size = seg_first->size - size;      // Setting size to the difference between size of the first initial segment 
                                                        // and size of allocated memory there.
        sub_second->start = seg_first->start + size;    // Specifying the memory address by pointing to the address of start of the previous segment 
        sub_second->next = NULL;                        // plus size of this segment.
        seg_first->allocated = 1;                               // Setting allocated to 1 as this segment is allocated now.
        seg_first->size = size;                                 // Setting size to the size of allocated memory.
        insertAfter(seg_first, sub_second);
        //seg_first->next = sub_second;                 // Setting next as a new created segment.
        return seg_first->start;                        // Return that it points to the start.
      }
    }
  }
}


void insertAfter ( Segment_t * oldSegment, Segment_t * newSegment )
{ 
  /* This function inserts new segment descriptors after an existing segment descriptor. */

  if (oldSegment->next != NULL) {       // Checking if next segment after existing one is not  NULL.
    Segment_t * curr;                   // Creating a pointer.
    curr = oldSegment->next;       
    oldSegment->next = newSegment;      // Pointing existing segment to new one.
    newSegment->next = curr;            // Pointing new segment to previous next segment of existing one.
  }
  else {
    oldSegment->next=newSegment;        //If next is NULL then pointing existing segment to the new one .
  }
}


void myfree ( void * ptr ) 
{ 
  /*The function myfree ( void * ptr) takes a pointer as parameter, “ptr”. 
  This is the pointer that points to a location in your memory array “mymemory”, where the allocated segment starts. */

  printf ( "myfree> start\n");

  Segment_t * seg_found;                                // Declaring the pointer.
  seg_found = findSegment (segmenttable, ptr);          // Searching in a segmentable for that pointer.
  
  if (seg_found->start == ptr) {                        // If the pointer found.
    void * start_to_free;             
    size_t size_of_segment; 
    size_of_segment = seg_found->size;                  // Size of the segment to free.  
    start_to_free = seg_found->start;                   // Points to a location in memory array “mymemory” where segment to free starts.
    int a = 1;
    allocated_segments-=1;

    while (a <= size_of_segment){                       // While loop that frees allocated memory using strcpy().
      strcpy(start_to_free, "\0");
      start_to_free++;                                  // Moving to next byte
      a++;                                              // Increasing a by 1 to eventually break while loop.
    }

    used_memory -= seg_found->size;                     // Substacting the size of deleted segment from the used memory.
    seg_found->allocated = 0;                           // As this segment is no longer allocated, setting 'allocated' to 0.
    for (int i=0;i<allocated_segments;i++){             // Traversing through ptrlist to find ptr .
      if (ptrlist[i]==ptr){
        ptrlist[i]=NULL;                                // When found, assigning it to NULL.
   }
  }
  }
}


// Implementation of moveSegment helper function to manage segmenation table as a linked list.
void * moveSegment ( Segment_t * list, void * ptr ) 
{
    /*This function moves a segment within the memory array (the content in the memory has to be moved as well. 
    This function returns the new “start” pointer of the segment, or NULL in case of a failure situation.
    Takes ptr as an argument.*/

    int found = 0;                              // Helping variable to break while loop when needed.

    // Seg_to_find is a free segment before allocated segment to move.   
    Segment_t * seg_to_find = segmenttable;     // Creating a pointer to segmenttable.

    // This while loop traverses through segmenttable until start of the next segment is equal to ptr.
    while(seg_to_find != NULL && found==0){     // Breaking conditions.
      if (seg_to_find->next != NULL ){          // Checking if next segment is not equal to NULL.
      if (seg_to_find->next->start==ptr){       // Checking if next segment points to the same memory as ptr.
        found=1;                                // If yes, assining found=1 to break while loop.
      }
      else {                                    // If not, traversing more
        seg_to_find=seg_to_find->next;          // by pointing to next segment.
      }
    }
    }
    if (seg_to_find->allocated==TRUE) {         // Segment not found = failure situation
        return NULL;                            // Function return NULL
    }

    Segment_t * allocated_after_stf;            // Creating a pointer 
    allocated_after_stf = seg_to_find->next;
    // Allocated_after_stf is a segment that points to the same allocated memory as ptr 
    // (before assigning to ptr = seg_to_find->start;).
    // This is the allocated segment after not allocated segment seg_to_find.

    // Copying the content of memory where ptr points and placing it 
    // where seg_to_find starts (so the segment that is free/not allocated)
    // before the allocated (allocated_after_stf) segment.
    strcpy(seg_to_find->start, ptr);

    // Setting mymemory to 0 where was previously allocated segment 
    // by using as arguments ptr that still points there and size of allocated_after_stf.
    memset(ptr, '\0', allocated_after_stf->size);

    // Pointing ptr to new location of memory content so 
    // to the previously (no longer) free/not allocated segment.
    ptr = seg_to_find->start;

    // Updating starting points of segments
    int sizeOfAllocated = allocated_after_stf->size;       // Keeping value of size of allocated segment that wil be moved.
    allocated_after_stf->allocated = 0;                    // As memory content was moved it's no longer allocated 
                                                           // so attribute allocated is seto to 0.
    allocated_after_stf->size = seg_to_find->size;         // Updating size of allocated_after_stf segment.
    seg_to_find->allocated = 1;                            // As memory content was put to previously free segment
                                                           // attribute allocated is set to 1.
    seg_to_find->size = sizeOfAllocated;                   // Updating size of previously free segment.
    return seg_to_find->start;                             // Returns new "start" pointer of the segment where allocated memory is
    }


// Implementation of delSegment helper function to manage segmenation table as a linked list.
int delSegment ( Segment_t * list, Segment_t * segment ) 
{
  /*This function searches for a segment in the segmentation table 
  and deletes this segment descriptor. 
  Returns 0 if no such segment exists, 1 otherwise.*/

  if (segment != NULL) {                     // If segment put as an argument is not equal to NULL then working on it.   
    int free_mem;                            // Variable to store free available memory in the program.
    free_mem = MAXMEM-used_memory;           // Counting this free memory by substacting allocated memory 
                                             // from the size of the whole memory.

    Segment_t *seg = list;                   // Creating a pointer that points to the list (segmenttable).
	  int found=0;                             // Helping variable to break while loop.

 	while(found==0 && seg->next != NULL){      // While loop to search for a free/not allocated segment that is before 
                                             // the one that was put as a variable.
		if (seg->next->start==segment->start){   // If found:
			found = 1;                             // assigning found=1 to break while loop.
  }
		else {                                   // If not keeping traversing through segmenttable
			seg=seg->next;	                       // by pointing to next segment.
  }
  }

  // Allocating the whole size to one segment I delete remain segment descriptors 
  // not of only one but all off segments in one move.
  seg->size = free_mem;                     // Attribute size is set to free_mem because now there is only one free segments 
                                            // that has size of the whole left free space in mymemory.
  seg->allocated=FALSE;                     // It is not allocated so assigning allocated to FALSE.
  seg->next = NULL;                         // Setting attribute next to NULL as this is the last segment so next one is NULL.
  return 1;                                 // Returns 1 as everything went successfully.
  }
	else {                                    // If segment put as an argument was NULL 
	  return 0;                               // then returns 0.
}
}


void mydefrag (void ** ptrlist) 
{
    /* This function defragments the memory and compats smaller free segments into larger one.
    Manipulates using helping functions segmentation list. Redirects the start pointers 
    in the segment descriptors of the segmentation table and the pointer variables.
    Takes as a parameter a list of pointers.*/

    Segment_t * segments = segmenttable;              // Creating a pointer to segmenttable

    int all = 0;                                      // Helping variable to break loop when needed
    //int i =0;                                         
    int allocated_in_row;                             // Variable saying how many allocated segments is in a row
    while (all==0) {                                  // Main while loop that goes through the segmenttable
                                                      // until the whole segmenttable and mymemory are defragged. 

        allocated_in_row = 0;                         // As no allocated was found value of it is 0.
        segments = segmenttable;                      // Pointing to segmenttable.

      while(segments->next!= NULL) {                  // Second while loop that goes through the segmenttable
                                                      // and moves each allocated segment once to a free one. 
      if (allocated_in_row==allocated_segments) {     // If number of allocated segments in a row is equal  
                                                      // to number of all allocated segments:
          all=1;                                      // Assigning all to 1 to finish the main while loop.
          break;                                      // Breaking second while loop
      }

      else if (segments->allocated==TRUE) {           // If segment allocated:
        segments=segments->next;                      // pointing to next one
        allocated_in_row +=1;                         // and adding one to allocated segments in a row.
      }
      
      // If segments is not allocated:
      else {
        Segment_t * next_segment;                     // Creating a pointer 
        next_segment=segments->next;                  // that points to the next segment.
        
        // While loop that traverses segmenttable until allocated segment is found.
        while (next_segment->allocated==FALSE && next_segment->next != NULL){
          next_segment=next_segment->next;
        }
        // Allocated segment found.
        // Checking if is not equal to NULL 
        if (next_segment != NULL) {                           // If allocated segment is not equal to NULL:
          moveSegment(segmenttable,next_segment->start);      // moving this segment up.
                                                              // Using ptr as a parameter.                                       
          // To call with segment as parameter:
          //moveSegment(segmenttable,next_segment);
        }
        segments=segments->next;                              // Pointing to the next segment.
      }
    }
    }
    Segment_t * first_free;                                   // Creating a pointer
    first_free = segmenttable;                                // that points to segmenttable.

    for (int i=0;i<allocated_segments-1;i++){                 // For loop that points to next segment 
                                                              // until not allocated is found.
        first_free = first_free->next;                        // As all allocated segments are now up in my memory 
                                                              // and segmenttable, it does it (pointing to next segment)
                                                              // in a for loop allocated_segments times.
      }
    delSegment(segmenttable, first_free->next);               // Calling delSegment function on the second free segment to 
                                                              // deletes this segment descriptor. 
                                                              // As a parameter uses segment. 
    // This functions uses ptr as a parameter. 
    //delSegment(segmenttable, first_free->next->start);
 }




// Second approaches of helping functions for mydefrag:

// Implementation of delSegment (second approach) helper function to manage segmenation table as a linked list.
/*
int delSegment(Segment_t * list, void * ptr) {
  // This function searches for a segment in the segmentation table 
  // and deletes this segment descriptor. 
  // Returns 0 if no such segment exists, 1 otherwise.

	Segment_t *seg = list;                         // Creating a pointer that points to the list (segmenttable).
	int found=0;                                   // Helping variable to break while loop.
	
  while(found==0 && seg->next != NULL){          // While loop to search for a free/not allocated segment that is before 
                                                 // the one that was put as a variable.
		if (seg->next->start==ptr){                  // If found:
			found = 1;                                 // assigning found=1 to break while loop.
  }
		else {                                       // If not keeping traversing through segmenttable
			seg=seg->next;	                           // by pointing to next segment.
  }
  }
  
  int free_mem;                                  // Variable to store free available memory in the program.
  free_mem = MAXMEM-used_memory;                 // Counting this free memory by substacting allocated memory 
                                                 // from the size of the whole memory.

  seg->size = free_mem;                          // Attribute size is set to free_mem because now there is only one free segments 
                                                 // that has size of the whole left free space in mymemory.

  seg->allocated=FALSE;                          // It is not allocated so assigning allocated to FALSE.
  seg->next = NULL;                              // Setting attribute next to NULL as this is the last segment so next one is NULL.
}*/



// Implementation of moveSegment (second approach) helper function to manage segmenation table as a linked list.

/*void * moveSegment ( Segment_t * list, Segment_t * segment ){
    // This function moves a segment within the memory array (the content in the memory has to be moved as well. 
    // This function returns the new “start” pointer of the segment, or NULL in case of a failure situation.
    // Takes ptr as an argument.

    int found = 0;                                        // Helping variable to break while loop when needed.
    Segment_t * seg_to_find = segmenttable;               // Creating pointer to segmenttable.

    // This while loop traverses through segmenttable until start of the next segment is equal to start of the segment put as an argument.
    while(seg_to_find != NULL && found==0){               // Breaking conditions.
      if (seg_to_find->next != NULL ){                    // Checking if next segment is not equal to NULL.
      if (seg_to_find->next->start==segment->start){      // Checking if next segment points to the same memory as segment put as an argument.
        found=1;                                          // If yes, assining found=1 to break while loop.
      }
      else {                                              // If not, traversing more
        seg_to_find=seg_to_find->next;                    // by pointing to next segment.
      }
    }
    }
    if (seg_to_find->allocated==TRUE) {                   // Segment not found = failure situation
        return NULL;                                      // Function return NULL
    }

    Segment_t * allocated_after_stf;                      // Creating a pointer  
    // Allocated_after_stf is a segment that points to the same allocated memory as segment put as an argument
    // This is the allocated segment after not allocated segment seg_to_find.                 
    allocated_after_stf = seg_to_find->next;
    
    int p;                                                // Creating a variable.

    for (int i=0;i<MAXSEGMENTS;i++){                      // Traversing by for loop through the ptrlist 
                                                          // to find a ptrlist[p] that points to the same memory address as segment.
      if (ptrlist[i]==segment->start){                    // If ptrlist[i] points to the same address as segment:
        p = i;                                            // then assign p to i.
        break;                                            // break for loop.
      }                                        
    }

    ptrlist[p] = seg_to_find->start;                      // Updating address where ptrlist[p] should point

    allocated_after_stf->start = seg_to_find->start+allocated_after_stf->size;    // Updating starting point of allocated_after_stf segment 
                                                                                  // (the one that was allocated before and swapped with free one).

    seg_to_find->allocated= 1;                                                    // As memory content was put to previously free segment
                                                                                  // attribute allocated is set to 1.

    allocated_after_stf->allocated=0;                                             // As memory content was moved it's no longer allocated 
                                                                                  // so attribute allocated is seto to 0.

    int sizeOfAllocated = allocated_after_stf->size;                              // Keeping value of size of allocated segment that wil be moved.
    allocated_after_stf->size = seg_to_find->size;                                // Updating size of allocated_after_stf segment.
    seg_to_find->size = sizeOfAllocated;                                          // Updating size of previously free segment.

    //seg_to_find->allocated = TRUE ;
    //allocated_after_stf->allocated=FALSE; 

    int start_of_fs;                                                              // Variable created to store there where allocated_after_stf
                                                                                  // (the free one before) starts in my memory.
    for (int c=0; c < MAXMEM; c++){                                               // For loop to traverse through mymemory
      if (&mymemory[c] == allocated_after_stf->start) {                           // If wanted address found:
        start_of_fs = c;                                                          // Assigning start_of_fs to c.
        break;                                                                    // Breaking for loop 
        }
    }

    int allocated_now;                                                            // Variable created to store there where seg_to_find 
                                                                                  // starts in mymemory.                                                      
    for (int z=0; z < MAXMEM; z++){                                               // For loop to traverse through mymemory
      if (&mymemory[z] == seg_to_find->start) {                                   // If wanted address found:
        allocated_now = z;                                                        // Assigning start_of_fs to c.
        break;                                                                    // Breaking for loop 
        }
    }

    for (int a=0; a<sizeOfAllocated; a++){                                        // For loop to update mymemory content 
                                                                                  
      mymemory[allocated_now] = mymemory[start_of_fs];                            // Putting content of the segment from previous location 
                                                                                  // to current location.
      allocated_now++;                                                            // Pointing to next byte.
      start_of_fs++;                                                              // Pointing to next byte.
    }

    // Do i need to put 0s?
    // for (int a=0; a<sizeOfAllocated; a++){
    //  mymemory[allocated_now] = mymemory[start_of_fs];
    //  allocated_now++;
    //  start_of_fs++;
    //}
    return seg_to_find->start;                              // Returns new "start" pointer of the segment where allocated memory is
}*/
