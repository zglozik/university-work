
:- use_module(library(lists), [permutation/2,member/2]).

/* compute derivative */

d(X, 0) :- number(X), !.

d(-X, -DX) :- !, d(X, DX).

d(F, FF*DF2) :- 
	functor(F, FName, 1),
	arg(1, F, F2),
	\+ atom(F2),
	!,
	d(F2, DF2),
	OF =.. [FName,x],
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
	compound(F1),
	!,
	F1 =.. [Name1|Args1],
	change_var(Args1, F2, Args2),
	G =.. [Name1|Args2].

change_var([], _, []).

change_var([A|As], F, [G|Gs]) :- 
	composition(A, F, G),	
	change_var(As, F, Gs).

/* simplify expressions in one pass */

simplify(X, X) :- atomic(X), !.

simplify(X*Y, Z) :- number(X), number(Y), !, Z is X*Y.

simplify(X/Y, Z) :- number(X), number(Y), !, Z is X/Y.

simplify(X+Y, Z) :- number(X), number(Y), !, Z is X+Y.

simplify(X-Y, Z) :- number(X), number(Y), !, Z is X-Y.

simplify(0+F, SF) :- \+ number(F), !, simplify(F, SF).

simplify(F+0, SF) :- \+ number(F), !, simplify(F, SF).

simplify(0-F, -SF) :- \+ number(F), !, simplify(F, SF).

simplify(F-0, SF) :- \+ number(F), !, simplify(F, SF).

simplify(1*F, SF) :- \+ number(F), !, simplify(F, SF).

simplify(F*1, SF) :- \+ number(F), !, simplify(F, SF).

simplify(0*F, 0) :- \+ number(F), !.

simplify(F*0, 0) :- \+ number(F), !.

simplify(F/1, SF) :- \+ number(F), !, simplify(F, SF).

simplify(F/F, 1) :- \+ number(F), !.

simplify(F*G+F*H, SF*SGH) :- !,	simplify(F, SF), simplify(G+H, SGH).

simplify(F*G+H*F, SF*SGH) :- !,	simplify(F, SF), simplify(G+H, SGH).

simplify(G*F+F*H, SF*SGH) :- !,	simplify(F, SF), simplify(G+H, SGH).

simplify(G*F+H*F, SF*SGH) :- !,	simplify(F, SF), simplify(G+H, SGH).

simplify(N*F+F, M*SF) :- number(N), !, simplify(F, SF), M is N+1.

simplify(F*N+F, M*SF) :- number(N), !, simplify(F, SF), M is N+1.

simplify(F+N*F, M*SF) :- number(N), !, simplify(F, SF), M is N+1.

simplify(F+F*N, M*SF) :- number(N), !, simplify(F, SF), M is N+1.

simplify(F+F, 2*SF) :- \+ number(F), !, simplify(F, SF).

simplify(F*(1/G), SF/SG) :- \+ number(F), !, simplify(F, SF), simplify(G, SG).

simplify((1/G)*F, SF/SG) :- \+ number(F), !, simplify(F, SF), simplify(G, SG).

simplify(F*(1/G), SF/SG) :- \+ number(G), !, simplify(F, SF), simplify(G, SG).

simplify((1/G)*F, SF/SG) :- \+ number(G), !, simplify(F, SF), simplify(G, SG).

simplify(N*(M*F), L*SF) :- number(N), number(M), simplify(F, SF), L is N*M.

simplify(N*(F*M), L*SF) :- number(N), number(M), simplify(F, SF), L is N*M.

simplify((M*F)*N, L*SF) :- number(N), number(M), simplify(F, SF), L is N*M.

simplify((F*M)*N, L*SF) :- number(N), number(M), simplify(F, SF), L is N*M.

simplify(N+(M+F), L+SF) :- number(N), number(M), simplify(F, SF), L is N+M.

simplify(N+(F+M), L+SF) :- number(N), number(M), simplify(F, SF), L is N+M.

simplify((M+F)+N, L+SF) :- number(N), number(M), simplify(F, SF), L is N+M.

simplify((F+M)+N, L+SF) :- number(N), number(M), simplify(F, SF), L is N+M.

simplify(_^0, 1) :- !.

simplify(F^1, SF) :- !, simplify(F, SF).

simplify(1^_, 1) :- !.

simplify(F*F*F, SF^3) :- !, simplify(F, SF).

simplify(F^N*F, SF^M) :- number(N), !, simplify(F, SF), M is N+1.

simplify(F*F^N, SF^M) :- number(N), !, simplify(F, SF), M is N+1.

simplify(F^N*F^M, SF^Z) :- number(N), number(M), !, simplify(F, SF), Z is N+M.

simplify(F, G) :- 
	compound(F),
	F =.. [Name|Arg],
	simplify_list(Arg, NewArgs),
	G =.. [Name|NewArgs].

simplify_list([], []).
simplify_list([A|As], [B|Bs]) :- simplify(A, B), simplify_list(As, Bs).

/* simplify expression in several passes */

iterate_simplify(F, G) :-
	simplify(F, G1),
	F \== G1,
	!,
	iterate_simplify(G1, G).

iterate_simplify(F, F).

/* to measure the complexity of an expresssion */

complexity(F, N) :- atomic(F), !, N is 0.

complexity(F, N) :- 
	compound(F),
	!,
	F =.. [_|As],
	complexity_list(As, M),
	functor(F, _, Arity),
	N is M+Arity+1.

complexity_list([], 0).
complexity_list([A|As], N) :-
	complexity(A, N1),
	complexity_list(As, N2),
	N is N1+N2.

/* permutes the order of the parameters of commutative operators */

permute(F, F) :- atomic(F), !.

permute(F, G) :-
	compound(F),
	functor(F, FN, _),
	commutative(FN),
	!,
	F =.. [FN|As],
	permute_list(As, Bs),
	permutation(Bs, Cs),
	G =.. [FN|Cs].

permute(F, G) :- 
	F =.. [FN|As],
	permute_list(As, Bs),
	G =.. [FN|Bs].

permute_list([], []).
permute_list([A|As], [B|Bs]) :- permute(A, B), permute_list(As, Bs).

/* commutative operators */

commutative(+).
commutative(*).

/* forces simplify for all permutations */

fsimplify(F, H) :- iterate_simplify(F, H).

/* main program */

derivate(X, Y) :- d(X, Z), fsimplify(Z, Y).

example :-	
	X = [(1+1)/x, sin(x)*cos(x), (1+3)*ln(x), ln(sin(x)/x), ln(x)*ln(x) ],
	derivate_list(X, _).

derivate_list([], []).
derivate_list([X|Xs], [Y|Ys]) :- 
	derivate(X, Y), 
	write_result(X, Y),
	derivate_list(Xs, Ys).

write_result(X, Y) :- write(X), write(' -> '), write(Y), nl.

/* user interface predicates */

main :-
	ask_term(X),
	process(X).

ask_term(X) :-
	write('enter expression: '),
	read(X).

process(end) :- !.

process(X) :-
	derivate(X, Y),
	write(' -> '),
	write(Y), nl, main.

process(_) :- nl, write('I can not derivate expression.'), nl, main.
