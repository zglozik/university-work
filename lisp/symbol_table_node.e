
class SYMBOL_TABLE_NODE

creation
	make

feature

	make is
		do
		end;

feature { SYMBOL_TABLE }

	set_name( pname: STRING ) is
		do
			name := clone( pname );
		end;

	get_name: STRING is
		do
			Result := name;
		end;

	set_value( pvalue: EXPRESSION ) is
		do
			value := pvalue;
		end;

	get_value : EXPRESSION is
		do
			Result := value;
		end;

feature { NONE }

	name: STRING;
	value: EXPRESSION;

end -- SYMBOL_TABLE_NODE
