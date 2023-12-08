/*****************************************************************************
 *
 * Module: opt_sub
 *
 * Prefix: OS
 *
 * Description:
 *
 * This module implements a demo traversal of the abstract syntax tree that 
 * replaces subtractions with identical left and right argument by zeros.
 *
 *****************************************************************************/


#include "strength_reduction.h"

#include "node_basic.h"
#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"
#include "copy.h"

#include "memory.h"
#include "free.h"
#include "str.h"



/*
 * Traversal functions
 */

node *SRbinop(node *arg_node, info *arg_info)
{
  DBUG_ENTER("OSbinop");

  /*
   * Extremely important:
   *  we must continue to traverse the abstract syntax tree !!
   */
  BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);
  BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);

  if ( BINOP_OP( arg_node) == BO_mul) {
    if (( NODE_TYPE( BINOP_LEFT( arg_node)) == N_num)
    &&  ( NUM_VALUE( BINOP_LEFT( arg_node)) == 2)
    &&  ( NODE_TYPE( BINOP_RIGHT( arg_node)) == N_var)) { // check for pattern 2*k with a variable k
      BINOP_OP( arg_node) = BO_add;
      BINOP_LEFT( arg_node) = FREEdoFreeTree( BINOP_LEFT( arg_node));
      BINOP_LEFT( arg_node) = COPYdoCopy( BINOP_RIGHT( arg_node)); // transform into k + k
    }
    if (( NODE_TYPE( BINOP_RIGHT( arg_node)) == N_num)
    &&  ( NUM_VALUE( BINOP_RIGHT( arg_node)) == 2)
    &&  ( NODE_TYPE( BINOP_LEFT( arg_node)) == N_var)) { // check for pattern k*2 with a variable k
      BINOP_OP( arg_node) = BO_add;
      BINOP_RIGHT( arg_node) = FREEdoFreeTree( BINOP_RIGHT( arg_node));
      BINOP_RIGHT( arg_node) = COPYdoCopy( BINOP_LEFT( arg_node)); // transform into k + k
    }
    if (( NODE_TYPE( BINOP_LEFT( arg_node)) == N_num)
    &&  ( NUM_VALUE( BINOP_LEFT( arg_node)) == 3)
    &&  ( NODE_TYPE( BINOP_RIGHT( arg_node)) == N_var)) { // check for pattern 3*k with a variable k
      BINOP_OP( arg_node) = BO_add;
      node *var = BINOP_RIGHT( arg_node);
      node *left_sum = TBmakeBinop(BO_add, COPYdoCopy( var), COPYdoCopy( var));
      BINOP_LEFT( arg_node) = FREEdoFreeTree( BINOP_LEFT( arg_node));
      BINOP_LEFT( arg_node) = left_sum;
    }
    if (( NODE_TYPE( BINOP_RIGHT( arg_node)) == N_num)
    &&  ( NUM_VALUE( BINOP_RIGHT( arg_node)) == 3)
    &&  ( NODE_TYPE( BINOP_LEFT( arg_node)) == N_var)) { // check for pattern 3*k with a variable k
      BINOP_OP( arg_node) = BO_add;
      node *var = BINOP_LEFT( arg_node);
      node *right_sum = TBmakeBinop(BO_add, COPYdoCopy( var), COPYdoCopy( var));
      BINOP_RIGHT( arg_node) = FREEdoFreeTree( BINOP_RIGHT( arg_node));
      BINOP_RIGHT( arg_node) = right_sum;
    }
  }
  
  

  DBUG_RETURN( arg_node);
}


/*
 * Traversal start function
 */

node *SRdoStrengthReduction( node *syntaxtree)
{
  DBUG_ENTER("SRdoStrengthReduction");

  TRAVpush( TR_sr);
  syntaxtree = TRAVdo( syntaxtree, NULL);
  TRAVpop();

  DBUG_RETURN( syntaxtree);
}
