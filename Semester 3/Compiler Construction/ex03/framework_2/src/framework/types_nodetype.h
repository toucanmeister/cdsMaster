
/**
 * @file types_nodetype.h
 *
 * This file defines the nodetype node enumeration.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: types_nodetype.h.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_TYPES_NODETYPE_H_
#define _SAC_TYPES_NODETYPE_H_

#define MAX_NODES 24
typedef enum
{ N_undefined = 0, N_module = 1, N_decblock = 2, N_fundec = 3, N_funparams =
    4, N_funparam = 5, N_funbody = 6, N_vardec = 7, N_block = 8, N_assign =
    9, N_proccall = 10, N_if = 11, N_while = 12, N_dowhile = 13, N_for =
    14, N_return = 15, N_binop = 16, N_monop = 17, N_cast = 18, N_funcall =
    19, N_funargs = 20, N_variable = 21, N_intconst = 22, N_floatconst =
    23, N_boolconst = 24 } nodetype;

#endif /* _SAC_TYPES_NODETYPE_H_ */
