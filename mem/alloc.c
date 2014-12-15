#include "alloc.h"
#include "ds/rb_tree.h"

/* defined in linker script, actually located in heap-start and ram-end */
extern unsigned int heap_start;
extern unsigned int ram_end;
static unsigned int align = 16; /* must be a power of 2 see malloc */

// we build the struct so that it aligns with the rbt_node first 8 bytes
typedef struct chunk_t {
  size_t size : 31;
  unsigned int reserved1 : 1;
  struct chunk_t* next; /* this one either points to the next free, or is the address returned to the caller */
} chunk_t;

static rb_tree free_chunks = {.root = NULL};
static chunk_t *free_ptr; /* last freed chunk */

void dealloc_freelist_node(rbt_node *node)
{
  /* the node is actually return to a user via malloc - just clean
   * make sure we clean `red`, but keep `key` which is the size */
  node->red = 0;
  node->link[0] = node->link[1] = NULL;
}

void *malloc(size_t nbytes)
{
  static void *next_unmapped = &heap_start;
  chunk_t *ret = NULL;
  rbt_node *node = NULL;
  chunk_t* clist = NULL;

  /* align nbytes, making sure we also add 4 bytes for chunk size info
   * we assume 'align' is a power of 2 so the affect of adding (align-1) and
   * then bitwise NAND (align-1) is to align up */
  nbytes = (nbytes+sizeof(size_t)+(align-1)) & ~(align-1);

  if(nbytes & 0x80000000)
    return NULL; /* too big anyway, but also avoid values over 31 bits see chunk_t */

  /* first look for the nearest available node */
  node = find_ex(free_chunks.root,nbytes,RBT_GTEQ);
  if(node)
  {
    clist = (chunk_t*)node;
    if(clist->next)
    {
      ret = clist->next;
      clist->next = clist->next->next; /* clipping 2nd element in list */
    } else {
      /* last one from this size, remove from rb_tree */
      ret = clist;
      remove(&free_chunks,&dealloc_freelist_node,node->key);
    }
    ret->next = NULL;
    ret = (void*)&ret->next; /* make sure we retun the ptr above the 'size' */
  }
  
  /* if not found - take from next unmapped */
  else if((next_unmapped+nbytes)<(void*)(&ram_end))
  {
    ((chunk_t*)next_unmapped)->size = nbytes;
    ret = (void*)&((chunk_t*)next_unmapped)->next;
    next_unmapped += nbytes;
  }
  return ret;
}


rbt_node *alloc_freelist_node(unsigned int key)
{
  rbt_node* retval = (rbt_node*)free_ptr;
  if(retval)
  {
    /* size is already set */
    retval->key = key;
    retval->red = 1;
    retval->link[0] = retval->link[1] = NULL;
  }
  return retval;
}

void free(void *ptr)
{
  free_ptr = (chunk_t*)(ptr-sizeof(size_t));
  rbt_node *node = find(free_chunks.root,free_ptr->size);
  if(!node) /* this chunk's size is not yet in RB tree */
  {
    node = insert(&free_chunks,&alloc_freelist_node,free_ptr->size);
    ((chunk_t*)node)->next = NULL;
    /* this is enough, the node is also the first of the free chunk list for
     * this size */
  } else {
    /* push the free_ptr as the 2nd in the linked list, denoted by the node */
    chunk_t *list = (chunk_t*)node;
    free_ptr->next = list->next;
    list->next = free_ptr;
  }
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

void dump_mem_dot()
{
  dump_dot(&free_chunks);
}
