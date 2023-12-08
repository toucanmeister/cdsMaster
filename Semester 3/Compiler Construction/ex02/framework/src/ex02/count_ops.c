/*****************************************************************************
 *
 * Module: count_ops
 *
 * Prefix: CO
 *
 * Description:
 *
 * This module implements a demo traversal of the abstract syntax tree that 
 * counts the number of occurences for each arithmetic operator and stores the
 * result in the root node.
 *
 *****************************************************************************/


#include "count_ops.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"
#include "lookup_table.h"
#include "memory.h"
#include "ctinfo.h"
#include "str.h"


/*
 * INFO structure
 */

struct INFO {
  lut_t *op_lut;
};


/*
 * INFO macros
 */

#define INFO_OPLUT(n)  ((n)->op_lut)


/*
 * INFO functions
 */

static info *MakeInfo(void)
{
  info *result;
  lut_t *lut;

  DBUG_ENTER( "MakeInfo");

  result = (info *)MEMmalloc(sizeof(info));

  lut = LUTgenerateLut();
  
  for (int i=BO_add; i <= BO_unknown; i++) {
    char *key = STRitoa( i);
    int *counter = malloc( sizeof(int));
    *counter = 0;
    LUTinsertIntoLutS(lut, key, (void*) counter);
    MEMfree( key);
  }
  
  INFO_OPLUT( result) = lut;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
  lut_t *lut;
  DBUG_ENTER ("FreeInfo");
  
  lut = INFO_OPLUT( info);
  char *key;
  for (int i=BO_add; i <= BO_unknown; i++) {
    key = STRitoa( i);
    MEMfree( *LUTsearchInLutS(lut, key));
    MEMfree( key);
  }
  INFO_OPLUT( info) = LUTremoveLut( INFO_OPLUT( info));
  info = MEMfree( info);

  DBUG_RETURN( info);
}


/*
 * Traversal functions
 */

node *CObinop (node *arg_node, info *arg_info)
{
  DBUG_ENTER("CObinop");

  BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);
  BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);

  lut_t *lut = INFO_OPLUT( arg_info);
  char *key = STRitoa( BINOP_OP( arg_node));
  int *counter = *LUTsearchInLutS(lut, key);
  *counter += 1;
  MEMfree( key);
  
  DBUG_RETURN( arg_node);
}


/*
 * Traversal start function
 */

node *COdoCountOps( node *syntaxtree)
{
  info *arg_info;

  DBUG_ENTER("COdoCountOps");
  
  arg_info = MakeInfo();

  TRAVpush( TR_co);
  syntaxtree = TRAVdo( syntaxtree, arg_info);
  TRAVpop();
  
  
  lut_t *lut = INFO_OPLUT( arg_info);
  char* key = STRitoa( BO_add);
  int *add_count = *LUTsearchInLutS( lut, key);
  MEMfree( key);
  key = STRitoa( BO_sub);
  int *sub_count = *LUTsearchInLutS( lut, key);
  MEMfree( key);
  key = STRitoa( BO_mul);
  int *mul_count = *LUTsearchInLutS( lut, key);
  MEMfree( key);
  key = STRitoa( BO_div);
  int *div_count = *LUTsearchInLutS( lut, key);
  MEMfree( key);
  key = STRitoa( BO_mod);
  int *mod_count = *LUTsearchInLutS( lut, key);
  MEMfree( key);

  syntaxtree = TBmakeModule(*add_count, *sub_count, *mul_count, *div_count, *mod_count, syntaxtree);

  arg_info = FreeInfo( arg_info);

  DBUG_RETURN( syntaxtree);
}
