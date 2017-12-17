
-- class for real expressions

class REAL_EXP

inherit EXPRESSION
	rename make as expression_make
	redefine eval, print, real_value
end;

creation
	make

feature

	make( pvalue: REAL ) is
		do
			expression_make( real_exp );
			value := pvalue;
		end;

	eval( params: CONS_EXP ): EXPRESSION is
		do
			if params = Void then
				Result := Current;
			else
				std_output.put_string( "Real mustn't have parameters!%N" );
			end;
		end;

	print is
		do
			std_output.put_real( value );
		end;

feature { FUNCTION_EXP }

	real_value: REAL is
		do
			Result := value;
		end;

feature { NONE }

	value: REAL;

end -- REAL_EXP
