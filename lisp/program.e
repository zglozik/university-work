-- class to build a list of lisp expressions

class PROGRAM

creation
	make

feature

	make( psymbol_table: SYMBOL_TABLE; pfile: STRING ) is
		do
			symbol_table := psymbol_table;
			file := clone(pfile);
		end;

	read_program: LINK_LIST[EXPRESSION] is
		local
			exp: EXPRESSION;
		do
			!!input_file.connect_to( file );
			if input_file.is_connected then
				!!expressions.make;
				error := FALSE;
				from
					token := input_file.read_token;
					exp := read_expression;
				until
					exp = Void or else error
				loop
					expressions.add_last( exp );
					exp := read_expression;
				end;
				input_file.disconnect;
				if not error then
					Result := expressions;
				end;
			end;
		end;

	read_expression: EXPRESSION is
		do
			if token = input_file.left_parent then
				token := input_file.read_token;
				if token /= input_file.eof then
					Result := read_cons;
				else
					error := TRUE;
				end;
			elseif token = input_file.symbol_token then
				!SYMBOL_EXP!Result.make( symbol_table, 
						clone(input_file.last_symbol) );
				token := input_file.read_token;
			elseif token = input_file.real_token then
				!REAL_EXP!Result.make( input_file.last_real );
				token := input_file.read_token;
			elseif token = input_file.right_parent then
				std_output.put_string( "unexpected ) token%N" );
				error := TRUE;
			end;
		end;

	read_cons: EXPRESSION is
		local
			exp: EXPRESSION
			first, last, cur: CONS_EXP;
		do
			from
			until
				token = input_file.eof or token = input_file.right_parent
				or error
			loop
				exp := read_expression;
				if exp /= Void then
					!!cur.make;
					cur.set_car( exp );
					if first = Void then
						first := cur;
					else
						last.set_cdr( cur );
					end;
					last := cur;
				end;
			end;
			if token = input_file.eof and not error then
				std_output.put_string( "Missing ) token.%N" );
				error := TRUE;
			elseif not error then 
				!SYMBOL_EXP!exp.make_nil( symbol_table );
				if last = Void then 
					Result := exp;
				else
					last.set_cdr( exp );
					Result := first;
				end;
				token := input_file.read_token;
			end;
		end;

feature { NONE }

	file: STRING;
	
	input_file: INPUT_FILE;

	token: INTEGER;

	expressions: LINK_LIST[EXPRESSION];

	symbol_table: SYMBOL_TABLE;

	error: BOOLEAN;

end -- PROGRAM
