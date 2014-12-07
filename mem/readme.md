implement basic heap management: malloc, free, later on realloc. Some points:
- the CPU does not support protected mode - so all memory is access in real mode
- Connect board RAM is 0xA0000000 to 0xA3FFFFFF (64MB). Part is allocated for
  stack and static variables (see starup.S), the rest is managed here.


Managing the remaining memory is done along the following guidelines:
- we do not have an OS and VMM so newly allocated memory is taken starting from
  the heap base, and going up. A single pointer tracks the next location, which we will call 'uncharted'
- Allocating new memory is aligned to (configurable) 16 bytes
- Freed blocks are tracked in the following manner:
  - We keep a red-black tree for tracking blocks by size. Algorithm is derived
    from http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
  - each node contains a linked list of memory chunks of the same (aligned) size
  - When malloc is called, we first look for a best fit (TBD how much bigger we
    are willing to provide) in the free list. If we do not find - we allocate from the next uncharted (see above). If we do find, we take the first free block in the list and return it.
  - if the list for a specific size goes down to 0 - we also remove the RB tree node


Now the above algorithm needs to be handled without the overhead of keeping a separate data structure, (a) because we want to remain lean and (b) we implement basic memory allocation, so the data structure cannot rely on an existing heap.
This is done as follows:
- each freed block is at least 16 bytes - so we can use the first few bytes in
  it, and erase them when we provide the block via malloc
- If the block is the first of its size in the tree, the first 4 bytes are
  dedicated to the RB tree node info: size, left-ptr, right-ptr, and color. The fifth byte is a pointer to the next node that will come in, initially points to NULL.
- When any additional chunck comes in - we copy the rb-node pointer to its first
  byte, and correct the rb-pointer to point to it. This effectively pushes the new chunk in o(1) into the list (adds to the o(log(n)) of rb tree traversal)
- to remove a chunck for malloc we perform the reverse, and when we get to the point
  where the list is empty except the rb node - we remove it from the tree completely, erase the info and pointers, and return it.
