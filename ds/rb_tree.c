/*
 * Red Black tree implementation based on Julienne Walker's tutorial
 * http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx.
 * JSW RB tree benefits: 
 * - Insertion/deleion is top down and hence recursion-less. 
 * - The code is more compact due to decoded direction, which allows the same code to
 *   handle symetric cases on left and right.
 */

#include "rb_tree.h"
#include "io/stdio.h"

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

rbt_node *rbt_find_ex(rbt_node* root,unsigned int key, int op)
{
  rbt_node *ret = NULL;

  if(root) {
    if(root->key == key)
      return root;

    int dir = key > root->key;
    ret = rbt_find_ex(root->link[dir],key,op);

    if(!ret && op!=RBT_EQ) 
    {
      /* this means we did not find an exact match, so try GT or LT */
      if((dir == 1 && op == RBT_LTEQ) || (dir == 0 && op == RBT_GTEQ))
        return root;
    }
  }

  return ret;
}

rbt_node *rbt_find(rbt_node* root, unsigned int key)
{
  return rbt_find_ex(root,key,RBT_EQ);
}

rbt_node *rbt_insert ( rb_tree *tree, rbt_node*(*make_node)(unsigned int), unsigned int key )
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

int rbt_remove ( rb_tree *tree, void(*free_node)(rbt_node*), unsigned int key )
{
  rbt_node *f = NULL;  /* Found item */
  if ( tree->root != NULL ) {
    rbt_node head = {0}; /* False tree root */
    rbt_node *q, *p, *g; /* Helpers */
    int dir = 1;

    /* Set up helpers */
    q = &head;
    g = p = NULL;
    q->link[1] = tree->root;

    /* Search and push a red down */
    while ( q->link[dir] != NULL ) {
      int last = dir;

      /* Update helpers */
      g = p, p = q;
      q = q->link[dir];
      dir = q->key < key;

      /* Save found node */
      if ( q->key == key )
        f = q;

      /* Push the red node down */
      if ( !is_red ( q ) && !is_red ( q->link[dir] ) ) {
        if ( is_red ( q->link[!dir] ) )
          p = p->link[last] = rotate_single ( q, dir );
        else if ( !is_red ( q->link[!dir] ) ) {
          rbt_node *s = p->link[!last];

          if ( s != NULL ) {
            if ( !is_red ( s->link[!last] ) && !is_red ( s->link[last] ) ) {
              /* Color flip */
              p->red = 0;
              s->red = 1;
              q->red = 1;
            }
            else {
              int dir2 = g->link[1] == p;

              if ( is_red ( s->link[last] ) )
                g->link[dir2] = rotate_double ( p, last );
              else if ( is_red ( s->link[!last] ) )
                g->link[dir2] = rotate_single ( p, last );

              /* Ensure correct coloring */
              q->red = g->link[dir2]->red = 1;
              g->link[dir2]->link[0]->red = 0;
              g->link[dir2]->link[1]->red = 0;
            }
          }
        }
      }
    }

    /* Replace and remove if found */
    if ( f != NULL ) {
      f->key = q->key;
      p->link[p->link[1] == q] =
        q->link[q->link[0] == NULL];
      (*free_node)(q);
    }

    /* Update root and make it black */
    tree->root = head.link[1];
    if ( tree->root != NULL )
      tree->root->red = 0;
  }

  return 1;
}

/*
 * Testing and dump functions from here below
 */
void dump_node_dot(rbt_node *node, rbt_node *parent)
{
    /* dump self */
    _uart0_prints("\tnode [style=filled fillcolor=");
    _uart0_prints(node->red ? "red]; \"" : "black]; \"");
    _uart0_printi(node->key);
    _uart0_prints("\"\n");

    /* edge from parent to self */
    if(parent)
    {
      _uart0_prints("\t\"");
      _uart0_printi(parent->key);
      _uart0_prints("\" -> \"");
      _uart0_printi(node->key);
      _uart0_prints("\"\n");
    }

    /* recurse */
    if(node->link[0])
      dump_node_dot(node->link[0],node);
    if(node->link[1])
      dump_node_dot(node->link[1],node);

}

void rbt_dump_dot(rb_tree *tree)
{
  _uart0_prints("digraph RBT {\n");
  _uart0_prints("\tnode [fontsize=11 fontcolor=white; fontname=Helvetica];\n");
  _uart0_prints("\tedge [arrowhead=vee];\n");

  if(tree->root)
    dump_node_dot(tree->root, NULL);

  _uart0_prints("}\n");
}

rbt_node *rbt_new_node(unsigned int key)
{
  rbt_node* retval = (rbt_node*)malloc(sizeof(rbt_node*));
  retval->key = key;
  retval->red = 1;
  retval->link[0] = retval->link[1] = NULL;
  return retval;
}

void rbt_free_node(rbt_node *node)
{
  free(node);
}

void rbt_test()
{
  rb_tree tree;
  unsigned int i = 0;
  tree.root = NULL;

  for(i=0; i<32; i++)
    rbt_insert(&tree,&rbt_new_node,i);
  for(i=0; i<32; i+=2)
    rbt_remove(&tree,&rbt_free_node,i);

  rbt_dump_dot(&tree);
}

