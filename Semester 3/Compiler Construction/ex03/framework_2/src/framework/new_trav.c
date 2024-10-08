
/**
 * @file eval.c
 *
 * Functions needed for compiling
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: free_node.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

/**
 * @defgroup comp Compilation functions.
 *
 * Functions needed for compiling.
 *
 * @{
 */


#include "eval.h"
#include "traverse.h"
#include "dbug.h"


/** <!--******************************************************************-->
 *
 * @fn EVALassign
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Assign node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALassign (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALassign");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALbinop
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALbinop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALbinop");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALblock
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Block node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALblock (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALblock");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALboolconst
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node BoolConst node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALboolconst (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALboolconst");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALcast
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Cast node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALcast (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALcast");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALdecblock
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node DecBlock node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALdecblock (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALdecblock");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALdowhile
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node DoWhile node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALdowhile (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALdowhile");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfloatconst
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FloatConst node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfloatconst (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfloatconst");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfor
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node For node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfor (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfor");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfunargs
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunArgs node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfunargs (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfunargs");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfunbody
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunBody node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfunbody (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfunbody");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfuncall
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunCall node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfuncall (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfuncall");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfundec
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfundec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfundec");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfunparam
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunParam node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfunparam (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfunparam");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfunparams
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunParams node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfunparams (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfunparams");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALif
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node If node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALif (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALif");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALintconst
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node IntConst node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALintconst (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALintconst");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALmodule
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Module node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALmodule (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALmodule");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALmonop
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node MonOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALmonop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALmonop");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALproccall
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ProcCall node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALproccall (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALproccall");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALreturn
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Return node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALreturn (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALreturn");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvardec
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node VarDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALvardec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvardec");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvariable
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Variable node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALvariable (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvariable");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALwhile
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node While node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALwhile (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALwhile");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/**
 * @}
 */
