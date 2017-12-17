
-- abstract expression class, it is the ancestor of the actual types

deferred class EXPRESSION

inherit GENERAL
	rename print as general_print
end;

feature

	cons_exp, function_exp, 
	real_exp, symbol_exp : INTEGER is unique;

	make( ptype: INTEGER ) is
		require
			ptype >= cons_exp and ptype <= symbol_exp
		do
			type := ptype;
		end;

	eval( params: CONS_EXP ): EXPRESSION is
		deferred
		end;

	print is
		deferred
		end;

	get_type: INTEGER is
		-- returns the type of the actual expression
		do
			Result := type;
		end;

	cons_value: CONS_EXP is
		require
			type = cons_exp
		do
		end;

	real_value: REAL is
		require
			type = real_exp
		do
		end;

	symbol_value: STRING is
		require
			type = symbol_exp
		do
		end;

	function_value: FUNCTION is
		require
			type = function_exp
		do
		end;

	is_nil: BOOLEAN is
		do
			Result := type = symbol_exp and then symbol_value.is_equal("NIL");
		end;

feature { NONE }

	type: INTEGER;
	
end -- EXPRESSION
