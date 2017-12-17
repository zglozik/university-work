
with text_io;
use text_io;
procedure train is

task crossing is
    entry felenged;
    entry leenged;
    entry athalad;
end crossing;

task type car is

end car;

type car_ptr is access car;
car_task: car_ptr;

task body crossing is

    type tstate is ( up, down );
    state: tstate := up;

begin
    loop
	select
	    when state = up =>
		accept leenged do
		    state:= down;
		    put_line( "a sorompot leengedtek" );
		end leenged;
	or
	    when state = up and leenged'count = 0 =>
		accept athalad do
		    put_line( "athaladtam a sorompon" );
		end athalad;
	or
	    when state = down =>
		accept felenged do
		    state:= up;
		    put_line( "a sorompot felengedtek" );
		end felenged;
	or
	    terminate;
	end select;
    end loop;
end crossing;

task body car is

    odaer: constant duration := 2.0;
    var:   constant duration := 1.0;

begin
    put_line( "egy kocsi elindult..." );
    delay odaer;
    select
	crossing.athalad;
    or
	delay var;
	put_line( "nem sikerult athaladnom a sorompon" );
    end select;
end car;

begin
    put_line( "szimulacio indul:" );
    for i in 1..5 loop
	car_task:= new car;
    end loop;
    delay 2.0;
    crossing.leenged;
    for i in 1..3 loop
	car_task:= new car;
    end loop;
    delay 2.0;
    crossing.felenged;
end train;
