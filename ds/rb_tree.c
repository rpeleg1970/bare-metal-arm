/*
 * Red Black tree implementation based on Julienne Walker's tutorial
 * http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx.
 * JSW RB tree benefits: 
 * - Insertion/deleion is top down and hence recursion-less. 
 * - The code is more compact due to decoded direction, which allows the same code to
 *   handle symetric cases on left and right.
 */

#include "rb_tree.h"

int is_red(rbt_node *node)
{
  return node!=NULL && node->red == 1;
}

rbt_node *rotate_single(rbt_node *root, int dir)
{
  rbt_node *save = root->link[!dir];

  /* the child node opposite the rotate dir, becomes the parent */
  root->link[!dir] = save->link[dir];
  save->link[dir] = root;

  /* and colors are set */
  root->red = 1;
  save->red = 0;

  return save;
}

rbt_node * rotate_double(rbt_node *root, int dir)
{
  root->link[!dir] = rotate_single(root->link[!dir], !dir);
  return rotate_single(root,dir);
} 

rbt_node *find_ex(rbt_node* root,unsigned int key, int op)
{
  rbt_node *ret = NULL;

  if(root) {
    if(root->key == key)
      return root;

    int dir = key > root->key;
    ret = find_ex(root->link[dir],key,op);

    if(!ret && op!=RBT_EQ) 
    {
      /* this means we did not find an exact match, so try GT or LT */
      if((dir == 1 && op == RBT_LTEQ) || (dir == 0 && op == RBT_GTEQ))
        return root;
    }
  }

  return ret;
}

rbt_node *find(rbt_node* root, unsigned int key)
{
  return find_ex(root,key,RBT_EQ);
}

rbt_node *insert ( rb_tree *tree, rbt_node*(*make_node)(unsigned int), unsigned int key )
{
  rbt_node *retval = NULL;
  if ( tree->root == NULL ) {
    /* Empty tree case */
    retval = tree->root = (*make_node)( key );
    if ( retval == NULL )
      return retval;
  }
  else {
    rbt_node  head = {0}; /* False tree root */

    rbt_node *g, *t;     /* Grandparent & parent */
    rbt_node *p, *q;     /* Iterator & parent */
    int dir = 0, last;

    /* Set up helpers */
    t = &head;
    g = p = NULL;
    q = t->link[1] = tree->root;

    /* Search down the tree */
    for ( ; ; ) {
      if ( q == NULL ) {
        /* Insert new node at the bottom */
        retval = p->link[dir] = q = (*make_node)( key );
        if ( retval == NULL )
          return retval;
      }
      else if ( is_red ( q->link[0] ) && is_red ( q->link[1] ) ) {
        /* Color flip */
        q->red = 1;
        q->link[0]->red = 0;
        q->link[1]->red = 0;
      }

      /* Fix red violation */
      if ( is_red ( q ) && is_red ( p ) ) {
        int dir2 = t->link[1] == g;

        if ( q == p->link[last] )
          t->link[dir2] = rotate_single ( g, !last );
        else
          t->link[dir2] = rotate_double ( g, !last );
      }

      /* Stop if found */
      if ( q->key == key ) {
        retval = q;
        break;
      }

      last = dir;
      dir = q->key < key;

      /* Update helpers */
      if ( g != NULL )
        t = g;
      g = p, p = q;
      q = q->link[dir];
    }

    /* Update root, if root rotation occured */
    tree->root = head.link[1];
  }

  /* Make root black */
  tree->root->red = 0;

  return retval;
}
