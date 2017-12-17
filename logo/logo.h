
#ifndef	LOGO_H
#define	LOGO_H

#include	"SymbolTable.h"
#include	"Expression.h"
#include	"BuildUp.h"
#include	"Turtle.h"

extern Ptr< SymbolTable >		symbol_table;
extern Ptr< ProcSymbolTable >		proc_symbol_table;
extern Ptr< Expression >		expr;
extern Ptr< Stack< Ptr<Sequence> > >	block;
extern Ptr< Stack< Ptr<While> > >	while_stack;
extern Ptr< Stack< Ptr<If> > >		if_stack;
extern Ptr< Turtle >			turtle;

#endif /* LOGO_H */
