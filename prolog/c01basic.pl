%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                     Simple Facts and Rules                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	father(terach,abraham).			male(terach).
	father(terach,nachor).			male(abraham).
	father(terach,haran).			male(nachor).
	father(abraham,isaac).			male(haran).
	father(haran,lot).			male(isaac).
	father(haran,milcah).			male(lot).
	father(haran,yiscah).

						female(sarah).
	mother(sarah,isaac).			female(milcah).
						female(yiscah).

%	Program 1.1: A biblical family database

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	son(X,Y) :- father(Y,X), male(X).
        son(X,Y) :- mother(Y,X), male(X).

	daughter(X,Y) :- father(Y,X), female(X).
        daughter(X,Y) :- mother(Y,X), female(X).

        father(X) :- father(X,_).
        mother(X) :- mother(X,_).

%	Program 1.2: Biblical family relationships

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
