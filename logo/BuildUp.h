
#ifndef	BUILDUP_H
#define	BUILDUP_H

#include	"stdtypes.h"
#include	"Expression.h"

class BuildUp : virtual public Reference {

    public:
	virtual	~BuildUp() { };

	virtual	void	execute() = 0;
};

class Command : public BuildUp {

    public:
	Command( Ptr<Expression> expr ) : _expr(expr) { }
	Command( const Command & );

	void	expression( Ptr< Expression > expr );
	void	execute();

    private:
	Ptr< Expression >	_expr;
};

class Sequence : public BuildUp {

    public:
	Sequence() { }
	Sequence( const Sequence & );

	void	add( Ptr< BuildUp > );
	void	execute();

    private:
	Array< RPtr<BuildUp> >	_program;
};

class While : public BuildUp {

    public:
	While() { }
	While( const While & );

	void	condition( Ptr< Expression > );
	void	body( Ptr< Sequence > );
	void	execute();

    private:
	Ptr< Expression >	_condition;
	Ptr< Sequence >		_sequence;
};

class If : public BuildUp {

    public:
	If() { }
	If( const If & );

	void	condition( Ptr< Expression > );
	void	then_branch( Ptr< Sequence > );
	void	else_branch( Ptr< Sequence > );
	void	execute();

    private:
	Ptr< Expression >	_condition;
	Ptr< Sequence >		_then_branch;
	Ptr< Sequence >		_else_branch;
};

#endif /* BUILD_UP */
