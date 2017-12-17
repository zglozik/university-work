
-- function class

class FUNCTION_EXP

inherit EXPRESSION
	rename make as expression_make
	redefine print, eval, function_value
end;

creation
	make

feature
	
	make( psymbol_table: SYMBOL_TABLE;
		pvariables: LINK_LIST[STRING]; 
		pexpressions: LINK_LIST[EXPRESSION]) is
		do
			expression_make( function_exp );
			symbol_table := psymbol_table;
			variables := pvariables;
			expressions := pexpressions;
		end;

	eval( params: CONS_EXP ): EXPRESSION is
		local
			param_list: LINK_LIST[EXPRESSION];
			error: BOOLEAN;
			index: INTEGER;
			value: EXPRESSION;
		do
			param_list := make_param_list( params );
			if param_list.count /= variables.count then
				std_output.put_string( "bad argument number!%N" );
			else
				symbol_table.begin_block;
				from index := 1	until index > param_list.count or error
				loop
					value := param_list.item(index).eval( Void );
					if value = Void then
						error := TRUE;
					else
						symbol_table.add( variables.item(index), value );
					end;
					index := index + 1;
				end;
				if not error then
					from index := 1 until index > expressions.count or error
					loop
						Result := expressions.item(index).eval( Void );
						error := Result = Void;
						index := index + 1;
					end;
				end;
				symbol_table.end_block;
			end;
		end;

	print is
		do
			std_output.put_string( "function" );
		end;

	function_value: FUNCTION_EXP is
		do
			Result := Current;
		end;

feature { FUNCTION_EXP }

	make_param_list( params: CONS_EXP ): LINK_LIST[EXPRESSION] is
		local
			param_list: LINK_LIST[EXPRESSION];
			cur_param: CONS_EXP;
		do
			!!param_list.make;
			if params /= Void then
				from
					cur_param := params;
					param_list.add_last( cur_param.car );
				until 
					cur_param.cdr.is_nil
				loop
					cur_param := cur_param.cdr.cons_value;
					param_list.add_last( cur_param.car );
				end;
			end;
			Result := param_list;
		end;

feature { FUNCTION_EXP }

	symbol_table: SYMBOL_TABLE;

	variables: LINK_LIST[ STRING ];

	expressions: LINK_LIST[ EXPRESSION ];

end -- FUNCTION_EXP
