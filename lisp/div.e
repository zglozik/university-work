
-- div function class

class DIV

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
			quotient: REAL;
			value1, value2: EXPRESSION;
		do
			param_list := make_param_list( params );
			if param_list.count /= 2 then
				std_output.put_string( "Sub operator must have two " );
				std_output.put_string( "arguments!%N" );
			else
				value1 := param_list.item(1).eval( Void );
				value2 := param_list.item(2).eval( Void );
				if value1 /= Void and value2 /= Void then
					if value1.get_type /= real_exp or 
						value2.get_type /= real_exp then
						std_output.put_string( "Bad argument type!%N" );
					else
						quotient := value1.real_value / value2.real_value;
						!REAL_EXP!Result.make( quotient );
					end;
				end;
			end;
		end;

end -- DIV
