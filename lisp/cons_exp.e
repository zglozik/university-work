
-- cons expression type

class CONS_EXP

inherit EXPRESSION
	rename make as expression_make
	redefine print, eval, cons_value
end;

creation
	make

feature

	make is
		do
			expression_make( cons_exp );
		end;

	print is
		do
			std_output.put_character( '(' );
			car.print;
			std_output.put_string( " . " );
			cdr.print;
			std_output.put_character( ')' );
		end;

	eval( params: CONS_EXP ): EXPRESSION is
		local
			param_list: CONS_EXP;
			function: EXPRESSION;
		do
			if params /= Void then
				std_output.put_string( "Cons mustn't have parameters!" );
			elseif not cdr.is_nil and cdr.get_type /= cons_exp then
				std_output.put_string( "Bad argument structure.%N" );
			else
				if not cdr.is_nil then
					param_list := cdr.cons_value;
				end;
				function := car.eval( Void );
				if function /= Void then
					if function.get_type /= function_exp then
						std_output.put_string( "It is not a function.%N" );
					else
						Result := function.eval( param_list );
					end;
				end;
			end;
		end;

	cons_value: CONS_EXP is
		do
			Result := Current;
		end;

feature { PROGRAM, FUNCTION_EXP, CONS_EXP }

	car: EXPRESSION is
		do
			Result := car_value;
		end;

	cdr: EXPRESSION is
		do
			Result := cdr_value;
		end;

	set_car( pcar: EXPRESSION ) is
		do
			car_value := pcar;
		end;

	set_cdr( pcdr: EXPRESSION ) is
		do
			cdr_value := pcdr;
		end;

feature { NONE }

	car_value: EXPRESSION;
	cdr_value: EXPRESSION;

end -- CONS_EXP
