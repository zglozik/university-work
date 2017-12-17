
#ifndef	SYMBOL_TABLE_H
#define	SYMBOL_TABLE_H

#include	"cstring.h"
#include	"stdtypes.h"
#include	"BuildUp.h"

class SymbolTable : virtual public Reference {

    public:
	class UNDEFINED : virtual public Reference { };

	SymbolTable() { begin(); }

	void	begin();
	void	end();
	void	set( const String &, double );
	void	append( const String &, double );
	double	get( const String & ) throw( UNDEFINED );

    private:
	struct Symbol : virtual public Reference {
		String	name;
		double	value;
	};

	Array< Symbol >	_table;
	Array< int >	_borders;
};

class ProcSymbolTable : virtual public Reference {

    public:
	class UNDEFINED : virtual public Reference { };
	class DEFINED : virtual public Reference { };

	ProcSymbolTable() { }
	~ProcSymbolTable() { }

	void declare( const String &, const Array<String> &params );
	void define( const String &, Ptr<Sequence> body ) throw( DEFINED );
	void get( const String &, Array<String> &params,
		 Ptr<Sequence> &body )
		throw( UNDEFINED );

    private:
	struct Symbol : virtual public Reference {
		String		name;
		Array<String>	params;
		Ptr<Sequence>	body;
	};

	Array< Symbol >	_table;
};

#endif /* SYMBOL_TABLE_H */
