
-- greater function class

class GREATER

inherit FUNCTION_EXP
	rename make as function_make
	redefine eval
end;

creation 
	make

feature

	make( psymbol_table: SYMBOL_TABLE ) is
		do
			expression_make( function_exp );
			symbol_table := psymbol_table;
		end;
	
	eval( params: CONS_EXP ): EXPRESSION is
		local
			param_list: LINK_LIST[ EXPRESSION ];
			value1, value2: EXPRESSION;
		do
			param_list := make_param_list( params );
			if param_list.count /= 2  then
				std_output.put_string( "> operator must have two" );
				std_output.put_string( " arguments!%N" );
			else
				value1 := param_list.item(1).eval( Void );
				value2 := param_list.item(2).eval( Void );
				if value1 /= Void and value2 /= Void then
					if value1.get_type /= real_exp or
						value2.get_type /= real_exp then
						std_output.put_string( "Arguments of > operator must");
						std_output.put_string( " be of type real!%N" );
					elseif value1.real_value > value2.real_value then
						!SYMBOL_EXP!Result.make( symbol_table, "T" );
					else
						!SYMBOL_EXP!Result.make_nil( symbol_table );
					end;
				end;
			end;
		end;

end -- GREATER
