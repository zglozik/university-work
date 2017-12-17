%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                      Chapter 11.: Cuts and Negation                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   merge_(Xs,Ys,Zs) :- 
	Zs is an ordered list of integers obtained from merging
	the ordered lists of integers Xs and Ys.
*/
     merge_([X|Xs],[Y|Ys],[X|Zs]) :-
	X < Y, merge_(Xs,[Y|Ys],Zs).
     merge_([X|Xs],[Y|Ys],[X,Y|Zs]) :-
	X =:= Y, merge_(Xs,Ys,Zs).
     merge_([X|Xs],[Y|Ys],[Y|Zs]) :-
	X > Y, merge_([X|Xs],Ys,Zs).
     merge_([X|Xs],[],[X|Xs]).
     merge_([],Xs,Xs).

%  Program 11.1    Merging ordered lists

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   merge(Xs,Ys,Zs) :- 
	Zs is an ordered list of integers obtained from merging
	the ordered lists of integers Xs and Ys.
*/
     merge([X|Xs],[Y|Ys],[X|Zs]) :-
	X < Y, !, merge(Xs,[Y|Ys],Zs).  %green cut
     merge([X|Xs],[Y|Ys],[X,Y|Zs]) :-
	X =:= Y, !, merge(Xs,Ys,Zs).    %green cut
     merge([X|Xs],[Y|Ys],[Y|Zs]) :-
	X > Y, !, merge([X|Xs],Ys,Zs).  %green cut
     merge(Xs,[],Xs) :- !. %red cut
     merge([],Xs,Xs) :- !. %redundant cut

%  Program 11.2    Merging with cuts

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   minimum(X,Y,Min) :- Min is the minimum of the numbers X and Y.
*/
   minimum(X,Y,X) :- X =< Y, !. %Green: Do not make it red!
   minimum(X,Y,Y) :- X > Y, !.

   :- mode minimum1(+,+,-).

   minimum1(X,Y,X) :- X =< Y, !. % red!(?-minimum1(2,3,3).)
   minimum1(_,Y,Y).

   minimum2(X,Y,Z) :- X =< Y, !, Z=X. % red!(?-minimum_(2,3,3).)
   minimum2(_,Y,Y).

%  Program 11.3    Minimum with cuts

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%     :- op( 1200, xfx, [ :-, --> ]).
%     :- op( 1200,  fx, [ :-, ?- ]).
%     :- op( 1150,  fx, [ mode, public, dynamic,
%                         multifile, block, meta_predicate,
%                         parallel, sequential ]).
%     :- op( 1100, xfy, [ ; ]).
%     :- op( 1050, xfy, [ -> ]).
%     :- op( 1000, xfy, [ ',' ]).      /* See note below */
%     :- op(  900,  fy, [ \+, spy, nospy ]).
%     :- op(  700, xfx, [ =, is, =.., ==, \==, @<, @>, @=<, @>=,
%                                     =:=, =\=, <, >, =<, >= ]).
%     :- op(  550, xfy, [ : ]).
%     :- op(  500, yfx, [ +, -, #, /\, \/ ]).
%     :- op(  500,  fx, [ +, - ]).
%     :- op(  400, yfx, [ *, /, //, <<, >> ]).
%     :- op(  300, xfx, [ mod ]).
%     :- op(  200, xfy, [ ^ ]). % Built-in operators in SICStus

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
     polynomial(Term,X) :- Term is a polynomial in X. 
					%for ground terms -- X is atom
*/

     polynomial(X,X) :- !.
     polynomial(Term,X) :- 
        atomic(Term), Term\==X, !.
     polynomial(Term1+Term2,X) :- 
        !, polynomial(Term1,X), polynomial(Term2,X).
     polynomial(Term1-Term2,X) :- 
        !, polynomial(Term1,X), polynomial(Term2,X).
     polynomial(Term1*Term2,X) :- 
        !, polynomial(Term1,X), polynomial(Term2,X).
     polynomial(Term1/Term2,X) :- 
        !, polynomial(Term1,X), atomic(Term2), Term2\==X.
     polynomial(Term ^ N,X) :- 	
        !, integer(N), N >= 0, polynomial(Term,X).	

%  Program 11.4   Recognizing polynomials

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
    interchange_sort(Xs,Ys) :- 
	Ys is an ordered permutation of the list of integers Xs.
*/
    interchange_sort(Xs,Ys) :-
	append(As,[X,Y|Bs],Xs),
	X > Y,
	!,
	append(As,[Y,X|Bs],Xs1),
	interchange_sort(Xs1,Ys).
    interchange_sort(Xs,Xs) :-
	ordered(Xs), !.

    ordered([]).
    ordered([_]).
    ordered([X,Y|Ys]) :- X =< Y, ordered([Y|Ys]).

%  Program 11.5   Interchange sort

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   not X :- X is not provable. "not X" is equivalent to "\+ X" in SICStus.
*/
	:- op(900, fy, [not]).

     not X :- X, !, fail.  % Never gives solution to variables. 
     not _.                % Use only for selection. ***

%  Program 11.6   Negation as failure

% *** See unmarried_student/1 and unmarried_student_/1 bellow:

student(a). student(b).
married(a).

unmarried_student(X) :- student(X), \+ married(X). % It works correctly.

unmarried_student_(X) :- \+ married(X), student(X). 

% It does not work correctly. It fails if any student is married.
% '\+'/1 and not/1 are to be used only for selection.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

all_of(X) :- X, fail.
all_of(_).

for_all(P,S) :- \+ (P,\+S).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   variants(Term1,Term2) :- Term1 and Term2 are variants.
*/
   variants(Term1,Term2) :-
	verify((numbervars_(Term1,0,N), 
		numbervars_(Term2,0,N),
		Term1=Term2)).

	verify(Goal) :- \+ \+ Goal.

% Predicate numbervars_/3 is applicable if 
%     terms of the form '$VAR(_)' are not used in the program.

	numbervars_('$VAR'(N),N,N1) :-
		N1 is N+1, !. %green cut
	numbervars_(Term,N1,N2) :-
		nonvar(Term), !, %redundant cut
		functor(Term,_,N),
		numbervars_(0,N,Term,N1,N2).

	numbervars_(N,N,_,N1,N1) :- !. %green cut
	numbervars_(I,N,Term,N1,N3) :-
		I < N, !, %redundant cut
		I1 is I+1,
		arg(I1,Term,Arg),
		numbervars_(Arg,N1,N2),
		numbervars_(I1,N,Term,N2,N3).

%  Program 11.7  Testing if terms are variants

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   X \= Y :- X and Y are not unifiable.
*/
	:- op(700, xfx, \=).

     X \= X :- !, fail.
     _ \= _.

%  Program 11.8   Implementing \=

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   delete_(Xs,X,Ys) :- 
	Ys is the result of deleting all occurrences of X from the list Xs,
        if Xs and X are ground.
*/
     delete_([X|Xs],X,Ys) :- !, delete_(Xs,X,Ys). %green cut
     delete_([X|Xs],Z,[X|Ys]) :- Z \= X, !, delete_(Xs,Z,Ys). %redundant cut
     delete_([],_,[]).

%   Program 11.9a   Deleting elements from a list

/*
   delete(Xs,X,Ys) :- 
	Ys is the result of deleting all occurrences of X from the list Xs,
        if Xs and X are ground.
*/

     delete([X|Xs],X,Ys) :- !, delete(Xs,X,Ys). %red cut
     delete([X|Xs],Z,[X|Ys]) :- delete(Xs,Z,Ys).
     delete([],_,[]).

%   Program 11.9b   Deleting elements from a list

/*
   delete_unifiables(Xs,Z,Ys) :- 
	Ys is the result of deleting those elements from the list Xs
        that are unifiable with Z.
*/

     delete_unifiables([X|Xs],Z,Ys) :- 
	 verify(X=Z), !, delete_unifiables(Xs,Z,Ys). %red cut
     delete_unifiables([X|Xs],Z,[X|Ys]) :- delete_unifiables(Xs,Z,Ys).
     delete_unifiables([],_,[]).

%   Program 11.9c   Deleting elements from a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   if_then_else(P,Q,R) :- Either P and Q, or not P and R.
*/
     if_then_else(P,Q,_) :- P, !, Q. %red cut
     if_then_else(_,_,R) :- R.

%   Program 11.10    If-then-else statement
%   if_then_else(P,Q,R) corresponds to the construction (P->Q;R)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
  pension(Person,Pension):- Pension is the type of pension received by Person.
*/
     pension(X,old_age_pension) :- over_65(X), paid_up(X).
     pension(X,invalid_pension) :- invalid(X), \+ pension(X,old_age_pension).
     pension(X,supplem_benefit) :- over_65(X), \+ paid_up(X), \+invalid(X).

     invalid(mc_tavish).
     over_65(mc_tavish).      over_65(mc_donald).    over_65(mc_duff).
     paid_up(mc_tavish).      paid_up(mc_donald).

%  Program 11.11a   Determining welfare payments

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% We want to define that Person receives nothing, 
% if Person does not qualify for one of the pensions.

% Procedural "solution" using cuts and default rule:
/*
  pension_(Person,Pension):- Pension is the type of pension received by Person.
*/
     pension_(X,old_age_pension) :- over_65(X), paid_up(X), !.
     pension_(X,invalid_pension) :- invalid(X), !.
     pension_(X,supplem_benefit) :- over_65(X), !.
     pension_(_,nothing).

%     invalid(mc_tavish).
%     over_65(mc_tavish).      over_65(mc_donald).    over_65(mc_duff).
%     paid_up(mc_tavish).      paid_up(mc_donald).

%  Program 11.11b    Determining welfare payments

% This program only works corretcly to determine 
% the pension to which a person is entitled.
% For example: "?- pension_(mc_tavish,nothing)." succeeds, and
% "?- pension_(X,old_age_pension)." has only one solution.
% Cut is useful only for deterministic predicates.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% A better solution: Introduce a new relation "entitlement(X,Y)",
% which is true if X is entitled to Y: (See 11.11a.)

entitlement(X,Y) :- pension(X,Y).
entitlement(X,nothing) :- \+ pension(X,_).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
