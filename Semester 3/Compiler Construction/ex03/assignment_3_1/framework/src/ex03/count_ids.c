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
 * LUT entry structure
 */
typedef struct LutEntry {
  char *id_name;
  int counter;
} lut_entry_t;

#define LUT_ENTRY_ID_NAME(n) ((n)->id_name)
#define LUT_ENTRY_COUNTER(n) ((n)->counter)

static lut_entry_t *MakeLutEntry(char *id_name, int counter)
{
  DBUG_ENTER( "CIMakeLutEntry");
  lut_entry_t *result;
  result = (lut_entry_t*) MEMmalloc( sizeof( lut_entry_t));
  LUT_ENTRY_ID_NAME( result) = id_name;
  LUT_ENTRY_COUNTER( result) = counter;
  DBUG_RETURN( result);
}


/*
 * INFO structure
 */

struct INFO {
  lut_t *id_lut;
};


/*
 * INFO macros
 */

#define INFO_IDLUT(n)  ((n)->id_lut)


/*
 * INFO functions
 */

static info *MakeInfo(void)
{
  info *result;
  lut_t *lut;

  DBUG_ENTER( "CIMakeInfo");

  result = (info *)MEMmalloc(sizeof(info));

  lut = LUTgenerateLut();
  
  INFO_IDLUT( result) = lut;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
  DBUG_ENTER ("CIFreeInfo");
  
  INFO_IDLUT( info) = LUTremoveContentLut( INFO_IDLUT( info));
  INFO_IDLUT( info) = LUTremoveLut( INFO_IDLUT( info));
  info = MEMfree( info);

  DBUG_RETURN( info);
}

/*
 * Helper functions
 */
 
void CIinsertOrIncrementLutEntry(lut_t *lut, char *key)
{
  if (LUTsearchInLutS(lut, key) == NULL) {
    lut_entry_t *entry = MakeLutEntry(key, 1);
    LUTinsertIntoLutS(lut, key, (void*) entry);
  } else {
    lut_entry_t *entry = *LUTsearchInLutS(lut, key);
    LUT_ENTRY_COUNTER(entry) += 1;
  }
  
}


/*
 * Traversal functions
 */

node *CIvar (node *arg_node, info *arg_info)
{
  DBUG_ENTER("CIvar");

  lut_t *lut = INFO_IDLUT( arg_info);
  char *key = VAR_NAME( arg_node);

  CIinsertOrIncrementLutEntry(lut, key);
  
  DBUG_RETURN( arg_node);
}

node *CIvarlet (node *arg_node, info *arg_info)
{
  DBUG_ENTER("CIvarlet");
  
  lut_t *lut = INFO_IDLUT( arg_info);
  char *key = VARLET_NAME( arg_node);

  CIinsertOrIncrementLutEntry(lut, key);
  
  DBUG_RETURN( arg_node);
}

void *CIprintIdLutEntry(void *entry_v) 
{
  DBUG_ENTER("CIprintIdLutEntry");
  lut_entry_t *entry = (lut_entry_t*) entry_v;
  char* id_name = LUT_ENTRY_ID_NAME( entry);
  int counter = LUT_ENTRY_COUNTER( entry);
  printf("Identifier %s appeared %d times.\n", id_name, counter);
  DBUG_RETURN(entry_v);
}

/*
 * Traversal start function
 */

node *CIdoCountIdentifiers( node *syntaxtree)
{
  info *arg_info;

  DBUG_ENTER("CIdoCountIdentifiers");
  
  arg_info = MakeInfo();

  TRAVpush( TR_ci);
  syntaxtree = TRAVdo( syntaxtree, arg_info);
  TRAVpop();
  
  
  lut_t *lut = INFO_IDLUT( arg_info);
  
  lut = LUTmapLutS( lut, &CIprintIdLutEntry);
  
  arg_info = FreeInfo( arg_info);

  DBUG_RETURN( syntaxtree);
}
