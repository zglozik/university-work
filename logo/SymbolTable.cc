
#include	"SymbolTable.h"
#include	"BuildUp.h"

void SymbolTable::begin()
{
	_borders.append( _table.num_elements() );
}

void SymbolTable::end()
{
	int last = _borders.num_elements()-1;
	int lastv = _borders[last];
	
	for( int i = _table.num_elements()-1; i >= lastv; i-- ) {
		_table.remove_index( i );
	}
	_borders.remove_index( last );
}

void SymbolTable::set( const String &name, double value )
{
	int	i;

	for( i=_table.num_elements()-1; i>=0 && name != _table[i].name; i-- );
	if( i < 0 ) {
		Symbol	symbol;
		symbol.name = name;
		symbol.value = value;
		_table.append( symbol );
	} else {
		_table[i].value = value;
	}
}

void SymbolTable::append( const String &name, double value )
{
	Symbol	symbol;
	
	symbol.name = name;
	symbol.value = value;
	_table.append( symbol );
}
	
double SymbolTable::get( const String &name ) throw( UNDEFINED )
{
	int	i;
	for( i=_table.num_elements()-1; i>=0 && name != _table[i].name; i-- );

	if( i < 0 ) throw UNDEFINED();

	return _table[i].value;
}

void ProcSymbolTable::declare( const String &name,
			      const Array<String> &params )
{
	int	i;

	for( i=_table.num_elements()-1; i>=0 && name != _table[i].name; i-- );
	if( i < 0 ) {
		Symbol	symbol;
		symbol.name = name;
		symbol.params = params;
		symbol.body = 0;
		_table.append( symbol );
	} else {
		_table[i].params = params;
	}
}

void ProcSymbolTable::define( const String &name, Ptr<Sequence> body ) 
	throw( DEFINED )
{
	int	i;

	for( i=_table.num_elements()-1; i>=0 && name != _table[i].name; i-- );

	assert( i>= 0 );
	if( !_table[i].body ) {
		_table[i].body = body;
	} else {
		throw DEFINED();
	}
}

void ProcSymbolTable::get( const String &name, Array<String> &params,
			  Ptr<Sequence> &body ) throw( UNDEFINED )
{
	int	i;
	for( i=_table.num_elements()-1; i>=0 && name != _table[i].name; i-- );

	if( i < 0 ) throw UNDEFINED();

	params = _table[i].params;
	body = _table[i].body;
}
