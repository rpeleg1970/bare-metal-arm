#ifndef __alloc_h_
#define __alloc_h_

#define NULL 0x00000000
#define size_t unsigned long

void *malloc(size_t nbytes);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

void dump_mem_dot();
#endif
