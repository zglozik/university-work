
-- defun function class

class DEFUN

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
		do
			param_list := make_param_list( params );
			if param_list.count < 3 then
				std_output.put_string( "Defun operator must have at least" );
				std_output.put_string( " three arguments!%N" );
			elseif param_list.item(1).get_type /= symbol_exp or else
				param_list.item(1).is_nil then
				std_output.put_string( "Function name is not symbol!%N" );
			else
				variables := get_variables( params );
				if variables /= Void then
					expressions := param_list.slice( 3, param_list.count );
					!FUNCTION_EXP!Result.make( symbol_table, 
							variables, expressions );
					symbol_table.add( param_list.item(1).symbol_value,
							Result );
				end;
			end;
		end;

feature { NONE }

	get_variables( params: CONS_EXP ): LINK_LIST[STRING] is
		local
			param_list: LINK_LIST[ EXPRESSION ];
			error: BOOLEAN;
			index: INTEGER;
		do
			if params.cdr.cons_value.car.get_type = cons_exp then
				param_list := make_param_list( 
						params.cdr.cons_value.car.cons_value );
				!!variables.make;
				from index := 1 until index > param_list.count or error
				loop
					if param_list.item(index).get_type = symbol_exp and then
						not param_list.item(index).is_nil then
						variables.add_last( 
							param_list.item(index).symbol_value );
					else
						error := TRUE;
					end;
					index := index + 1;
				end;
				if not error then
					Result := variables;
				end;
			elseif params.cdr.cons_value.car.is_nil then
				!!Result.make;
			else
				std_output.put_string( "Variable list is absent.%N" );
			end;
		end;

end -- DEFUN
