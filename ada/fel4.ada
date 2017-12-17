
with text_io;
with max_generic;
with FIU_package;
procedure fel4 is

    package max_fiu is new max_generic( FIU_package.FIU, FIU_package."<" );
    use max_fiu;
    use FIU_package;
    package int_io is new text_io.integer_io( integer );
    package float_io is new text_io.float_io( float );

    A: FIU( false );
    B: FIU( false );
    C: FIU( true );
    D: FIU( true );
    E: FIU( true );
    F: FIU( false );
    m: FIU;

begin
    set_integer( A, 4 );
    set_integer( B, 3 );
    set_float( C, 3.0 );
    set_float( D, 4.0 );
    set_float( E, 3.0 );
    set_integer( F, 4 );
    m := max_fiu.max( A, B );
    int_io.put( get_integer(m) );
    m := max_fiu.max( C, D );
    float_io.put( get_float(m) );
    m := max_fiu.max( E, F );

exception
    when max_fiu.NOT_COMPARABLE =>
	text_io.put_line( "nem osszehasonlithatoak!" );
    when FIU_package.NOT_THE_SAME =>
	text_io.put_line( "nem megfelelo a valtozo tipusa!" );
end fel4;

