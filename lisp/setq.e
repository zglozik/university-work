
-- SETQ function class

class SETQ

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
			param_list: LINK_LIST[EXPRESSION];
			value: EXPRESSION;
		do
			param_list := make_param_list( params );
			if param_list.count /= 2 then
				std_output.put_string( "Setq operator must have two" );
				std_output.put_string( " arguments!%N" );
			elseif param_list.item(1).get_type /= symbol_exp or else
				param_list.item(1).is_nil then
				std_output.put_string( "First argument of setq must " );
				std_output.put_string( " be symbol!");
			else
				value := param_list.item(2).eval( Void );
				if value /= Void then
					symbol_table.set( param_list.item(1).symbol_value,
							value );
					Result := value;
				end;
			end;
		end;

end -- SETQ
