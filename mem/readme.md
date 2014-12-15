implement basic heap management: malloc, free, later on realloc. Some points:
- the CPU does not support protected mode - so all memory is accessed in real mode
- Connect board RAM is 0xA0000000 to 0xA3FFFFFF (64MB). Part is allocated for
  stack and static variables (see starup.S), the rest is managed here.

Managing the remaining memory is done along the following guidelines:
- we do not have an OS and VMM so newly allocated memory is taken starting from
  the heap base, and going up. A single pointer tracks the next location, which we will call 'uncharted'
- Allocating new memory is aligned to (configurable) 16 bytes
  - every allocated chunk has at least the size (4 bytes in this architecture) attached below the first usable chunk byte. So this should be considered in the overall size (so up to 12 bytes we still allocate a 16 byte chunk)
- Freed blocks are tracked in the following manner:
  - We keep a red-black tree for tracking blocks by size. Algorithm is derived
    from http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
  - each node contains a linked list of memory chunks of the same (aligned) size
  - When malloc is called, we first look for a best fit (see below) in the free list. If we do not find - we allocate from the next uncharted (see above). If we do find, we take the first free block in the list and return it.
    - TBD how much bigger we are willing to provide as best-fit
    - the  tree find should be augmented to return equal-or-first-larger (instead of just equal)
  - if the list for a specific size goes down to 0 - we also remove the RB tree node


Now the above algorithm needs to be handled without the overhead of keeping a separate data structure, (a) because we want to remain lean and (b) we implement basic memory allocation, so the data structure cannot rely on an existing heap.
This is done as follows:
- each freed block is at least 16 bytes - so we can use the first few bytes in
  it, and erase them when we provide the block via malloc
- If the chunk is the first of its size in the tree, we arrange the data in it as follows:
  - 4 bytes size (already there, from malloc)
  - 4 bytes pointer to next chunk (linked list ptr)
  - 4 bytes left ptr
  - 4 bytes right ptr
  - We also need a color bit, which we cannot fit if the chunk was 16 bytes. HOWEVER - since the the size is aligned to 16 bytes, we can use any of the 4 LSB in the LS byte (first byte of chunk, as connex is little-endian)
- When any additional chunck comes in (same size) - we copy the 'next' pointer from the RB node to its 'next' pointer (bytes 5-8), and correct the RB node 'next' pointer to point to it. This effectively pushes the new chunk in o(1) into the list (adds to the o(log(n)) of rb tree traversal)
- to remove a chunck for malloc we perform the reverse, and when we get to the point
  where the list is empty except the rb node - we remove it from the tree completely, erase the info and pointers, and return it.
