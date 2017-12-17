
package FIU_package is

type FIU( float_type: boolean := false ) is private;
NOT_THE_SAME: exception;

    function "<"( X,Y: in FIU ) return boolean;
    procedure set_float( o: out FIU; X: in float );
    procedure set_integer( o: out FIU; X: in integer );
    function get_integer( o: in FIU ) return integer;
    function get_float( o: in FIU ) return float;

private
    type FIU( float_type: boolean := false ) is
	record
	    case float_type is
		when true =>
		    float_num: float := 0.0;
		when false =>
		    int_num: integer := 0;
	    end case;
	end record;

end FIU_package;

package body FIU_package is

function "<"( X,Y: in FIU ) return boolean is
    res: boolean;
begin
    if X.float_type and Y.float_type then
	res := X.float_num < Y.float_num;
    else
	if not X.float_type and not Y.float_type then
	    res := X.int_num < Y.int_num;
	else
	    raise NOT_THE_SAME;
	end if;
    end if;
    return res;
end "<";

procedure set_float( o: out FIU; X: in float ) is

begin
    if not o.float_type then
	raise NOT_THE_SAME;
    else
	o.float_num := X;
    end if;
end set_float;

procedure set_integer( o: out FIU; X: in integer ) is

begin
    if o.float_type then
	raise NOT_THE_SAME;
    else
        o.int_num := X;
    end if;
end set_integer;

function get_integer( o: in FIU ) return integer is

begin
    if o.float_type then
	raise NOT_THE_SAME;
    else
	return o.int_num;
    end if;
end get_integer;

function get_float( o: in FIU ) return float is

begin
    if o.float_type then
	return o.float_num;
    else
	raise NOT_THE_SAME;
    end if;
end get_float;
end FIU_package;
