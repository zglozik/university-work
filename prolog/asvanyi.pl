
:- use_module(library(lists)).

/* compute derivative */

d(X, 0) :- number(X),!.

d(F, FF*DF2) :- 
	functor(F, FName, 1),
	arg(1, F, F2),
	\+ atom(F2),
        !,
	d(F2, DF2),
	OF =.. [FName|[x]],
	d(OF, DF1),
	composition(DF1, F2, FF).

d(x, 1).

d(X+Y, DX+DY) :- d(X, DX), d(Y, DY).

d(X-Y, DX-DY) :- d(X, DX), d(Y, DY).

d(X*Y, DX*Y+X*DY) :- d(X, DX), d(Y, DY).

d(F/G, (DF*G-F*DG)/G^2) :- d(F, DF), d(G,DG).

/* famous functions */

d(ln(x), 1/(x)).
d(sin(x), cos(x)).
d(cos(x), -sin(x)).
d(tan(x), 1/cos(x)^2).
d(x^Y, Y*x^Y1) :- number(Y), Y1 is Y-1.

/* compute composition of two functions */

composition(X, _, X) :- number(X), !.

composition(x, F, F) :- !.

composition(F1, F2, G) :-
	compound(F1),!,
	F1 =.. [Name1|Arg1],
	change_var(Arg1, F2, Arg2),
	G =.. [Name1|Arg2].

change_var([], _, []).

change_var([A|As], F, [G|Gs]) :- 
	composition(A, F, G),	
	change_var(As, F, Gs).

/* simplify expressions in one pass */

not_number(X) :- number(X), !, fail.
not_number(_).

simplify(X, X) :- atomic(X), !.
simplify(X*Y, Z) :- number(X), number(Y), !, Z is X*Y.
simplify(X/Y, Z) :- number(X), number(Y), !, Z is X/Y.
simplify(X+Y, Z) :- number(X), number(Y), !, Z is X+Y.
simplify(X-Y, Z) :- number(X), number(Y), !, Z is X-Y.

simplify(1*F, F) :- not_number(F), !.
simplify(0*F, 0) :- not_number(F), !.
simplify(0+F, F) :- not_number(F), !.
simplify(F/1, F) :- not_number(F), !.
simplify(F/F, 1) :- not_number(F), !.
simplify(F*(1/G), F/G) :- not_number(F), !.
simplify(F*(1/G), F/G) :- not_number(G), !.
simplify(_^0, 1) :- !.
simplify(X^1, X) :- !.
simplify(1^_, 1) :- !.
simplify(X*X*X, X^3) :- !.
simplify(X^N*X, X^M) :- number(N), !, M is N+1.
simplify(X^N*X^M, X^Z) :- number(N), number(M), !, Z is N+M.

simplify(F, G) :- 
	compound(F),
	F =.. [Name|Arg],
	simplify_list( Arg, NewArg ),
	G =.. [Name|NewArg].

simplify_list([], []).
simplify_list([A|As], [B|Bs]) :- simplify(A, B), simplify_list(As, Bs).

/* simplify expression in several passes */

iterate_simplify(F, G) :-
	complexity(F, FC),
	simplify(F, G1),
	complexity(G1, G1C),
	FC > G1C,
	iterate_simplify(G1, G).

iterate_simplify(F, G) :-
	simplify(F, G),
	complexity(F, FC),
	complexity(G, GC),
	FC =:= GC.

/* to measure the complexity of an expresssion */

complexity(F, N) :- 
	atomic(F), !, N is 0.

complexity(F, N) :- 
	compound(F), !, 
	F =.. [_|As],
	complexity_list( As, M ),
	functor(F, _, Arity),
	N is M+Arity.

complexity_list([], 0).
complexity_list([A|As], N) :-
	complexity(A, N1),
	complexity_list(As, N2),
	N is N1+N2.

/* permutes the order of the parameters of commutative operators */

permute(F, F).

permute(F, G) :-
	compound(F),
	functor(F, FN, _),
	commutative(FN), !,
	F =.. [FN|As],
	permute_list(As, Bs),
	permutation(Bs, Cs),
	G =.. [FN|Cs].

permute(F, G) :- 
	compound(F),
	F =.. [FN|As],
	permute_list(As, Bs),
	G =.. [FN|Bs].

permute_list([], []).
permute_list([A|As], [B|Bs]) :- permute(A, B), permute_list(As, Bs).

/* commutative operators */

commutative(+).
commutative(*).

/* force to simplify */
/*
fsimplify(F, G, L) :-
	permute(F, F2),
	iterate_simplify(F2, G),
	min_complexity(F, G).

min_complexity(F, G) :-
	permute(F, F2),
	iterate_simplify(F2, H),
	complexity(G, GC),
	complexity(H, HC),
	GC > HC, !, fail.

min_complexity(_, _).
*/

fsimplify(F, G1, M, L) :-
	%complexity(G1, G1C),
	findall(F2,permute(F, F2),L2),
	list_simplify(L2,M).


	\+ member(F2, L),
	iterate_simplify(F2, G2),
	complexity(G2, G2C),
	min_complexity(G1C, G2C, G1, G2, M1),
	append(L, [F2], L2),
	fsimplify(F, M1, M, L2).

fsimplify(_, M, M, _).

min_complexity(G1C, G2C, G1, _, G1) :- G1C < G2C, !.
min_complexity(G1C, G2C, _, G2, G2) :- G2C =< G1C, !.

/* main program */

derivate(X, Y) :- d(X, Z), fsimplify(Z, Z, Y, []).

example :-	
	X = [(1+1)/x, sin(x)*cos(x), (1+3)*ln(x), ln(sin(x)/x), ln(x)*ln(x) ],
	derivate_list(X, _).

derivate_list([], []).
derivate_list([X|Xs], [Y|Ys]) :- 
	derivate(X, Y), 
	write_result(X, Y),
	derivate_list(Xs, Ys).

write_result(X, Y) :- write(X), write(' -> '), write(Y), nl.
