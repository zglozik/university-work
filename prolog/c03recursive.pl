%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         Recursive Logic Programs Implemented in SICStus Prolog 3         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/* 
	natural_number(X) :- X is a natural number.
*/

	natural_number(0).
	natural_number(s(X)) :- natural_number(X).

%	Program 3.1:  Defining the natural numbers

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	X lesseq Y :-  X and Y are natural numbers,
	  		  such that X is less than or equal to Y.

We use lesseq to represent the operator rather than cause problems
with redefining a built-in Prolog operator!
*/

	:- op( 700, xfx, [ lesseq, less ] ).

	0 lesseq X :- natural_number(X).
	s(X) lesseq s(Y) :- X lesseq Y.

%	Program 3.2: The less than or equal relation

	0 less s(X) :- natural_number(X).
	s(X) less s(Y) :- X less Y.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	plus(X,Y,Z) :-
		X, Y and Z are natural numbers
		such that Z is the sum of X and Y.
*/

	plus(0,X,X) :- natural_number(X).
	plus(s(X),Y,s(Z)):- plus(X,Y,Z).

%	Program 3.3: Addition

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	times(X,Y,Z) :-
		X, Y and Z are natural numbers
		such that Z is the product of X and Y
*/

	times(0,_,0).
	times(s(X),Y,Z) :- times(X,Y,XY), plus(XY,Y,Z).

%	Program 3.4: Multiplication as repeated addition

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	exp(N,X,Y) :-
			N, X and Y are natural numbers
			such that Y equals X raised to the power N.
*/

	exp(s(_),0,0).
	exp(0,s(_),s(0)).
	exp(s(N),X,Y) :- exp(N,X,Z), times(Z,X,Y).

%	Program 3.5: Exponentiation as repeated multiplication

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	factorial(N,F) :- F equals N factorial.
*/

	factorial(0,s(0)).
	factorial(s(N),F) :- factorial(N,F1), times(s(N),F1,F).

%	Program 3.6: Computing factorials

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	minimum(N1,N2,Min) :- 
		The minimum of natural numbers N1 and N2 is Min.

We use lesseq to represent the operator rather than cause problems
with an error message from Prolog about redefining an operator!
*/

	minimum(N1,N2,N1) :- N1 lesseq N2.
	minimum(N1,N2,N2) :- N2 lesseq N1.

%	Program 3.7: The minimum of two numbers

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	rem(X,Y,Z) :-
		Z is the remainder of the integer division of X by Y.
*/

	rem(X,Y,Z) :- Z less Y, times(Y,_,Y_), plus(Y_,Z,X).

%	Program 3.8a: A nonrecursive definition of modulus

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	mod(X,Y,Z) :-
		Z is the remainder of the integer division of X and Y.
*/

	mod(X,Y,X):- X less Y.
	mod(X,Y,Z) :- plus(X1,Y,X), mod(X1,Y,Z).

%	Program 3.8b: A recursive definition of modulus

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	ackermann(X,Y,A) :-
		A is the value of Ackermann's function for
		the natural numbers X and Y.
*/

	ackermann(0,N,s(N)).
	ackermann(s(M),0,Val) :- ackermann(M,s(0),Val).
	ackermann(s(M),s(N),Val) :-
		ackermann(s(M),N,Val1), ackermann(M,Val1,Val).

%	Program 3.9: Ackermann's function

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	gcd(X,Y,Z) :- Z is the greatest common divisor of the 
			      natural numbers X and Y.
*/

	gcd(X,Y,Gcd) :- mod(X,Y,Z), gcd(Y,Z,Gcd).
	gcd(X,0,X) :- 0 less X.

%	gcd(X,Y,Gcd) :- Z is X mod Y, gcd(Y,Z,Gcd).
%	gcd(X,0,X) :- X > 0.

%	Program 3.10: The Euclidean algorithm

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	list(Xs) :- Xs is a list.
*/

	list([]).
	list([_|Xs]) :- list(Xs).

%	Program 3.11: Defining a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	member(Element,List) :- Element is an element of the list List
*/

	member(X,[X|_]).
	member(X,[_|Ys]) :- member(X,Ys).

%	Program 3.12: Membership of a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	prefix(Prefix,List) :- Prefix is a prefix of List.
*/

	prefix([],_).
	prefix([X|Xs],[X|Ys]) :- prefix(Xs,Ys).

/*
	suffix(Suffix,List) :- Suffix is a suffix of List.
*/

	suffix(Xs,Xs).
	suffix(Xs,[_|Ys]) :- suffix(Xs,Ys).

%	Program 3.13: Prefixes and suffixes of a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	sublist_x(Sub,List) :- Sub is a continuous sublist of List.
*/

	% a: Suffix of a prefix
	sublist_a(Xs,Ys) :- prefix(Ps,Ys), suffix(Xs,Ps).

	% b: Prefix of a suffix
	sublist_b(Xs,Ys) :- suffix(Ss,Ys), prefix(Xs,Ss).

	% c: Recursive definition of a sublist
	sublist_c(Xs,Ys) :- prefix(Xs,Ys).
	sublist_c(Xs,[_|Ys]) :- sublist_c(Xs,Ys).

	% d: Prefix of a suffix, using append
	sublist_d(Xs,_Xs_) :-
		append(_,Xs_,_Xs_), append(Xs,_,Xs_).

	% e: Suffix of a prefix, using append
	sublist_e(Xs,_Xs_) :-
		append(_Xs,_,_Xs_), append(_,Xs,_Xs).

%	Program 3.14: Determining sublists of lists

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	append(Xs,Ys,XsYs) :-
		XsYs is the result of concatening the lists Xs and Ys.
*/

	append([],Ys,Ys).
	append([X|Xs],Ys,[X|Zs]) :- append(Xs,Ys,Zs).

%	Program 3.15: Appending two lists

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	reverse(List,Tsil):-
		Tsil is the result of reversing the list List.
*/
	% a: Naive reverse
	naive_reverse([],[]).
	naive_reverse([X|Xs],Zs) :- naive_reverse(Xs,Ys), append(Ys,[X],Zs).

	% b: Reverse-accumulate

	reverse(Xs,Ys):- rev_app(Xs,[],Ys).

	rev_app([X|Xs],Acc,Ys) :- rev_app(Xs,[X|Acc],Ys).
	rev_app([],Ys,Ys).

% rev_app(L1,L2,RevL1_L2) :- 
%     RevL1_L2 is the concatenation 
%     of the reversed list of L1 and of the list L2.

%	Program 3.16: Reversing a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	length_(Xs,N) :- The list Xs has N elements.
*/

	length_([],0).
	length_([_|Xs],s(N)) :- length_(Xs,N).

%	Program 3.17: Determining the length of a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

length2([],0).
length2([_|Xs],N) :- length2(Xs,N1), N is N1+1.

length3(L,N) :- length3(L,0,N).

length3([],S,S).
length3([_|Xs],A,S) :- A1 is A+1, length3(Xs,A1,S).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	delete(List,X,HasNoXs) :-
		The list HasNoXs is the result of removing all
		occurrences of X from the list List.
*/

	delete([X|Xs],X,Ys) :- delete(Xs,X,Ys).
	delete([X|Xs],Z,[X|Ys]) :- \+ X = Z, delete(Xs,Z,Ys).
	delete([],_,[]).

%	Program 3.18: Deleting all occurrences of an element from a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*		
	select(X,HasXs,OneLessXs) :- 
		The list OneLessXs is the result of removing one 
		occurrence of X from the list HasXs.
*/

	select(X,[X|Xs],Xs).
	select(X,[Y|Ys],[Y|Zs]) :- select(X,Ys,Zs).

%	Program 3.19: Selecting an element from a list

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*			       
	permSort(Xs,Ys) :- 
		The list Ys is an ordered permutation of the list Xs.
*/
	
	permSort(Xs,Ys) :- permutation(Xs,Ys), ordered(Ys).

	permutation(Xs,[Z|Zs]) :- select(Z,Xs,Ys), permutation(Ys,Zs).
	permutation([],[]).

	ordered([]).
	ordered([_]).
	ordered([X,Y|Ys]) :- X =< Y, ordered([Y|Ys]).

%	Program 3.20 Permutation sort

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	insertSort(Xs,Ys) :- 
		The list Ys is an ordered permutation of the list Xs.
*/

	insertSort([X|Xs],Ys) :- insertSort(Xs,Zs), insert(X,Zs,Ys).
	insertSort([],[]).

	insert(X,[],[X]).
	insert(X,[Y|Ys],[X,Y|Ys]) :- X =< Y.
	insert(X,[Y|Ys],[Y|Zs]) :- X > Y, insert(X,Ys,Zs).

%	Program 3.21: Insertion sort

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% green cuts
%	insert(X,[],[X]) :- !.
%	insert(X,[Y|Ys],[X,Y|Ys]) :- X =< Y, !.
%	insert(X,[Y|Ys],[Y|Zs]) :- X > Y, !, insert(X,Ys,Zs).

% red cut
%	insert(X,[Y|Ys],[Y|Zs]) :- X > Y, !, insert(X,Ys,Zs).
%	insert(X,L,[X|L]).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	quickSort(Xs,Ys) :- 
		The list Ys is an ordered permutation of the list Xs.
*/
	quickSort([X|Xs],Ys) :-
		partition(Xs,X,Littles,Bigs),
		quickSort(Littles,Ls),
		quickSort(Bigs,Bs),
		append(Ls,[X|Bs],Ys).
	quickSort([],[]).

	partition([X|Xs],Y,[X|Ls],Bs) :- X =< Y, partition(Xs,Y,Ls,Bs).
	partition([X|Xs],Y,Ls,[X|Bs]) :- X >  Y, partition(Xs,Y,Ls,Bs).
	partition([],_,[],[]).

%	Program 3.22: Quicksort

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	binary_tree(Tree) :- Tree is a binary tree.
*/
	binary_tree(void).
	binary_tree(tree(_,Left,Right)) :-
		binary_tree(Left), binary_tree(Right).

%	Program 3.23: Defining binary trees

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	tree_member(Element,Tree):-
		Element is an element of the binary tree Tree
*/
	tree_member(X,tree(X,_,_)).
	tree_member(X,tree(_,Left,_)) :- tree_member(X,Left).
	tree_member(X,tree(_,_,Right)) :- tree_member(X,Right).

%	Program 3.24: Testing tree membership

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	isotree(Tree1,Tree2) :- 
		Tree1 and Tree2 are isomorphic binary trees
*/
	isotree(void,void).
	isotree(tree(X,Left1,Right1),tree(X,Left2,Right2)) :- 
		isotree(Left1,Left2), isotree(Right1,Right2).
	isotree(tree(X,Left1,Right1),tree(X,Left2,Right2)) :- 
		isotree(Left1,Right2), isotree(Right1,Left2).
	
%	Program 3.25: Determining when trees are isomorphic

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*		
	substitute(X,Y,TreeX,TreeY) :- 
		The binary tree TreeY is the result of replacing all 
		occurrences of X in the binary tree TreeX by Y.
*/

	substitute(_,_,void,void).
	substitute(X,Y,tree(Leaf,Left,Right),tree(Leaf1,Left1,Right1)) :-
		replace(X,Y,Leaf,Leaf1),
		substitute(X,Y,Left,Left1), 
		substitute(X,Y,Right,Right1).

	replace(X,Y,X,Y).
	replace(X,_,Z,Z) :- \+ X = Z.

%	Program 3.26: Substituting for a term in a tree

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	preorder(Tree,Pre) :- 
		Pre is a preorder traversal of the binary tree Tree.
*/
	preorder(tree(X,L,R),Xs) :-
		preorder(L,Ls), preorder(R,Rs), append([X|Ls],Rs,Xs).
	preorder(void,[]).

/*
	inorder(Tree,In) :- 
		In is an inorder traversal of the binary tree Tree.
*/
	inorder(tree(X,L,R),Xs) :-
		inorder(L,Ls), inorder(R,Rs), append(Ls,[X|Rs],Xs).
	inorder(void,[]).
/*
	postorder(Tree,Post) :- 
		Post is a postorder traversal of the binary tree Tree.
*/
	postorder(tree(X,L,R),Xs) :-
		postorder(L,Ls), 
		postorder(R,Rs),
		append(Rs,[X],Rs1), 
		append(Ls,Rs1,Xs).
	postorder(void,[]).

%	Program 3.27: Traversals of a binary tree

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	heapify(Tree,Heap) :-
	  The elements of the complete binary tree Tree have been adjusted
	  to form the binary tree Heap, which has the same shape as Tree 
	  and satisfies the heap property that the value of each parent node
	  is greater than or equal to the values of its children.
*/

heapify(void,void).
heapify(tree(X,L,R),Heap) :-
   heapify(L,HeapL), heapify(R,HeapR), adjust(X,HeapL,HeapR,Heap).

adjust(X,HeapL,HeapR,tree(X,HeapL,HeapR)) :-
    greater(X,HeapL), greater(X,HeapR).
adjust(X,tree(X1,L,R),HeapR,tree(X1,HeapL,HeapR)) :-
    X < X1, greater(X1,HeapR), adjust(X,L,R,HeapL).
adjust(X,HeapL,tree(X1,L,R),tree(X1,HeapL,HeapR)) :-
    X < X1, greater(X1,HeapL), adjust(X,L,R,HeapR).

    greater(_,void).
    greater(X,tree(X1,_,_)) :- X >= X1.
    
%   Program 3.28    Adjusting a binary tree to satisfy the heap property

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	polynomial(Expression,X) :- 
		Expression is a polynomial in X.
*/
	polynomial(X,X).
	polynomial(Term,X) :- atomic(Term), Term \== X.
	polynomial(Term1+Term2,X) :-
		polynomial(Term1,X), polynomial(Term2,X).
	polynomial(Term1-Term2,X) :-
		polynomial(Term1,X), polynomial(Term2,X).
	polynomial(Term1*Term2,X) :-
		polynomial(Term1,X), polynomial(Term2,X).
	polynomial(Term1/Term2,X) :-
		polynomial(Term1,X), atomic(Term2), Term2 \== X.
	polynomial(Term ^ N,X) :-
		natural_number(N), polynomial(Term,X).

%	Program 3.29: Recognizing polynomials

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	derivative(Expression,X,DifferentiatedExpression) :-
		DifferentiatedExpression is the derivative of
			Expression with respect to X.
*/

	derivative(X,X,s(0)).
	derivative(Term,X,0) :- atomic(Term), Term \== X.
	derivative(X ^ s(N),X,s(N) * X ^ N).
	derivative(sin(X),X,cos(X)).
	derivative(cos(X),X,-sin(X)).
	derivative(e ^ X,X,e ^ X).
	derivative(log(X),X,1/X).

	derivative(F+G,X,DF+DG) :-
		derivative(F,X,DF), derivative(G,X,DG).
	derivative(F-G,X,DF-DG) :-
		derivative(F,X,DF), derivative(G,X,DG).
	derivative(F*G,X,F*DG + DF*G) :-
		derivative(F,X,DF), derivative(G,X,DG).
	derivative(1/F,X,-DF/(F*F)) :-
		derivative(F,X,DF).
	derivative(F/G,X,(G*DF-F*DG)/(G*G)) :-
		derivative(F,X,DF), derivative(G,X,DG).

%	Program 3.30: Derivative rules

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	hanoi(N,A,B,C,Moves) :- 
		Moves is a sequence of moves for solving the Towers of
		Hanoi puzzle with N disks and three pegs, A, B and C.
*/

	hanoi(s(0),A,B,_,[(0:A->B)]).         % The disks are indexed by
	hanoi(s(N),A,B,C,Moves) :-           % with  0, s(0), s(s(0)), ...
		hanoi(N,A,C,B,Ms1),
		hanoi(N,C,B,A,Ms2),
		append(Ms1,[(N:A->B)|Ms2],Moves).

%	Program 3.31: Towers of Hanoi

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	satisfiable(Formula) :- 
		There is a true instance of the Boolean formula Formula.     
*/
	:- op(920, yfx, [&]).
	:- op(940, yfx, [v]).
	:- op(900,  fy, [~]).


	satisfiable(true).
	satisfiable(X & Y) :- satisfiable(X), satisfiable(Y).
	satisfiable(X v _) :- satisfiable(X).
	satisfiable(_ v Y) :- satisfiable(Y).
	satisfiable((~ X)) :-  invalid(X).
/*
	invalid(Formula) :-	
		There is a false instance of the Boolean formula Formula.	  
*/
	invalid(false).
	invalid(X v Y) :- invalid(X), invalid(Y).
	invalid(X & _) :- invalid(X).
	invalid(_ & Y) :- invalid(Y).
	invalid((~ X)) :- satisfiable(X).

%	Program 3.32: Satisfiability of Boolean formulae

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
