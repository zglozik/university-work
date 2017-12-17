
-- if function class

class IF_F

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
			cond: EXPRESSION;
			param_list: LINK_LIST[ EXPRESSION ];
		do
			param_list := make_param_list( params );
			if param_list.count < 2 then
				std_output.put_string( "If operator must have two or three" );
				std_output.put_string( " arguments!%N" );
			else
				cond := param_list.item(1).eval( Void );
				if cond /= Void then
					if cond.is_nil then
						if param_list.count = 3 then
							Result := param_list.item(3).eval( Void );
						else
							!SYMBOL_EXP!Result.make_nil( symbol_table );
						end;
					else
						Result := param_list.item(2).eval( Void );
					end;
				end;
			end;
		end;

end -- IF_F
