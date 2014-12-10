#include "alloc.h"

/* defined in linker script, actually located in heap-start and ram-end */
extern unsigned int heap_start;
extern unsigned int ram_end;
static unsigned int align = 16; /* must be a power of 2 see malloc */

typedef struct {
  size_t size;
  void* next; /* this one either points to the next free, or is the address returned to the caller */
} chunk_t;

static chunk_t free_list = {.size = 0L, .next = NULL};

void *malloc(size_t nbytes)
{
  static void *next_unmapped = &heap_start;
  void *ret = NULL;
  chunk_t* prev_iter = &free_list;
  chunk_t* iter = free_list.next;

  /* align nbytes, making sure we also add 4 bytes for chunk size info
   * we assume 'align' is a power of 2 so the affect of adding (align-1) and
   * then bitwise NAND (align-1) is to align up */
  nbytes = (nbytes+sizeof(size_t)+(align-1)) & ~(align-1);

  /* first look in free_list (currently first fit, TBI best fit via RB tree) */
  while(iter && iter->size<nbytes)
  {
    prev_iter = iter;
    iter = iter->next;
  }

  if(iter)
  {
    prev_iter->next = iter->next;
    iter->next = 0L;
    ret = &iter->next;
  }
  
  /* if not found - take from next unmapped */
  else if((next_unmapped+nbytes)<(void*)(&ram_end))
  {
    ((chunk_t*)next_unmapped)->size = nbytes;
    ret = &((chunk_t*)next_unmapped)->next;
    next_unmapped += nbytes;
  }
  return ret;
}

void free(void *ptr)
{
  chunk_t *free_ptr = (chunk_t*)(ptr-sizeof(size_t));
  free_ptr->next = free_list.next;
  free_list.next = free_ptr;
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
