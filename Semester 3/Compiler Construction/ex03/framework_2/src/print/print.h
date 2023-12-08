
/**
 * @file print.h
 *
 * Functions to print node structures
 * 
 */

#ifndef _SAC_PRT_NODE_H_
#define _SAC_PRT_NODE_H_

#include "types.h"

extern node *PRTmodule (node * arg_node, info * arg_info);
extern node *PRTfunction_declaration (node * arg_node, info * arg_info);
extern node *PRTfunction_definition (node * arg_node, info * arg_info);
extern node *PRTfunction_header (node * arg_node, info * arg_info);
extern node *PRTfunction_body (node * arg_node, info * arg_info);
extern node *PRTvariable_declaration (node * arg_node, info * arg_info);
extern node *PRTvariable_definition (node * arg_node, info * arg_info);
extern node *PRTassignment (node * arg_node, info * arg_info);
extern node *PRTassignment_left (node * arg_node, info * arg_info);
extern node *PRTprocedure_call (node * arg_node, info * arg_info);
extern node *PRTif (node * arg_node, info * arg_info);
extern node *PRTwhile (node * arg_node, info * arg_info);
extern node *PRTdo_while (node * arg_node, info * arg_info);
extern node *PRTfor (node * arg_node, info * arg_info);
extern node *PRTfor_header (node * arg_node, info * arg_info);
extern node *PRTreturn (node * arg_node, info * arg_info);
extern node *PRTmonop (node * arg_node, info * arg_info);
extern node *PRTbinop (node * arg_node, info * arg_info);
extern node *PRTcast (node * arg_node, info * arg_info);
extern node *PRTfunction_call (node * arg_node, info * arg_info);
extern node *PRTfloat_constant (node * arg_node, info * arg_info);
extern node *PRTint_constant (node * arg_node, info * arg_info);
extern node *PRTbool_constant (node * arg_node, info * arg_info);
extern node *PRTvariable (node * arg_node, info * arg_info);
extern node *PRTsymboltableentry (node * arg_node, info * arg_info);
extern node *PRTerror (node * arg_node, info * arg_info);

extern node *PRTdoPrint( node *syntaxtree);

#endif /* _SAC_PRT_NODE_H_ */
