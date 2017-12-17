-- file: city.e
-- a NODE implementation, which can contain a city

class CITY

inherit NODE
	redefine first, next, cost
end;

creation
	make

feature
	
	make( pname: STRING ) is
		-- makes a new city instance
		require
			pname /= Void and then not pname.empty
		do
			name := clone( pname );
			current_elem := 0;
			!!neighbours.make;
			!!costs.make;
		ensure
			name.is_equal( pname );
			neighbours.empty and costs.empty;
		end;

	first: like Current is
		-- gives the first neighbour
		do
			if neighbours.count > 0 then
				current_elem := 1;
				Result := neighbours.first;
			else
				Result := Void;
			end;
		ensure
			current_elem = 1;
		end;

	next: like Current is
		-- gives the next neighbour
		do
			if neighbours.count > current_elem then
				current_elem := current_elem + 1;
				Result := neighbours.item( current_elem );
			else
				Result := Void;
			end;
		ensure
			old neighbours.count = old current_elem or else
			old current_elem + 1 = current_elem;
		end;

	add_neighbour( city: like Current; c: INTEGER ) is
		-- add a new neighbour to the city
		do
			neighbours.add_last( city );
			costs.add_last( c );
		end;

	cost( node: like Current ): INTEGER is
		-- gives -1 or the cost to the node neighbour
		local
			index: INTEGER;
		do
			index := neighbours.index_of( node );
			if index > costs.count then
				Result := -1;
			else
				Result := costs.item( index );
			end;
		end;

	get_name: STRING is
		-- gives the name of the city
		do
			Result := name;
		end;

feature { NONE }

	name: STRING;

	neighbours: LINK_LIST[like Current];

	costs: LINK_LIST[INTEGER];	-- costs to the neighbours

	current_elem: INTEGER;

end -- CITY
