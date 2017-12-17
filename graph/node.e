-- file: node.e
-- abstract NODE class, which can be used as interior graph nodes

deferred class NODE 

feature

	first: like Current is
		-- returns the first neighbour of the node
		deferred
		end;

	next: like Current is
		-- returns the next neighbour of the node, you must call first
		-- before calling this member
		deferred
		end;

	cost( node: like Current ): INTEGER is
		-- returns the cost from this node to the neighbour node
		require 
			node /= Void
		deferred
		end;

end -- NODE

