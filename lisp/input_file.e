
-- class for reading input file

class INPUT_FILE

creation
	connect_to

feature

	real_token, symbol_token, 
	left_parent, right_parent, 
	eof, failed: INTEGER is unique;

	connect_to( file_name: STRING ) is
		do
			!!file.connect_to( file_name );
			!!symbol_value.make( 100 );
			file.read_character;
			skip_white_spaces;
		end;

	read_token: INTEGER is
		do
			if file.end_of_input then
				Result := eof;
			else
				inspect file.last_character
					when '(' then
						Result := left_parent;
						file.read_character;
					when ')' then
						Result := right_parent;
						file.read_character;
					else
						if file.last_character.is_digit then
							Result := read_real;
						else
							Result := read_symbol;
						end;
				end;
				skip_white_spaces;
			end;
		end;

	last_symbol: STRING is
		do
			Result := symbol_value;
		end;

	last_real: REAL is
		do
			Result := real_value;
		end;

	disconnect is
		do
			if file.is_connected then
				file.disconnect;
			end;
		end;

	is_connected: BOOLEAN is
		do
			Result := file.is_connected;
		end;

feature { INPUT_FILE }

	is_white_space( ch: CHARACTER ): BOOLEAN is
		do
			Result := ch = ' ' or ch = '%N' or ch = '%/9/';
		end;

	skip_white_spaces is
		do
			from
			until
				file.end_of_input or else 
				not is_white_space( file.last_character )
			loop
				file.read_character;
			end;
		end;

	read_symbol: INTEGER is
		do
			from
				symbol_value.make( 100 );
			until
				file.end_of_input or else is_white_space( file.last_character )
				or else file.last_character = ')'
			loop
				symbol_value.add_last( file.last_character );
				file.read_character;
			end;
			Result := symbol_token;
		end;

	read_real: INTEGER is
		local
			power: REAL;
			index: INTEGER;
			error: BOOLEAN;
		do
			Result := read_symbol;
			from
				index := 1;
				real_value := 0;
			until
				index > symbol_value.count or else 
				symbol_value.item(index) = '.' or else error
			loop
				if symbol_value.item(index).is_digit then
					real_value := real_value * 10 + 
						symbol_value.item(index).value;
				else
					error := TRUE;
				end;
				index := index + 1;
			end;
			from
				power := 1;
				index := index + 1;
			until index > symbol_value.count or else error
			loop
				if symbol_value.item(index).is_digit then
					power := power * 10;
					real_value := real_value + 
						symbol_value.item(index).value.to_real / power;
				else
					error := TRUE;
				end;
				index := index + 1;
			end;
			if not error then
				Result := real_token;
			else
				Result := failed;
			end;
		end;

feature { NONE }

	symbol_value: STRING;
	real_value: REAL;
	file: STD_FILE_READ;

end -- INPUT_FILE
