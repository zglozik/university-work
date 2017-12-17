
generic
    type T is private;
    with function "<"( X,Y: T ) return boolean;
package max_generic is

    NOT_COMPARABLE: exception;

    function max( X,Y: in T ) return T;

end max_generic;

package body max_generic is
function max( X,Y: in T ) return T is
    res: T;
begin
    if X < Y then
	res:= Y;
    else
	if Y < X then
	    res:= X;
	else
	    raise NOT_COMPARABLE;
	end if;
    end if;
    return res;
end max;
end max_generic;

with text_io;
with max_generic;
procedure fel2 is

    package max_int is new max_generic( integer, "<" );
    package int_io is new text_io.integer_io( integer );

begin
    int_io.put( max_int.max(3,6) );
    int_io.put( max_int.max(4,2) );
    int_io.put( max_int.max(5,5) );
    int_io.put( max_int.max(5,7) );

exception
    when max_int.NOT_COMPARABLE =>
	text_io.put_line( "nem osszehasonlithatoak!" );
end fel2;

