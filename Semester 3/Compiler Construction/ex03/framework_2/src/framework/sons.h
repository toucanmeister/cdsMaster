
/**
 * @file sons.h
 *
 * Defines the NodesUnion and node structures.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: sons.h.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_SONS_H_
#define _SAC_SONS_H_

#include "types.h"


/******************************************************************************
 * For each node a structure of its sons is defined, named 
 * SONS_<nodename>
 *****************************************************************************/
struct SONS_N_ASSIGN
{
  node *Expression;
};
struct SONS_N_BINOP
{
  node *Left;
  node *Right;
};
struct SONS_N_BLOCK
{
  node *This;
  node *Next;
};
struct SONS_N_BOOLCONST
{
};
struct SONS_N_CAST
{
  node *Expression;
};
struct SONS_N_DECBLOCK
{
  node *This;
  node *Next;
};
struct SONS_N_DOWHILE
{
  node *Condition;
  node *Body;
};
struct SONS_N_FLOATCONST
{
};
struct SONS_N_FOR
{
  node *Start;
  node *Stop;
  node *Step;
  node *Body;
};
struct SONS_N_FUNARGS
{
  node *This;
  node *Next;
};
struct SONS_N_FUNBODY
{
  node *Declarations;
  node *Statements;
};
struct SONS_N_FUNCALL
{
  node *Args;
};
struct SONS_N_FUNDEC
{
  node *Params;
  node *Body;
};
struct SONS_N_FUNPARAM
{
};
struct SONS_N_FUNPARAMS
{
  node *This;
  node *Next;
};
struct SONS_N_IF
{
  node *Condition;
  node *Then;
  node *Else;
};
struct SONS_N_INTCONST
{
};
struct SONS_N_MODULE
{
  node *Declarations;
};
struct SONS_N_MONOP
{
  node *Operand;
};
struct SONS_N_PROCCALL
{
  node *Args;
};
struct SONS_N_RETURN
{
  node *Expression;
};
struct SONS_N_VARDEC
{
  node *Expression;
};
struct SONS_N_VARIABLE
{
};
struct SONS_N_WHILE
{
  node *Condition;
  node *Body;
};
/*****************************************************************************
 * This union handles all different types of sons. Its members are
 * called N_nodename.
 ****************************************************************************/
struct SONUNION
{
  struct SONS_N_ASSIGN *N_assign;
  struct SONS_N_BINOP *N_binop;
  struct SONS_N_BLOCK *N_block;
  struct SONS_N_BOOLCONST *N_boolconst;
  struct SONS_N_CAST *N_cast;
  struct SONS_N_DECBLOCK *N_decblock;
  struct SONS_N_DOWHILE *N_dowhile;
  struct SONS_N_FLOATCONST *N_floatconst;
  struct SONS_N_FOR *N_for;
  struct SONS_N_FUNARGS *N_funargs;
  struct SONS_N_FUNBODY *N_funbody;
  struct SONS_N_FUNCALL *N_funcall;
  struct SONS_N_FUNDEC *N_fundec;
  struct SONS_N_FUNPARAM *N_funparam;
  struct SONS_N_FUNPARAMS *N_funparams;
  struct SONS_N_IF *N_if;
  struct SONS_N_INTCONST *N_intconst;
  struct SONS_N_MODULE *N_module;
  struct SONS_N_MONOP *N_monop;
  struct SONS_N_PROCCALL *N_proccall;
  struct SONS_N_RETURN *N_return;
  struct SONS_N_VARDEC *N_vardec;
  struct SONS_N_VARIABLE *N_variable;
  struct SONS_N_WHILE *N_while;
};
#endif /* _SAC_SONS_H_ */