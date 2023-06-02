/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * declare_stmt(void);
static TreeNode * exp(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);
static TreeNode * declare(void);
static TreeNode * value(void);
static TreeNode * bnum(void);
static TreeNode * function_block(void);
static TreeNode * params(void);
static TreeNode * param_list(void);
static TreeNode * param(void);

static void syntaxError(char * message)
{ fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

static void match(TokenType expected)
{ if (token == expected) token = getToken();
  else {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
    fprintf(listing,"     ");
  }
}

TreeNode * stmt_sequence(void)
{ TreeNode * t = statement();
  TreeNode * p = t; // 当前的节点
  while ((token!=ENDFILE) && (token!=END) &&
         (token!=ELSE) && (token!=UNTIL))
  { TreeNode * q;
    match(SEMI);
    q = statement(); // 下一句
    if (q!=NULL) { // 不空，给p接一个兄弟节点
      if (t==NULL) t = p = q;
      else /* now p cannot be NULL either */
      { p->sibling = q;
        p = q;
      }
    }
  }
  return t;
}

TreeNode * statement(void)
{ TreeNode * t = NULL;
  switch (token) {
    case IF : t = if_stmt(); break;
    case REPEAT : t = repeat_stmt(); break;
    case ID : t = assign_stmt(); break;
    case READ : t = read_stmt(); break;
    case WRITE : t = write_stmt(); break;
    case VOID:
    case INT: t = declare_stmt(); break;
    default : syntaxError("unexpected token -> ");
              printToken(token,tokenString);
              token = getToken();
              break;
  } /* end case */
  return t;
}

TreeNode * if_stmt(void)
{ TreeNode * t = newStmtNode(IfK);
  match(IF);
  if (t!=NULL) t->child[0] = exp();
  match(THEN);
  if (t!=NULL) t->child[1] = stmt_sequence();
  if (token==ELSE) {
    match(ELSE);
    if (t!=NULL) t->child[2] = stmt_sequence();
  }
  match(END);
  return t;
}

TreeNode * repeat_stmt(void)
{ TreeNode * t = newStmtNode(RepeatK);
  match(REPEAT);
  if (t!=NULL) t->child[0] = stmt_sequence();
  match(UNTIL);
  if (t!=NULL) t->child[1] = exp();
  return t;
}

TreeNode * assign_stmt(void)
{ TreeNode * t = newStmtNode(AssignK);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(ASSIGN);
  if (t!=NULL) t->child[0] = exp();
  return t;
}

TreeNode * read_stmt(void)
{ TreeNode * t = newStmtNode(ReadK);
  match(READ);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  return t;
}

TreeNode * write_stmt(void)
{ TreeNode * t = newStmtNode(WriteK);
  match(WRITE);
  if (t!=NULL) t->child[0] = exp();
  return t;
}

TreeNode * exp(void)
{ TreeNode * t = simple_exp();
  if ((token==LT)||(token==EQ)) {
    TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
    }
    match(token);
    if (t!=NULL)
      t->child[1] = simple_exp();
  }
  return t;
}

TreeNode * simple_exp(void)
{ TreeNode * t = term();
  while ((token==PLUS)||(token==MINUS))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      t->child[1] = term();
    }
  }
  return t;
}

TreeNode * term(void)
{ TreeNode * t = factor();
  while ((token==TIMES)||(token==OVER))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      p->child[1] = factor();
    }
  }
  return t;
}

TreeNode * factor(void)
{ TreeNode * t = NULL;
  switch (token) {
    case NUM :
      t = newExpNode(ConstK);
      if ((t!=NULL) && (token==NUM))
        t->attr.val = atoi(tokenString);
      match(NUM);
      break;
    case ID : // declare -> ID { [NUM] }
      t = declare();
      break;
    case LPAREN :
      match(LPAREN);
      t = exp();
      match(RPAREN);
      break;
    default:
      syntaxError("unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
    }
  return t;
}

TreeNode * declare(void)
{ // declare -> ID { [NUM] } [ := value ]
  TreeNode * t = NULL;
  char *tmp = copyString(tokenString);
  match(ID);
  if(token == LBRACK)
  {
    t = newExpNode(ArrayK);
    t->attr.name = copyString(tmp);
    TreeNode * p = t->child[0];
    while(token == LBRACK)
    {
      match(LBRACK);
      TreeNode * q = newExpNode(ConstK);
      if ((q!=NULL) && (token==NUM))
        q->attr.val = atoi(tokenString);
      if(p != NULL)
        p->sibling = q;
      else
        t->child[0] = q;
      p = q;
      match(NUM);
      match(RBRACK);
    }
  }
  else // ID
  {
    t = newExpNode(IdK);
    t->attr.name = copyString(tmp);
  }
  if(token == ASSIGN)
  {
    match(ASSIGN);
    t->child[1] = value();
  }
  free(tmp);
  return t;
}

TreeNode * declare_stmt(void)
{ // declare_stmt -> type declare { ,declare }
  TreeNode * t = newStmtNode(DeclareK);
  t->attr.name = copyString(tokenString);
  switch(token) {
    case VOID: match(VOID); break;
    case INT: match(INT); break;
    default:
      syntaxError("unexpected type -> ");
      printToken(token,tokenString);
      token = getToken();
      free(t);
      return NULL;
  }
  t->child[0] = declare();
  TreeNode * p = t->child[0];
  if(p == NULL)
  {
    syntaxError("no var -> ");
    printToken(token,tokenString);
    token = getToken();
    free(t);
    free(p);
    return NULL;
  }
  while(token == COMMA)
  {
    match(COMMA);
    p->sibling = declare();
    p = p->sibling;
  }
  return t;
}

TreeNode * value(void)
{ // value -> NUM | (BNUM)
  TreeNode * t = newExpNode(ConstK);
  switch(token)
  {
    case NUM:
      t->attr.val = atoi(tokenString);
      match(NUM);
      break;
    case LPAREN:
      match(LPAREN);
      t = bnum();
      match(RPAREN);
      break;
    default:
      syntaxError("no value -> ");
      printToken(token,tokenString);
      token = getToken();
      free(t);
      return NULL;
  }
  return t;
}

TreeNode * bnum(void)
{ // bnum -> NUM { ,NUM }
  TreeNode * t = newExpNode(ConstK);
  if(token!=NUM) {
    syntaxError("no value -> ");
    printToken(token,tokenString);
    token = getToken();
    free(t);
    return NULL;
  }
  t->attr.val = atoi(tokenString);
  match(NUM);
  TreeNode * p = t;
  while(token == COMMA)
  {
    match(COMMA);
    TreeNode * q = newExpNode(ConstK);
    if(token!=NUM) {
      syntaxError("no value -> ");
      printToken(token,tokenString);
      token = getToken();
      free(q);
      break;
    }
    q->attr.val = atoi(tokenString);
    match(NUM);
    p->sibling = q;
    p = q;
  }
  return t;
}

TreeNode * function_block(void)
{ // function type ID(params) stmt_sequence end
  match(FUNCTION);
  TreeNode * q = NULL;
  switch(token) { // type
    case VOID:
      q = newFuncNode(TypeK);
      q->attr.name = copyString(tokenString);
      match(VOID);
      break;
    case INT:
      q = newFuncNode(TypeK);
      q->attr.name = copyString(tokenString);
      match(INT);
      break;
    default:
      syntaxError("unexpected function type -> ");
      printToken(token,tokenString);
      token = getToken();
      return NULL;
  }
  TreeNode * t = newFuncNode(FuncIdK);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(LPAREN);
  t->child[0] = q;
  t->child[1] = params();
  match(RPAREN);
  t->child[2] = stmt_sequence();
  match(END);
  return t;
}

TreeNode * params(void)
{ // params -> param_list | void
  if(token == VOID)
  {
    match(VOID);
    return NULL;
  }
  TreeNode * t = param_list();
  return t;
}

TreeNode * param_list(void)
{ //param_list -> param { ,param }
  TreeNode * t = param();
  TreeNode * p = t; // 当前的节点
  while (token == COMMA && p != NULL)
  {
    TreeNode * q;
    match(COMMA);
    q = param(); // 下一句
    p->sibling = q;
    p = q;
  }
  return t;
}

TreeNode * param(void)
{ // param -> type ID { [] }
  TreeNode * p = NULL;
  switch(token) { // type
    case VOID:
      p = newFuncNode(TypeK);
      p->attr.name = copyString(tokenString);
      match(VOID);
      break;
    case INT:
      p = newFuncNode(TypeK);
      p->attr.name = copyString(tokenString);
      match(INT);
      break;
    default:
      syntaxError("unexpected param type -> ");
      printToken(token,tokenString);
      token = getToken();
      return NULL;
  }
  TreeNode * t = newFuncNode(ParamK);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  t->child[0] = p;
  TreeNode * q = newFuncNode(DimensionK);
  q->attr.val = 0;
  while(token == LBRACK)
  {
    match(LBRACK);
    match(RBRACK);
    q->attr.val++;
  }
  t->child[1] = q;
  return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode * parse(void)
{ TreeNode * t = NULL, *p = NULL, *s = NULL;
  token = getToken();
  while(token == FUNCTION) {
    t = function_block();
    if(s == NULL)
      s = p = t;
    else if(p != NULL)
    {
      p->sibling = t;
      p = t;
    }
  }
  t = stmt_sequence();
  if(p != NULL)
    p->sibling = t;
  p = t;
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return s;
}
