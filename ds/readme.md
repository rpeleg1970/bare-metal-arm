# Data Structures
## Red Black Tree

Red Black tree implementation based on Julienne Walker's tutorial
http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx.
JSW RB tree benefits: 
- Insertion/deleion is top down and hence recursion-less. 
- The code is more compact due to decoded direction, which allows the same code to
  handle symetric cases on left and right.

Differences implemented here, keeping in mind this is mostly developed for
the malloc implementation:
- A node contains the key which is the sorting value, but also needs
  a payload which will be a pointer to something (e.g. next block in free
  list).
- As I wanted rbt_node to fit in the minimal block malloc provides, the node
  is limited to 16 bytes. This means that we need to find room for the color
  indicator. So by design I limited the value of 'key' to the first 31 bits,
  and use the MSB as a color indicator. An alternative could be allowing the
  calling code to extend the basic struct(same fields + color, and maybe
  more), as well as pass getter/setter for color and 'key'.
- Also placing the 'key'/color values  in the first 4 bytes,
  payload in the next 4 bytes, to match the free list node structure.
- The find function allows us to return the next GT or next LT if EQ
  was not found. An example is our malloc usage - get the next chunk
  size if exact match was not found.
- The insert function receives a make_node() function pointer to allow the
  caller to control how nodes are created. Again - useful for malloc impl.
  The remove function receives similar for delete.

Two more additional functions are provided: `test()` and `dump_dot()`. The latter provides a graphviz `DOT` dump to stdout,
so you can copy this dump into a `.dot` file and execute e.g. `dot -Tpng test.dot > test.png` 

Following is an example run of rb_tree/test():

![alt text](https://raw.githubusercontent.com/rpeleg1970/bare-metal-arm/rbt-mallloc/ds/images/rb.png
 "test run, rb_tree")
