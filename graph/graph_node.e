-- file: graph_node.e
-- it is based on a NODE class, and it encapsulates the length and parent of
-- an interior node in the graph

class GRAPH_NODE[ N ] 

creation
	make, make_fill

feature

	make is
		do
		end;

	make_fill( pnode: N; pparent: GRAPH_NODE[N]; plen: INTEGER ) is
		do
			make;
			set_node( pnode );
			set_ancestor( pparent );
			set_length( plen );
		end;

	set_ancestor( pparent: GRAPH_NODE[ N ] ) is
		do
			parent := pparent;
		ensure
			parent = pparent
		end;

	get_ancestor: GRAPH_NODE[ N ] is
		do
			Result := parent;
		ensure
			Result = parent
		end;

	set_node( pnode: N ) is
		do
			node := pnode;
		ensure
			node = pnode
		end;

	get_node: N is
		do
			Result := node;
		ensure
			Result = node
		end;

	set_length( plen: INTEGER ) is
		do
			length := plen;
		ensure
			length = plen
		end;

	get_length: INTEGER is
		do
			Result := length;
		ensure
			Result = length
		end;

feature { NONE }

	node:	N;
	parent: GRAPH_NODE[ N ];
	length: INTEGER;

end -- GRAPH_NODE

