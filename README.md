# Memory_Management
Implementation of own version of malloc() in C.

Implemented my own simplified version of the function malloc() that allows to allocate memory segments, my version of function free() to deallocate these segments, and a function defrag() that defragments my “mymemory”. Implementing using array for mymemory with limited memory capacity which I divided into smaller segments. I keep track of segments using pointer segmenttable that has attributes allocated, *start, size, *next and works like a linked list. 
