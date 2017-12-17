
generic
    type T is private;
    with function "<"( X,Y: T ) return boolean;
function max( X,Y: in T ) return T;

function max( X,Y: in T ) return T is

    res: T;

begin
    if X < Y then
	res:= Y;
    else
	res:= X;
    end if;
    return res;
end max;

with text_io;
with max;
procedure fel1 is

    function max_int is new max( integer, "<" );
    package int_io is new text_io.integer_io( integer );

begin
    int_io.put( max_int(3,6) );
    int_io.put( max_int(4,2) );
end fel1;

