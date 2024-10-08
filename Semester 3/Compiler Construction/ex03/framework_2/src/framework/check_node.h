
/**
 * @file check_node.h
 *
 * Functions to free node structures
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: check_node.h.xsl 15657 2007-11-13 13:57:30Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_CHECK_NODE_H_
#define _SAC_CHECK_NODE_H_

#include "types.h"

extern node *CHKMpostfun (node * arg_node, info * arg_info);

extern node *CHKMassign (node * arg_node, info * arg_info);
extern node *CHKMbinop (node * arg_node, info * arg_info);
extern node *CHKMblock (node * arg_node, info * arg_info);
extern node *CHKMboolconst (node * arg_node, info * arg_info);
extern node *CHKMcast (node * arg_node, info * arg_info);
extern node *CHKMdecblock (node * arg_node, info * arg_info);
extern node *CHKMdowhile (node * arg_node, info * arg_info);
extern node *CHKMfloatconst (node * arg_node, info * arg_info);
extern node *CHKMfor (node * arg_node, info * arg_info);
extern node *CHKMfunargs (node * arg_node, info * arg_info);
extern node *CHKMfunbody (node * arg_node, info * arg_info);
extern node *CHKMfuncall (node * arg_node, info * arg_info);
extern node *CHKMfundec (node * arg_node, info * arg_info);
extern node *CHKMfunparam (node * arg_node, info * arg_info);
extern node *CHKMfunparams (node * arg_node, info * arg_info);
extern node *CHKMif (node * arg_node, info * arg_info);
extern node *CHKMintconst (node * arg_node, info * arg_info);
extern node *CHKMmodule (node * arg_node, info * arg_info);
extern node *CHKMmonop (node * arg_node, info * arg_info);
extern node *CHKMproccall (node * arg_node, info * arg_info);
extern node *CHKMreturn (node * arg_node, info * arg_info);
extern node *CHKMvardec (node * arg_node, info * arg_info);
extern node *CHKMvariable (node * arg_node, info * arg_info);
extern node *CHKMwhile (node * arg_node, info * arg_info);

#endif /* _SAC_CHECK_NODE_H_ */
