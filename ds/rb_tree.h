#ifndef __jsw_rb_tree_h_
#define __jsw_rb_tree_h_

/*
 * Red Black tree implementation based on Julienne Walker's tutorial
 * http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx.
 * JSW RB tree benefits: 
 * - Insertion/deleion is top down and hence recursion-less. 
 * - The code is more compact due to decoded direction, which allows the same code to
 *   handle symetric cases on left and right.
 *
 * Differences implemented here, keeping in mind this is mostly developed for
 * the malloc implementation:
 * - A node contains the data which is the sorting value, but also needs
 *   a payload which will be a pointer to something (e.g. next block in free
 *   list).
 * - As I wnated the node to fit in the minimal block malloc provides, the node
 *   is limited to 16 bytes. This means that we need to find room for the color
 *   indicator. So by design I limited the value of 'data' to the first 31 bits,
 *   and use the MSB as a color indicator. An alternative could be allowing the
 *   calling code to extend the basic struct(same fields + color, and maybe
 *   more), as well as pass getter/setter for color and 'data'.
 * - Also placing the 'data' union  in the first 4 bytes,
 *   payload in the next 4 bytes, to match the free list node structure.
 */



#endif
