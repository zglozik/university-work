
-- class for symbols

class SYMBOL_EXP

inherit EXPRESSION
	rename make as expression_make
	redefine eval, print, symbol_value
end;

creation
	make, make_nil

feature

	make( psymbol_table: SYMBOL_TABLE; pvalue: STRING ) is
		do
			expression_make( symbol_exp );
			symbol_table := psymbol_table;
			value := clone( pvalue );
			value.to_upper;
		end;

	make_nil( psymbol_table: SYMBOL_TABLE ) is
		do
			make( psymbol_table, "NIL" );
		end;

	eval( params: CONS_EXP ): EXPRESSION is
		do
			if params = Void then
				Result := symbol_table.get( value );
				if Result = Void then
					std_output.put_string( value );
					std_output.put_string( " symbol has no value.%N" );
				end;
			else
				std_output.put_string( "Symbol mustn't have parameters!%N" );
			end;
		end;

	print is
		do
			std_output.put_string( value );
		end;

feature { FUNCTION_EXP }

	symbol_value: STRING is
		do
			Result := value;
		end;

feature { NONE }

	value: STRING;

	symbol_table: SYMBOL_TABLE;

end -- SYMBOL_EXP
