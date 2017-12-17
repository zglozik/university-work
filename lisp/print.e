
-- print function class

class PRINT

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
			index: INTEGER;
			value: EXPRESSION;
		do
			param_list := make_param_list( params );
			from index := 1	until index > param_list.count or error
			loop
				value := param_list.item(index).eval( Void );
				if value = Void then
					error := TRUE;
				else
					value.print;
					std_output.put_character( ' ' );
					Result := value;
				end;
				index := index + 1;
			end;
			std_output.put_character( '%N' );
		end;

end -- PRINT
