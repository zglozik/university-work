
class LISP

creation
	make

feature

	make is
		local
			index: INTEGER;
			program: PROGRAM;
			exp: EXPRESSION;
		do
			make_symbol_table;
			if argument_count /= 1 then
				std_output.put_string( "usage: lisp <source file>%N" );
			else
				!!program.make( symbol_table, argument(1) );
				expressions := program.read_program;
			end;
			if expressions /= Void then
--				write_program;
				exp := eval_expressions;
				if exp = Void then
					std_output.put_string( "ERROR%N" );
				else
					exp.print;
					std_output.put_new_line;
				end;
			end;
		end;

	eval_expressions: EXPRESSION is
		local
			index: INTEGER;
			exp: EXPRESSION;
			error: BOOLEAN;
		do
			from
				index := 1;
			until
				index > expressions.count or else error
			loop
				exp := expressions.item(index).eval( Void );
				error := exp = Void;
				index := index + 1;
			end;
			Result := exp;
		end;

	write_program is
		local
			index: INTEGER;
		do
			from index := 1 until index > expressions.count
			loop
				expressions.item(index).print;
				std_output.put_new_line;
				index := index + 1;
			end;
		end;

	make_symbol_table is
		local
			nil: SYMBOL_EXP;
			t: SYMBOL_EXP;
			print_f: PRINT;
			add_f: ADD;
			sub_f: SUB;
			mul_f: MUL;
			div_f: DIV;
			defun_f: DEFUN;
			setq_f: SETQ;
			if_f: IF_F;
			less_f: LESS;
			greater_f: GREATER;
			equal_f: EQUAL;
		do
			!!symbol_table.make;
			!!nil.make_nil( symbol_table );
			symbol_table.add( "NIL", nil );
			!!t.make( symbol_table, "T" );
			symbol_table.add( "T", t );
			!!print_f.make;
			symbol_table.add( "PRINT", print_f );
			!!add_f.make;
			symbol_table.add( "ADD", add_f );
			symbol_table.add( "+", add_f );
			!!sub_f.make;
			symbol_table.add( "SUB", sub_f );
			symbol_table.add( "-", sub_f );
			!!mul_f.make;
			symbol_table.add( "MUL", mul_f );
			symbol_table.add( "*", mul_f );
			!!div_f.make;
			symbol_table.add( "DIV", div_f );
			symbol_table.add( "/", div_f );
			!!defun_f.make( symbol_table );
			symbol_table.add( "DEFUN", defun_f );
			!!setq_f.make( symbol_table );
			symbol_table.add( "SETQ", setq_f );
			!!if_f.make( symbol_table );
			symbol_table.add( "IF", if_f );
			!!less_f.make( symbol_table );
			symbol_table.add( "<", less_f );
			!!greater_f.make( symbol_table );
			symbol_table.add( ">", greater_f );
			!!equal_f.make( symbol_table );
			symbol_table.add( "=", equal_f );
		end;

feature { NONE }

	symbol_table: SYMBOL_TABLE;

	expressions: LINK_LIST[ EXPRESSION ];

end -- LISP
