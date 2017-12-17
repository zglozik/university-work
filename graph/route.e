-- file: route.e
-- class to read in a graph which consists of the cities and roads and
-- to find the shortest route between two cities using class ROUTE_SEARCH

class ROUTE 

creation
	make

feature

	make is
		-- does all the tasks
		local
			source, dest: STRING;
		do
			!!cities.make;
			read_cities;
			write_cities;
			!!source.make(80);
			!!dest.make(80);
			from
				std_output.put_string( "indulas: " );
				std_input.read_line;
			until
				std_input.end_of_input
			loop
				source.copy( std_input.last_string );
				std_output.put_string( "erkezes: " );
				std_input.read_line;
				dest.copy( std_input.last_string );
				search( source, dest );
				std_output.put_string( "--------%N" );
				std_output.put_string( "indulas: " );
				std_input.read_line;
			end;
			std_output.put_new_line;
		end;

	search( source: STRING; dest: STRING ) is
		-- search the shortest route between the cities given
		local
			graph: ROUTE_SEARCH[CITY];
			route: LINK_LIST[CITY];
		do
			!!graph.make( cities );
			route := graph.search( source, dest );
			if route = Void then
				std_output.put_string( "nem talaltam utat " );
				std_output.put_string( dest );
				std_output.put_string( " varosba%N" );
			else
				write_route( route );
			end;
		end;

	read_cities is
		-- fills up the cities list from a unix file
		local
			input: STD_FILE_READ;
			source, dest: CITY;
		do
			!!input.connect_to( "input" );
			from
				input.read_line;
			until
				input.end_of_input
			loop
				source := insert_city( input.last_string );
				from
					input.read_line;
				until
					input.last_string.empty
				loop
					dest := insert_city( input.last_string );
					input.read_integer;
					source.add_neighbour( dest, input.last_integer );
					dest.add_neighbour( source, input.last_integer );
					input.read_line;
				end;
				input.read_line;
			end;
			input.disconnect;
		end;

	write_cities is
		-- write out the road map to standard output
		local
			city: CITY;
			index: INTEGER;
		do
			from
				index := 1;
			until
				index > cities.count
			loop
				std_output.put_string( cities.item(index).get_name );
				std_output.put_string( " :%N" );
				from
					city := cities.item(index).first;
				until
					city = Void
				loop
					std_output.put_string( "   " );
					std_output.put_integer( cities.item(index).cost(city) );
					std_output.put_string( " km - " );
					std_output.put_string( city.get_name );
					std_output.put_new_line;
					city := cities.item(index).next;
				end;
				index := index + 1;
			end;
		end;

	insert_city( name: STRING ): CITY is
		-- add a new city to the city database
		local
			city: CITY;
			index: INTEGER;
		do
			from
				index := 1;
			until
				index > cities.count or else 
				name.is_equal( cities.item(index).get_name )
			loop
				index := index + 1;
			end;
			if index > cities.count then
				!!city.make( name );
				cities.add_last( city );
				Result := city;
			else
				Result := cities.item( index );
			end;
		end;			

	write_route( route: LINK_LIST[CITY] ) is
		-- write out the route found to standard output
		local
			index: INTEGER;
			cost, total: INTEGER;
		do
			from
				index := 1;
			until
				index > route.count
			loop
				if index > 1 then
					cost := route.item(index-1).cost( route.item(index) );
					total := total + cost;
					std_output.put_integer( cost );
					std_output.put_string( "km - " );
				end;
				std_output.put_string( route.item(index).get_name );
				std_output.put_new_line;
				index := index + 1;
			end;
			std_output.put_string( "tavolsag: " );
			std_output.put_integer( total );
			std_output.put_string( " km%N" );
		end;

feature { NONE }

	cities: LINK_LIST[CITY];

end -- ROUTE
