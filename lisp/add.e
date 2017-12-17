
-- add function class

class ADD

inherit FUNCTION_EXP
	rename make as function_make
	redefine eval
end;

creation 
	make

feature

	make is
		do
			expression_make( function_exp );
		end;
	
	eval( params: CONS_EXP ): EXPRESSION is
		local
			param_list: LINK_LIST[EXPRESSION];
			error: BOOLEAN;
			sum: REAL;
			index: INTEGER;
			value: EXPRESSION;
		do
			param_list := make_param_list( params );
			if param_list.count < 1 then
				std_output.put_string( "Add operator must have at least one" );
				std_output.put_string( " argument!%N" );
			else
				from index := 1	until index > param_list.count or error
				loop
					value := param_list.item(index).eval( Void );
					if value = Void then
						error := TRUE;
					elseif value.get_type /= real_exp then
						std_output.put_string( "Bad argument type!%N" );
						error := TRUE;
					else
						sum := sum + value.real_value;
					end;
					index := index + 1;
				end;
				if not error then
					!REAL_EXP!Result.make( sum );
				end;
			end;
		end;

end -- ADD
