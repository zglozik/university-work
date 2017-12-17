:- module( sorts, [ insert_sort/2, insertsort/2,
                    merge_sort/2, mergesort/2,
                    tree_sort/2, treesort/2] ).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

insert_sort([X|L],SXL) :- 
    insert_sort(L,SL), insert_(X,SL,SXL).
insert_sort([],[]).

insert_(X,[Y|L],[Y|XL]) :- Y @< X, !, insert_(X,L,XL).
insert_(X,L,[X|L]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

insertsort([X|L],SXL) :- 
    insertsort(L,SL), insert(SL,X,SXL).
insertsort([],[]).

insert([Y|L],X,XYL) :- compare(Rel,X,Y), insert(Rel,X,Y,L,XYL).
insert([],X,[X]).

insert(<,X,Y,L,[X,Y|L]).
insert(=,X,Y,L,[X,Y|L]).
insert(>,X,Y,L,[Y|LX]) :- insert(L,X,LX).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

merge_sort([X,Y|L],S) :- !,
    divide_(L,L1,L2), 
    merge_sort([X|L1],S1), merge_sort([Y|L2],S2), 
    merge_(S1,S2,S).
merge_sort([X],[X]).
merge_sort([],[]).

merge_([X|Xs],[Y|Ys],[X|Zs]) :- X @=< Y, !, merge_(Xs,[Y|Ys],Zs).
merge_([X|Xs],[Y|Ys],[Y|Zs]) :- /* X @> Y, */ !, merge_([X|Xs],Ys,Zs).
merge_(L,[],L) :- !.
merge_([],L,L).

divide_([X,Y|L],[X|L1],[Y|L2]) :- !, divide_(L,L1,L2).
divide_([X],[X],[]).
divide_([],[],[]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

mergesort([],[]).
mergesort([X|L],S) :- mergesort(L,X,S).

mergesort([],X,[X]).
mergesort([Y|L],X,S) :-
    divide(L,L1,L2),
    mergesort(L1,X,S1), mergesort(L2,Y,S2), merge(S1,S2,S).

merge([],L,L).
merge([X|Xs],L,ML) :- merge(L,X,Xs,ML).

merge([],Y,Ys,[Y|Ys]).
merge([X|Xs],Y,Ys,ML) :- compare(Rel,Y,X), merge(Rel,Y,Ys,X,Xs,ML).

merge(<,X,Xs,Y,Ys,[X|ML]) :- merge(Xs,Y,Ys,ML).
merge(>,X,Xs,Y,Ys,[Y|ML]) :- merge(Ys,X,Xs,ML).
merge(=,X,Xs,Y,Ys,[X,Y|ML]) :- merge(Xs,Ys,ML).

divide([X|L],[X|L1],L2) :- divide(L,L2,L1).
divide([],[],[]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tree_sort(L,SL) :- list_tree_(L,T), tree_list_(T,SL) .

list_tree_(L,T) :- list_into_tree_(L,empty_tree,T) .

list_into_tree_([X|L],T0,T) :- 
    ins_into_tree_(X,T0,T1), list_into_tree_(L,T1,T) .
list_into_tree_([],T,T) .

ins_into_tree_(X,t(L1,Y,R),t(L2,Y,R)) :- X@=<Y, !, ins_into_tree_(X,L1,L2).
ins_into_tree_(X,t(L,Y,R1),t(L,Y,R2)) :- /*X@>Y,*/ !, ins_into_tree_(X,R1,R2).
ins_into_tree_(X,empty_tree,t(empty_tree,X,empty_tree)) .

tree_list_(T,L) :- tree_into_list_(T,[],L).

tree_into_list_(empty_tree,List,List).
tree_into_list_(t(Left,Root,Right),List0,List) :- 
    tree_into_list_(Right,List0,List1), 
    tree_into_list_(Left,[Root|List1],List).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

treesort(L,SL) :- list_tree(L,T), tree_list(T,SL) .

list_tree(L,T) :- list_into_tree(L,empty_tree,T) .

list_into_tree([X|L],T0,T) :- 
    ins_into_tree(T0,X,T1), list_into_tree(L,T1,T) .
list_into_tree([],T,T) .

ins_into_tree(t(Root,Left,Right),X,TX) :-
    compare(Rel,X,Root), ins_into_tree(Rel,X,Root,Left,Right,TX).
ins_into_tree(empty_tree,X,t(X,empty_tree,empty_tree)).

ins_into_tree(<,X,Root,Left,Right,t(Root,LeftX,Right)) :- 
    ins_into_tree(Left,X,LeftX).
ins_into_tree(=,X,Root,Left,Right,t(Root,LeftX,Right)) :- 
    ins_into_tree(Left,X,LeftX).
ins_into_tree(>,X,Root,Left,Right,t(Root,Left,RightX)) :- 
    ins_into_tree(Right,X,RightX).

tree_list(T,L) :- tree_into_list(T,[],L).

tree_into_list(empty_tree,List,List).
tree_into_list(t(Root,Left,Right),List0,List) :- 
    tree_into_list(Right,List0,List1), 
    tree_into_list(Left,[Root|List1],List).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
