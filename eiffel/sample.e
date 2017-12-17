
class SAMPLE

creation
	make

feature

	s1, s2, s3: INTEGER is unique

	make is
	do
		proc
	end

	proc is
	local
		i: INTEGER
		state: INTEGER
	do
		state := s3;
		inspect state
		when s1 then
			std_output.put_string( "s1" )
		when s2 then
			std_output.put_string( "s2" )
		else
			std_output.put_string( "else" )
		end
		std_output.put_new_line
		from
			i := 0
		until
			i = 10
		loop
			std_output.put_integer( i )
			std_output.put_new_line
			i := i+1
		end -- loop	
	end

end -- SAMPLE

