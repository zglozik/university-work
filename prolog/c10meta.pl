%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                           Meta-Logical Predicates                           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Predicates for checking the types of parameters:
% ------------------------------------------------

/*
   plus(X,Y,Z) :- The sum of the numbers X and Y is Z.
*/
   plus(X,Y,Z) :- nonvar(X), nonvar(Y), Z is X + Y.
   plus(X,Y,Z) :- nonvar(X), nonvar(Z), Y is Z - X.
   plus(X,Y,Z) :- nonvar(Y), nonvar(Z), X is Z - Y.

%   Program 10.1   Multiple uses for plus
% Overcomes the difficulty coming from the feature of is/2.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   length_(Xs,N) :- The list Xs has length N.
*/
   length_(Xs,N) :- var(N), length1(Xs,0,N).
   length_(Xs,N) :- integer(N), N>=0, length2(Xs,N).

/*
   length1(Xs,N0,N) :- N is N0 + the length of the list Xs.
*/
     length1([],N,N).
     length1([_|Xs],N0,N) :- N1 is N0+1, length1(Xs,N1,N).

%  length1([],0).
%  length1([_|Xs],N1) :- length1(Xs,N), N1 is N+1.

/*
   length2(Xs,N) :- Xs is a list of length N.
*/
     length2([_|Xs],N) :- N > 0, N1 is N-1, length2(Xs,N1).
     length2([],0).

%  Program 10.2    A multipurpose length program

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   grandparent(X,Z) :- X is the grandparent of Z.
*/
   grandparent(X,Z) :- nonvar(X), parent(X,Y), parent(Y,Z).
   grandparent(X,Z) :- var(X), nonvar(Z), parent(Y,Z), parent(X,Y).
   grandparent(X,Z) :- var(X), var(Z), parent(X,Y), parent(Y,Z).

%  Program 10.3  A more efficient version of grandparent

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/* 
     ground_(Term) :- Term is a ground term.
*/
     ground_(Term) :- 
	atomic(Term).
     ground_(Term) :- 
	compound(Term),
	functor(Term,_,N),
	ground(N,Term).
	
     ground(N,Term) :-
        N > 0,
	arg(N,Term,Arg),
	ground_(Arg),
	N1 is N-1,
	ground(N1,Term).
     ground(0,_).

%  Program 10.4   Testing if a term is ground

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Term comparison and manipulation:
% ---------------------------------

/*
   unify_(Term1,Term2) :- 
	Term1 and Term2 are unified, ignoring the occurs check.
*/
   unify_(X,Y) :-
	var(X), var(Y), X@>Y, X=Y.
   unify_(X,Y) :-
	var(X), var(Y), X@<Y, Y=X.   
   unify_(X,Y) :-
	var(X), nonvar(Y), X=Y.
   unify_(X,Y) :-
	var(Y), nonvar(X), Y=X.
   unify_(X,Y) :-
	nonvar(X), nonvar(Y), atomic(X), atomic(Y), X==Y.
   unify_(X,Y) :-
	nonvar(X), nonvar(Y), compound(X), compound(Y), term_unify_(X,Y).

   term_unify_(X,Y) :-
	functor(X,F,N), functor(Y,F,N), unify__args(N,X,Y).

   unify__args(N,X,Y) :-
	N > 0, unify__arg(N,X,Y), N1 is N-1, unify__args(N1,X,Y).
   unify__args(0,_,_).

   unify__arg(N,X,Y) :-
	arg(N,X,ArgX), arg(N,Y,ArgY), unify_(ArgX,ArgY).


%  Program 10.5    Unification algorithm without occurs check

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   unify(Term1,Term2) :- Term1 and Term2 are unified with the occurs check.
*/
   unify(X,Y) :-
	var(X), var(Y), X=Y.
   unify(X,Y) :-
	var(X), nonvar(Y), not_occurs_in(X,Y), X=Y.
   unify(X,Y) :-
	var(Y), nonvar(X), not_occurs_in(Y,X), Y=X.
   unify(X,Y) :-
	/*nonvar(X), nonvar(Y),*/ atomic(X), atomic(Y), X==Y.
   unify(X,Y) :-
	/*nonvar(X), nonvar(Y),*/ compound(X), compound(Y), term_unify(X,Y).

   not_occurs_in(X,Y) :- 
	var(Y), X \== Y.
   not_occurs_in(_,Y) :- 
	/*nonvar(Y),*/ atomic(Y).
   not_occurs_in(X,Y) :-
	/*nonvar(Y),*/ compound(Y), functor(Y,_,N), not_occurs_in(N,X,Y).

   not_occurs_in(N,X,Y) :-
	N > 0, arg(N,Y,Arg), not_occurs_in(X,Arg), N1 is N-1, 
			not_occurs_in(N1,X,Y).
   not_occurs_in(0,_,_).

   term_unify(X,Y) :-
	functor(X,F,N), functor(Y,F,N), unify_args(N,X,Y).

   unify_args(N,X,Y) :-
	N > 0, unify_arg(N,X,Y), N1 is N-1, unify_args(N1,X,Y).
   unify_args(0,_,_).

   unify_arg(N,X,Y) :-
	arg(N,X,ArgX), arg(N,Y,ArgY), unify(ArgX,ArgY).

%  Program 10.6    Unification with the occurs check

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   occurs_in(Sub,Term) :- 
	Sub is a subterm of the (possibly non-ground term) Term.
*/
% a:  Using ==

   occurs_in(X,Term) :- subterm(Sub,Term), X == Sub.

% b:  Using numbervars

   occurs_in_(X,Term) :- 
        copy_term((X,Term),(X2,Term2)),
	numbervars(Term2,0,_), ground(X2), subterm(X2,Term2).

% How to write occurs_in_/2 in another way?

/*
   subterm(Sub,Term) :- Sub is a subterm of the term Term.
                     Sub is a new variable or Term is ground when invoking
                     this predicate, if Term is not to be affected.
*/
     subterm(Term,Term).
     subterm(Sub,Term) :- 
        compound(Term), functor(Term,_,N), subterm(N,Sub,Term).

     subterm(N,Sub,Term) :- 
        arg(N,Term,Arg), subterm(Sub,Arg).
     subterm(N,Sub,Term) :- 
        N > 1, N1 is N-1, subterm(N1,Sub,Term).

%  Program 10.7    Occurs in

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   numbervars_(Term,N1,N2) :- 
	The variables in Term are numbered from N1 to N2-1.
*/

	numbervars_('$VAR'(N),N,N1) :- N1 is N+1.
	numbervars_(Term,N1,N2) :-
	   nonvar(Term), functor(Term,_,N), numbervars_(0,N,Term,N1,N2).

	numbervars_(N,N,_,N1,N1).
	numbervars_(I,N,Term,N1,N3) :-
		I < N,
		I1 is I+1,
		arg(I1,Term,Arg),
		numbervars_(Arg,N1,N2),
		numbervars_(I1,N,Term,N2,N3).

%  Program 10.8: Numbering the variables in a term

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% The meta-variable facility:
% ---------------------------

/*
    X v Y :- X or Y.
*   
*   Note: used 'v' instead of ';' as ';' is a system predicate.
*/
	:- op(1100,yfx,[v]).

    X v _ :- X.
    _ v Y :- Y.

%   Program 10.9    Logical disjunction

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

parent(X,Y) :- father(X,Y) v mother(X,Y).

father(a,c). father(c,e). father(c,f).
mother(b,c). mother(d,e). mother(d,f).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
