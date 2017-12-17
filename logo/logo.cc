
#include	<iostream.h>
#include	<stdio.h>

#include	"logo.h"

/* these are needs for gcc-2.7.2 and earlier

template class	Ptr< SymbolTable >;
template class	Ptr< ProcSymbolTable >;
template class	Ptr< Stack< Ptr<Sequence> > >;
template class	Ptr< Stack< Ptr<While> > >;
template class	Ptr< Stack< Ptr<If> > >;
template class	Ptr< Turtle >;

*/

Ptr< SymbolTable >		symbol_table;
Ptr< ProcSymbolTable >		proc_symbol_table;
Ptr< Expression >		expr;
Ptr< Stack< Ptr<Sequence> > >	block;
Ptr< Stack< Ptr<While> > >	while_stack;
Ptr< Stack< Ptr<If> > >		if_stack;
Ptr< Turtle >			turtle;

int main( int argc, char *argv[] )
{
	extern FILE	*yyin;
	extern int	yyparse();
	extern int	line_no;

	turtle = new Turtle( argc, argv );
	if( argc != 2 ) {
		cerr << "usage: logo <logo_file>\n";
		exit( 1 );
	}
	if( (yyin = fopen(argv[1],"r")) == NULL ) {
		cerr << "can't open file: " << argv[1] << endl;
		exit( 1 );
	}

	symbol_table = new SymbolTable();
	proc_symbol_table = new ProcSymbolTable();

	line_no = 1;
	if( yyparse() == 0 ) {
		Ptr<Sequence> seq = block->pop();
		seq->execute();
		turtle->main_loop();
	}

	return 0;
}
