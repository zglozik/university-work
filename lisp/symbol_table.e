
-- symbol table for variables and functions (they are all the same)

class SYMBOL_TABLE

creation
	make

feature

	make is
		local
			block_table: LINK_LIST[ SYMBOL_TABLE_NODE ];
		do
			!!table.make;
			!!block_table.make;
			table.add_last( block_table );
		end;

	begin_block is
		local
			block_table: LINK_LIST[ SYMBOL_TABLE_NODE ];
		do
			!!block_table.make;
			table.add_last( block_table );
		end;

	end_block is
		require
			table.count > 1
		do
			table.remove_last;
		end;

	set( symbol: STRING; value: EXPRESSION ) is
		local
			node: SYMBOL_TABLE_NODE;
		do
			node := search_node( symbol );
			if node = Void then
				add( symbol, value );
			else
				node.set_value( value );
			end;
		end;

	add( symbol: STRING; value: EXPRESSION ) is
		local
			node: SYMBOL_TABLE_NODE;
		do
			!!node.make;
			node.set_name( symbol );
			node.set_value( value );
			table.last.add_last( node );
		end;

	get( symbol: STRING ) : EXPRESSION is
		local
			node: SYMBOL_TABLE_NODE;
		do
			node := search_node( symbol );
			if node /= Void then
				Result := node.get_value;
			end;
		end;

feature { SYMBOL_TABLE }

	search_node( symbol: STRING ): SYMBOL_TABLE_NODE is
		local
			blocks, index: INTEGER;
			found: BOOLEAN;
		do
			from blocks := table.count + 1 until blocks = 1 or found
			loop
				blocks := blocks - 1;
				from index := table.item(blocks).count + 1
				until index = 1 or found
				loop
					index := index - 1;
					found := symbol.is_equal(
							table.item(blocks).item(index).get_name );
				end;
			end;
			if found then
				Result := table.item(blocks).item(index);
			end;
		end;

feature { NONE }

	table: LINK_LIST[ LINK_LIST[ SYMBOL_TABLE_NODE ] ];

end -- SYMBOL_TABLE
