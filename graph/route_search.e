-- file: route_search.e
-- an implementation of GRAPH_SEARCH to find the shortest route between
-- two cities

class ROUTE_SEARCH

inherit GRAPH_SEARCH[ CITY ]
	rename
		search as graph_search,
		make as graph_make
end;

creation
	make

feature

	make( pcities: LINK_LIST[CITY] ) is
		-- makes a new ROUTE_SEARCH class
		do
			graph_make;
			cities := clone(pcities);
		end;

	search( source: STRING; dest: STRING ): LINK_LIST[CITY] is
		-- search the road map for the proper route
		local
			index: INTEGER;
		do
			from
				index := 1;
			until
				index > cities.count or else
				source.is_equal( cities.item(index).get_name )
			loop
				index := index + 1;
			end;
			if index > cities.count then
				Result := Void;
			else
				dest_name := dest;
				Result := graph_search( cities.item(index) );
			end;
		end;

feature { GRAPH_SEARCH }

	is_destination( node: GRAPH_NODE[CITY] ): BOOLEAN is
		-- is node destination?
		do
			Result := dest_name.is_equal( node.get_node.get_name );
		end;

	elem( o_n: like open_nodes ): GRAPH_NODE[CITY] is
		-- choose an open node to extend
		local
			index: INTEGER;
			min: INTEGER;
			min_index: INTEGER;
		do
			from
				index := 1;
				min_index := 1;
				min := o_n.item( min_index ).get_length;
			until 
				index > o_n.count
			loop
				if min > o_n.item(index).get_length then
					min_index := index;
					min := o_n.item(min_index).get_length;
				end;
				index := index + 1;
			end;
			Result := o_n.item(min_index);
		end;

feature { NONE }

	dest_name: STRING;

	cities: LINK_LIST[CITY];

end -- ROUTE_SEARCH
