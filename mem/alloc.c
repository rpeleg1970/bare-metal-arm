#include "alloc.h"

extern unsigned int heap_start;
extern unsigned int ram_end;

void *malloc(size_t nbytes)
{
  static void *next_chunk = &heap_start;
  static void *heap_end = &ram_end;

  void *ret = next_chunk;
  next_chunk += nbytes;
  return ret;
}

void free(void *ptr)
{
}

void *calloc(size_t nmemb, size_t size)
{
  size_t nbytes = nmemb*size;
  void *ret = malloc(nbytes);
  while(--nbytes>0)
    ((char*)ret)[nbytes]=0;

  return ret;
}

void *realloc(void *ptr, size_t size)
{
  return NULL;
}
