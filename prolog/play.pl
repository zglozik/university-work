
szinesz( evi, [okos, szep, gonosz, jo] ).
szinesz( kati, [gonosz, okos] ).
szinesz( laci, [jo, okos, szep] ).
szinesz( sanyi, [szep, jo, gonosz] ).

darab( hofeherke, [gonosz, szep, okos] ).
darab( aladdin, [szep, okos, jo] ).

szerep( X, L ) :- darab(X, L2), megfelel(L, L2).

megfelel( [], [] ).
megfelel( [ [X| [Y| []]] | L1], [Y|L2] ) :- szinesz( X, L3 ), member( Y, L3 ),
	megfelel( L1, L2 ), not(member([X|_], L1)).

osszes(X) :- szerep(X, L), write(L), nl, fail.
osszes(_).

parbeszed :- write( 'valasz:' ),
	write( '1.' ), nl,
	write( '2.' ), nl,
	read(X), elemez(X).
parbeszed.

elemez(X) :- X =:= 1, !, kerdez(Y), szerep(Y,L), write(L), nl.
elemez(X) :- X =:= 2, !, kerdez(Y), osszes(Y).
elemez(X) :- write( 'hulye.' ).

kerdez(Y) :- write( 'hof... vagy aladdin..: ' ), read(Y), elemez2(Y).

elemez2(Y) :- darab(Y,_).
elemez2(Y) :- write( 'hulye2.' ).

