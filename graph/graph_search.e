-- file: graph_search.e
-- abstract class to search a graph for a route from start node to
-- a destination node

deferred class GRAPH_SEARCH[ N -> NODE ] 

feature

	make is
		-- creates a new instance of GRAPH_SEARCH
		do
			!!open_nodes.make;
			!!graph.make;
		end;

	is_destination( node: GRAPH_NODE[N] ): BOOLEAN is 
		-- returns true if and only the node given is destination node
		require
			node /= Void
		deferred
		end;

	elem( o_n: LINK_LIST[GRAPH_NODE[N]] ): GRAPH_NODE[N] is
		-- choose a node from open nodes to extend
		deferred
		end;

	search( start: N ): LINK_LIST[ N ] is
		-- returns a route from start node to a destination node or
		-- Void if it couldn't find any destination nodes
		local
			graph_node: GRAPH_NODE[N];
		do
			open_nodes.make;
			graph.make;
			from
				!!graph_node.make_fill( start, Void, 0 );
				graph.add_first( graph_node );
				add_to_open_nodes( graph_node );
			until
				open_nodes.empty or else is_destination( graph_node )
			loop
				open_nodes.remove( open_nodes.index_of(graph_node) );
				extend_node( graph_node );
				if not open_nodes.empty then
					graph_node := elem( open_nodes );
				end;
			end;
			if open_nodes.empty then
				Result := Void;
			else
				from
					!!Result.make;
				until
					graph_node = Void
				loop
					Result.add_first( graph_node.get_node );
					graph_node := graph_node.get_ancestor;
				end;
			end;
		ensure
			open_nodes.empty and Result = Void or
			not open_nodes.empty and Result /= Void
		end;

	extend_node( cur: GRAPH_NODE[N] ) is
		-- puts the neighbours of current into the search database and
		-- updates the length of the routes
		require
			cur /= Void
		local
			node: N;
			graph_node: GRAPH_NODE[N];
		do
			from
				node := cur.get_node.first;
			until
				node = Void
			loop
				graph_node := search_graph( node );
				if graph_node = Void or else 
					graph_node.get_length > cur.get_length + 
					cur.get_node.cost( node ) then
					if graph_node = Void then
						!!graph_node.make;
						graph_node.set_node( node );
						graph.add_first( graph_node );
					end;
					graph_node.set_ancestor( cur );
					graph_node.set_length( cur.get_length + 
						cur.get_node.cost( node ) );
					add_to_open_nodes( graph_node );
				end;
				node := cur.get_node.next;
			end;
		end;

	search_graph( node: N ) : GRAPH_NODE[N] is
		-- search the graph database for the node given
		local
			index: INTEGER;
		do
			from
				index := 1;
			until
				index > graph.count or else graph.item(index).get_node = node
			loop
				index := index + 1;
			end;
			if index > graph.count then
				Result := Void;
			else
				Result := graph.item( index );
			end;
		end;

	add_to_open_nodes( graph_node: GRAPH_NODE[N] ) is
		-- if graph_node isn't in the graph database this member will put it
		-- into graph
		local
			index: INTEGER;
		do
			index := open_nodes.index_of( graph_node );
			if index > open_nodes.count then
				open_nodes.add_first( graph_node );
			end;
		end;

feature { NONE }

	open_nodes: LINK_LIST[ GRAPH_NODE[N] ];

	graph: LINK_LIST[ GRAPH_NODE[N] ];

end -- GRAPH_SEARCH
