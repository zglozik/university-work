%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%           Art of Prolog, Chapter 9.: Basics of Structure Handling           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   flatten(Xs,Ys) :- Ys is a list of the atomic elements of the ground list Xs.
*/

    flatten([X|Xs],Ys) :- 
        flatten(X,Ys1), flatten(Xs,Ys2), append(Ys1,Ys2,Ys).
    flatten(X,[X]) :-
	atomic(X), X \== [].
    flatten([],[]).

% Program 9.1a   Flattening a list with double recursion

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   flatten(Xs,Ys) :- Ys is a list of the elements of Xs. 
                     Variable elements are not allowed. 
                     List elements beeing lists are considered to be sublists.
*/
     flatten_(Xs,Ys) :- flatten(Xs,[],Ys).

% flatten(L1,L2,Flat) :- Flat is the flattened form of (L1 conc L2).

     flatten([X|Xs],S,Ys) :- 
		list(X), flatten(X,[Xs|S],Ys).
     flatten([X|Xs],S,[X|Ys]) :- 
		\+ list(X), flatten(Xs,S,Ys).
     flatten([],[X|S],Ys) :- 
		flatten(X,S,Ys).
     flatten([],[],[]).

     list([_|_]).
     list([]).

% Program 9.1b   Flattening a list using a stack

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   subterm(Sub,Term) :- Sub is a subterm of the ground term Term.
*/
     subterm(Term,Term).
     subterm(Sub,Term) :- 
        compound(Term), functor(Term,_,N), subterm(N,Sub,Term).

     subterm(N,Sub,Term) :- 
        N > 1, N1 is N-1, subterm(N1,Sub,Term).
     subterm(N,Sub,Term) :- 
        arg(N,Term,Arg), subterm(Sub,Arg).

%  Program 9.2    Finding subterms of a term

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*   
  substitute(Old,New,OldTerm,NewTerm) :- NewTerm is the result of replacing
	all occurences of Old in OldTerm by New.	
*/
     substitute(Old,New,Term,New) :- Term == Old.
     substitute(Old,_,Term,Term) :- 
        simple(Term), Term \== Old.
     substitute(Old,New,Term,Term1) :-
        Term \== Old,
        compound(Term), 
        functor(Term,F,N), 
        functor(Term1,F,N),
        substitute(N,Old,New,Term,Term1).

	substitute(N,Old,New,Term,Term1) :-
            N > 0,
	    arg(N,Term,Arg),
	    substitute(Old,New,Arg,Arg1),
	    arg(N,Term1,Arg1),
	    N1 is N-1,
	    substitute(N1,Old,New,Term,Term1).
        substitute(0,_,_,_,_).

%  Program 9.3  A program for substituting in a term

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   subterm2(Sub,Term) :- Sub is a subterm of the ground term Term.
*/
     subterm2(Term,Term).
     subterm2(Sub,Term) :- 
        compound(Term), Term =.. [_|Args], subterm_list(Sub,Args).

     subterm_list(Sub,[Arg|_]) :- 
        subterm2(Sub,Arg).
     subterm_list(Sub,[_|Args]) :-
	subterm_list(Sub,Args).

%  Program 9.4   Subterm defined using univ

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   univ_tl(Term, List) :- List is a list containing the functor of Term 
                         followed by the arguments of Term.
*/

:- mode univ_tl(+,?).

     univ_tl(Term, [F|Args]) :-
		functor(Term,F,N), args_tl(0,N,Term,Args).

     args_tl(I,N,Term,[Arg|Args]) :-
		I < N, I1 is I+1, arg(I1,Term,Arg), args_tl(I1,N,Term,Args).
     args_tl(N,N,_,[]).

%  Program 9.5a   Constructing a list corresponding to a term


/*
   univ_lt(List, Term) :- 
	The functor of Term is the first element of the list List, 
	and its arguments are the rest of List's elements.
*/

:- mode univ_lt(+,?).

     univ_lt( [F|Args], Term ) :-
	atom(F), length(Args,N), functor(Term,F,N), args_lt(Args,Term,1).

     args_lt([Arg|Args],Term,N) :-
		arg(N,Term,Arg), N1 is N+1, args_lt(Args,Term,N1).
     args_lt([],_,_).


/*
   length(Xs,N) :- N is the length of the list Xs.
*/
%     length(L,N) :- length(L,0,N).   % Built_in in SICStus

%     length([],N,N).
%     length([_|Xs],N0,N) :-  N1 is N0+1, length(Xs,N1,N).


%  Program 9.5b   Constructing a term corresponding to a list

univ(Term,List) :- compound(Term), univ_tl(Term,List).
univ(Term,List) :- var(Term), univ_lt(List,Term).
univ(Term,[Term]) :- atomic(Term).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
