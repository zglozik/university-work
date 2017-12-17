%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                    Chapter 13. Program Development                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- use_module(library(lists),[member/2,non_member/2]).

% Every program for ground lists.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   union(Xs,Ys,Us) :- Us is the union of the elements in Xs and Ys. 
*/

union([X|Xs],Ys,Us) :- member(X,Ys), union(Xs,Ys,Us).
union([X|Xs],Ys,[X|Us]) :- non_member(X,Ys), union(Xs,Ys,Us).
union([],Ys,Ys).

%  Program 13.1    Finding the union of two lists

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
intersection(Xs,Ys,Is) :- Is is the intersection of the elements in Xs and Ys. 
*/

intersection([X|Xs],Ys,[X|Is]) :- member(X,Ys), intersection(Xs,Ys,Is).
intersection([X|Xs],Ys,Is) :- non_member(X,Ys), intersection(Xs,Ys,Is).
intersection([],_,[]).

%  Program 13.2    Finding the intersection of two lists

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   union_intersect(Xs,Ys,Us,Is) :- 
	Us and Is are the union and intersection, respectively, of the
		elements in Xs and Ys.  
*/

union_intersect([X|Xs],Ys,Us,[X|Is]) :- 
	member(X,Ys), union_intersect(Xs,Ys,Us,Is).
union_intersect([X|Xs],Ys,[X|Us],Is) :- 
	non_member(X,Ys), union_intersect(Xs,Ys,Us,Is).
union_intersect([],Ys,Ys,[]).

%  Program 13.3    Finding the union and intersection of two lists

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Homework: Where to insert green cuts? How do they turn into red?
%           Consequences? 
%           Are there some cases when they are not to be turned into red?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
