
/**
 * @file print.c
 *
 * Functions needed by print traversal.
 *
 */

/**
 * @defgroup print Print Functions.
 *
 * Functions needed by print traversal.
 *
 * @{
 */


#include "print.h"
#include "traverse.h"
#include "tree_basic.h"
#include "dbug.h"
#include "memory.h"
#include "globals.h"


/*
 * INFO structure
 */
struct INFO {
  bool firsterror;
};

#define INFO_FIRSTERROR(n) ((n)->firsterror)

static info *MakeInfo()
{
  info *result;
  
  result = MEMmalloc(sizeof(info));

  INFO_FIRSTERROR(result) = FALSE;
  
  return result;
}


static info *FreeInfo( info *info)
{
  info = MEMfree( info);

  return info;
}



node *
PRTmodule (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTmodule");
  
  MODULE_STMTS( arg_node) = TRAVdo( MODULE_STMTS( arg_node), arg_info);
  
  DBUG_RETURN (arg_node);
}

node *
PRTfunction_declaration (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfunction_declaration");
  
  FUNCTION_DECLARATION_HEADER( arg_node) = TRAVdo( FUNCTION_DECLARATION_HEADER( arg_node), arg_info);
  
  DBUG_RETURN (arg_node);
}

node *
PRTfunction_definition (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfunction_definition");
  
  FUNCTION_DEFINITION_HEADER( arg_node) = TRAVdo( FUNCTION_DEFINITION_HEADER( arg_node), arg_info);
  FUNCTION_DEFINITION_BODY( arg_node) = TRAVdo( FUNCTION_DEFINITION_BODY( arg_node), arg_info);
  
  DBUG_RETURN (arg_node);
}

node *
PRTfunction_header (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfunction_header");
  
  DBUG_RETURN (arg_node);
}

node *
PRTfunction_body (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfunction_body");
  
  FUNCTION_BODY_FIRST_DECLARATION( arg_node) = TRAVdo( FUNCTION_BODY_FIRST_DECLARATION( arg_node), arg_info);
  FUNCTION_BODY_FIRST_STATEMENT( arg_node) = TRAVdo( FUNCTION_BODY_FIRST_STATEMENT( arg_node), arg_info);
  
  DBUG_RETURN (arg_node);
}

node *
PRTvariable_declaration (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTvariable_declaration");
  
  DBUG_RETURN (arg_node);
}

node *
PRTvariable_definition (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTvariable_definition");

  VARIABLE_DEFINITION_DECLARATION( arg_node) = TRAVdo( VARIABLE_DEFINITION_DECLARATION( arg_node), arg_info);
  VARIABLE_DEFINITION_EXPRESSION( arg_node) = TRAVdo( VARIABLE_DEFINITION_EXPRESSION( arg_node), arg_info);
  
  DBUG_RETURN (arg_node);
}

node *
PRTassignment (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTassignment");

  if (ASSIGNMENT_LEFT( arg_node) != NULL) {
    ASSIGNMENT_LEFT( arg_node) = TRAVdo( ASSIGNMENT_LEFT( arg_node), arg_info);
    printf( " = ");
  }
  
  ASSIGNMENT_EXPRESSION( arg_node) = TRAVdo( ASSIGNMENT_EXPRESSION( arg_node), arg_info);
  
  printf( ";\n");
  
  DBUG_RETURN (arg_node);
}

node *
PRTassignment_left (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTassignment_left");
  
  DBUG_RETURN (arg_node);
}

node *
PRTprocedure_call (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTprocedure_call");
  
  DBUG_RETURN (arg_node);
}

node *
PRTif (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTif");
  
  IF_CONDITION( arg_node) = TRAVdo( IF_CONDITION( arg_node), arg_info);
  if (IF_CONSEQUENCE( arg_node) != NULL) {
    IF_CONSEQUENCE( arg_node) = TRAVdo( IF_CONSEQUENCE( arg_node), arg_info);
  }
  
  DBUG_RETURN (arg_node);
}

node *
PRTwhile (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTwhile");
  
  WHILE_CONDITION( arg_node) = TRAVdo( WHILE_CONDITION( arg_node), arg_info);
  if (WHILE_BODY( arg_node) != NULL) {
    WHILE_BODY( arg_node) = TRAVdo( WHILE_BODY( arg_node), arg_info);
  }
  
  DBUG_RETURN (arg_node);
}

node *
PRTdo_while (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTdo_while");
  
  DO_WHILE_CONDITION( arg_node) = TRAVdo( DO_WHILE_CONDITION( arg_node), arg_info);
  if (DO_WHILE_BODY( arg_node) != NULL) {
    DO_WHILE_BODY( arg_node) = TRAVdo( DO_WHILE_BODY( arg_node), arg_info);
  }
  
  DBUG_RETURN (arg_node);
}

node *
PRTfor (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfor");
  
  FOR_HEADER( arg_node) = TRAVdo( FOR_HEADER( arg_node), arg_info);
  if (FOR_BODY( arg_node) != NULL) {
    FOR_BODY( arg_node) = TRAVdo( FOR_BODY( arg_node), arg_info);
  }
  
  DBUG_RETURN (arg_node);
}

node *
PRTfor_header (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfor_header");
  
  FOR_HEADER_EXPRESSION( arg_node) = TRAVdo( FOR_HEADER_EXPRESSION( arg_node), arg_info);
  
  DBUG_RETURN (arg_node);
}

node *
PRTreturn (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTreturn");
  
  if (RETURN_EXPRESSION( arg_node) != NULL) {
    RETURN_EXPRESSION( arg_node) = TRAVdo( RETURN_EXPRESSION( arg_node), arg_info);
  }
  
  DBUG_RETURN (arg_node);
}

node *
PRTmonop (node * arg_node, info * arg_info) 
{
  DBUG_ENTER ("PRTmonop");
  
  MONOP_OPERAND( arg_node) = TRAVdo( MONOP_OPERAND( arg_node), arg_info);
  
  DBUG_RETURN( arg_node);
}

node *
PRTbinop (node * arg_node, info * arg_info)
{
  char *tmp;

  DBUG_ENTER ("PRTbinop");

  printf( "( ");

  BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);

  switch (BINOP_OP( arg_node)) {
    case BO_add:
      tmp = "+";
      break;
    case BO_sub:
      tmp = "-";
      break;
    case BO_mul:
      tmp = "*";
      break;
    case BO_div:
      tmp = "/";
      break;
    case BO_mod:
      tmp = "%";
      break;
    case BO_lt:
      tmp = "<";
      break;
    case BO_le:
      tmp = "<=";
      break;
    case BO_gt:
      tmp = ">";
      break;
    case BO_ge:
      tmp = ">=";
      break;
    case BO_eq:
      tmp = "==";
      break;
    case BO_ne:
      tmp = "!=";
      break;
    case BO_or:
      tmp = "||";
      break;
    case BO_and:
      tmp = "&&";
      break;
    case BO_unknown:
      DBUG_ASSERT( 0, "unknown binop detected!");
  }

  printf( " %s ", tmp);

  BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);

  printf( ")");

  DBUG_RETURN (arg_node);
}

node *
PRTcast (node * arg_node, info * arg_info) 
{
  DBUG_ENTER ("PRTcast");
  
  CAST_EXPRESSION( arg_node) = TRAVdo( CAST_EXPRESSION( arg_node), arg_info);
  
  DBUG_RETURN( arg_node);
}

node *
PRTfunction_call (node * arg_node, info * arg_info) 
{
  DBUG_ENTER ("PRTfunction_call");
  
  DBUG_RETURN( arg_node);
}

node *
PRTfloat_constant (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfloat_constant");

  printf( "%f", FLOAT_CONSTANT_VALUE( arg_node));

  DBUG_RETURN (arg_node);
}

node *
PRTint_constant (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTint_constant");

  printf( "%i", INT_CONSTANT_VALUE( arg_node));

  DBUG_RETURN (arg_node);
}

node *
PRTbool_constant (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTbool_constant");

  if (BOOL_CONSTANT_VALUE( arg_node)) {
    printf( "true");
  }
  else {
    printf( "false");
  }
  
  DBUG_RETURN (arg_node);
}

node *
PRTvariable (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTvariable");

  printf( "%s", VARIABLE_NAME( arg_node));

  DBUG_RETURN (arg_node);
}

node *PRTsymboltableentry (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTsymboltableentry");

  DBUG_RETURN (arg_node);
}

node *
PRTerror (node * arg_node, info * arg_info)
{
  bool first_error;

  DBUG_ENTER ("PRTerror");

  if (NODE_ERROR (arg_node) != NULL) {
    NODE_ERROR (arg_node) = TRAVdo (NODE_ERROR (arg_node), arg_info);
  }

  first_error = INFO_FIRSTERROR( arg_info);

  if( (global.outfile != NULL)
      && (ERROR_ANYPHASE( arg_node) == global.compiler_anyphase)) {

    if ( first_error) {
      printf ( "\n/******* BEGIN TREE CORRUPTION ********\n");
      INFO_FIRSTERROR( arg_info) = FALSE;
    }

    printf ( "%s\n", ERROR_MESSAGE( arg_node));

    if (ERROR_NEXT (arg_node) != NULL) {
      TRAVopt (ERROR_NEXT (arg_node), arg_info);
    }

    if ( first_error) {
      printf ( "********  END TREE CORRUPTION  *******/\n");
      INFO_FIRSTERROR( arg_info) = TRUE;
    }
  }

  DBUG_RETURN (arg_node);
}



/** <!-- ****************************************************************** -->
 * @brief Prints the given syntaxtree
 * 
 * @param syntaxtree a node structure
 * 
 * @return the unchanged nodestructure
 ******************************************************************************/

node 
*PRTdoPrint( node *syntaxtree)
{
  info *info;
  
  DBUG_ENTER("PRTdoPrint");

  DBUG_ASSERT( (syntaxtree!= NULL), "PRTdoPrint called with empty syntaxtree");

  printf( "\n\n------------------------------\n\n");

  info = MakeInfo();
  
  TRAVpush( TR_prt);

  syntaxtree = TRAVdo( syntaxtree, info);

  TRAVpop();

  info = FreeInfo(info);

  printf( "\n------------------------------\n\n");

  DBUG_RETURN( syntaxtree);
}

/**
 * @}
 */
