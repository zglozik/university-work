%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                    Chapter 12. Extra-Logical Predicates                     %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


/*
     writeln(Xs) :- 
	The list of terms Xs is written on the output stream by side-effect.
*/
     writeln([X|Xs]) :- write(X), writeln(Xs).
     writeln([]) :- nl.

%  Program 12.1   Writing a list of terms

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
   read_word_list(Words) :- 
	Words is a list of words read from the input stream via side effects.
*/
     read_word_list(Words) :- 
		get0(FirstChar), read_words(FirstChar,Words).

     read_words(Char,[Word|Words]) :-
		word_char(Char), !,
		read_word(Char,Word,NextChar), 
		read_words(NextChar,Words).
     read_words(Char,Words) :-
		fill_char(Char), !,
		get0(NextChar),
		read_words(NextChar,Words).
     read_words(Char,[Mark|Words]) :-
		inner_punctuation_char(Char), !,
                name(Mark,[Char]),
		get0(NextChar),
		read_words(NextChar,Words).
     read_words(Char,[End]) :-
		end_of_words_char(Char), !,
                name(End,[Char]), skip_line.
     read_words(Char,[???,End,???]) :-           % Default rule
                name(End,[Char]), skip_line.

     read_word(Char,Word,NextChar) :- 
		word_chars(Char,Chars,NextChar),
		name(Word,Chars).

     word_chars(Char,[Char|Chars],FinalChar) :- 
	  	word_char(Char), !,
		get0(NextChar),
		word_chars(NextChar,Chars,FinalChar).
     word_chars(Char,[],Char) :- 
		\+ word_char(Char).

     word_char(C) :- 0'a =< C, C =< 0'z.        % Lower-case letter
     word_char(C) :- 0'A =< C, C =< 0'Z.        % Upper-case letter
     word_char(0'_).                            % Hyphen
     word_char(0'-).                            % 
     word_char(C) :- 0'0 =< C, C =< 0'9.        % numeric digits as well

     word_char(0'á).                            % 
     word_char(0'Á).                            % 
     word_char(0'é).                            % 
     word_char(0'É).                            % 
     word_char(0'í).                            % 
     word_char(0'Í).                            % 
     word_char(0'ó).                            % Letters with accents
     word_char(0'Ó).                            % 
     word_char(0'ö).                            % 
     word_char(0'Ö).                            % 
     word_char(0'õ).                            % 
     word_char(0'Õ).                            % 
     word_char(0'ü).                            % 
     word_char(0'Ü).                            % 
     word_char(0'û).                            % 
     word_char(0'Û).                            % 

     inner_punctuation_char(0',).
     inner_punctuation_char(0';).
     inner_punctuation_char(0':).

     fill_char(32).						 % Blank
     fill_char(10).						 % nl
     fill_char(13).
     fill_char(9).						 % Tab

     end_of_words_char(0'.).	   		% Period
     end_of_words_char(0'?).
     end_of_words_char(0'!).

%  Program 12.2   Reading in a list of words

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
	hanoi(N,A,B,C,Moves) :-
		Moves is the sequence of moves required to move N discs
		from peg A to peg B using peg C as an intermediary
		according to the rules of the Towers of Hanoi puzzle
*/

	:- op(100, xfx, [to]).
        :- dynamic hanoi/5.

	hanoi(1,A,B,_,[A to B]).
	hanoi(N,A,B,C,Moves) :-
		N > 1,
		N1 is N -1,
		lemma(hanoi(N1,A,C,B,Ms1)),
		hanoi(N1,C,B,A,Ms2),
		append(Ms1,[A to B|Ms2],Moves).

	lemma(P):- P, asserta((P :- !)).

	/* Testing */

	test_hanoi(N,Pegs,Moves) :-
		hanoi(N,A,B,C,Moves), Pegs = [A,B,C].

%	Program 12.3: Towers of Hanoi using a memo-function

% hanoi(N,Moves) :- Moves move N discs from peg1 to peg2 using peg3.
%                   Remembers the results during later runs.

hanoi(N,Moves) :- lemma_of_hanoi(N,A,B,C,Moves), A=1,B=2,C=3.

hanoi_(1,A,B,_,[(A->B)]) :- !.
hanoi_(N,A,B,C,Moves) :-
	N > 1,
	N1 is N -1,
	lemma_of_hanoi(N1,A,C,B,Ms1),
	hanoi_lemma(N1,C,B,A,Ms2),  % Just hanoi/2 is to be called from prompt.
	append(Ms1,[(A->B)|Ms2],Moves).

:- dynamic hanoi_lemma/5.

lemma_of_hanoi(N,A,B,C,Moves) :- hanoi_lemma(N,A,B,C,Moves), !.
lemma_of_hanoi(N,A,B,C,Moves) :- % \+hanoi_lemma(N,A,B,C,Moves), !.
     hanoi_(N,A,B,C,Moves), assert(hanoi_lemma(N,A,B,C,Moves)).

:- use_module(library(lists),[append/3]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
    echo :- An interactive loop.
*/
    echo :- read(X), echo(X).

    echo(X) :- last_input(X), !.
    echo(X) :- write(X), nl, read(Y), !, echo(Y).

%  Program 12.4    Basic interactive loop

last_input('bye').

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
    edit :- A line editor.  
	
Files are represented in the form file(Before,After), where After is a
list of lines after the current cursor position and Before is a list
of lines before the cursor in reverse order.

*   Note: Program has been augmented to accomodate boundary 
*         condition.
*/

     edit :- 
         write('insert(Line). delete. up. up(N). '), 
         write('down. down(N). print. print(*). exit.'),
         nl,
         edit(file([],[])).

     edit(File) :-
		write_prompt, read(Command), edit(File,Command).

     edit(_,exit) :- !.
     edit(File,Command) :-
        apply(Command,File,File1), !, edit(File1).
     edit(File,Command) :-
		writeln([Command,' is not applicable.']), !, edit(File).

     apply(up,file([X|Xs],Ys),file(Xs,[X|Ys])).
     apply(up,file([],Ys),file([],Ys)) :-
		write('<<top>>'), nl.
     apply(up(N),file(Xs,Ys),file(Xs1,Ys1)) :-
		N > 0, up(N,Xs,Ys,Xs1,Ys1).
     apply(down,file(Xs,[Y|Ys]),file([Y|Xs],Ys)).
     apply(down,file(Xs,[]),file(Xs,[])) :-
		write('<<bottom>>'), nl.
     apply(down(N),file(Xs,Ys),file(Xs1,Ys1)) :-
		N > 0, down(N,Xs,Ys,Xs1,Ys1).
     apply(insert(Line),file(Xs,Ys),file([Line|Xs],Ys)).
     apply(delete,file([_|Xs],Ys),file(Xs,Ys)).
     apply(delete,file([],Ys),file([],Ys)) :-
		write('<<top>>'), nl.
     apply(print,file([X|Xs],Ys),file([X|Xs],Ys)) :-
		write(X), nl.
     apply(print,file([],Ys),file([],Ys)) :-
		write('<<top>>'), nl.
     apply(print(*),file(Xs,Ys),file(Xs,Ys)) :- 
        reverse(Xs,Xs1), 
	nl, write('<<top>>'), nl,
        write_file(Xs1), write_file(Ys),
	write('<<bottom>>'), nl, nl.

     :- use_module(library(lists),[reverse/2]).
 
     up(0,Xs,Ys,Xs,Ys).
     up(N,[],Ys,[],Ys) :-
		N > 0, write('<<top>>'), nl.
     up(N,[X|Xs],Ys,Xs1,Ys1) :- 
		N > 0, N1 is N-1, up(N1,Xs,[X|Ys],Xs1,Ys1).

     down(0,Xs,Ys,Xs,Ys).
     down(N,Xs,[],Xs,[]) :-
		N > 0, write('<<bottom>>'), nl.
     down(N,Xs,[Y|Ys],Xs1,Ys1) :- 
		N > 0, N1 is N-1, down(N1,[Y|Xs],Ys,Xs1,Ys1).

     write_file([X|Xs]) :- 
		write(X), nl, write_file(Xs).
     write_file([]).

     write_prompt :- write('>> '), ttyflush.

%  Program 12.5: A line editor

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

     shell :-  shell_prompt, read(Goal), shell(Goal).

     shell(exit) :- !.
     shell(Goal) :-
        ground(Goal), !, shell_solve_ground(Goal), shell. 
     shell(Goal) :- 
        shell_solve(Goal), shell.

     shell_solve(Goal) :-
	Goal, write(user,Goal), nl(user), fail.
     shell_solve(_) :- 
        write(user,'No (more) solutions'), nl(user).
     
     shell_solve_ground(Goal) :- 
		Goal, !, write(user,'Yes'), nl(user).
     shell_solve_ground(_) :- 
		write(user,'No'), nl(user).
          
     shell_prompt :-  write(user, 'Next command? '), ttyflush.

%	Program 12.6    An interactive shell

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


     :- dynamic logging_file/1.

     logg :- 
         nl(user), write(user,'Goal. logg. nolog. exit.'), nl(user), nl(user),
	 open_logging_file_if_needed,
	 shell_flag(logg).

     shell_flag(Flag) :- 
		shell_prompt, shell_read(Goal,Flag), shell(Goal,Flag).

     shell(exit,_) :- 
	!, close_logging_file.
     shell(nolog,_) :-
	!, shell_flag(nolog).
     shell(logg,_) :-
	!, shell_flag(logg).
     shell(Goal,Flag) :-
	ground(Goal), !, shell_solve_ground(Goal,Flag), shell_flag(Flag).
     shell(Goal,Flag) :-
        shell_solve(Goal,Flag), shell_flag(Flag).
     
     shell_solve(Goal,Flag) :- 
        Goal, flag_write(Goal,Flag), yes_no('    More solution?',fail,true), 
	!, flag_write('Yes',Flag), nl(user), nl(user).
     shell_solve(_,Flag) :- 
        flag_write('No (more) solutions',Flag), nl(user), nl(user).
     
:- use_module(library(env3),[yes_no/3]).

     shell_solve_ground(Goal,Flag) :- 
		Goal, !, flag_write('Yes',Flag), nl(user).
     shell_solve_ground(_,Flag) :- 
		flag_write('No',Flag), nl(user).
          
%     shell_prompt :- write(user, 'Next command? '), ttyflush.

     shell_read(X,logg) :-
		read(user,X), 
		logging_file(S),
		file_write(['Next command? ',X],S).
     shell_read(X,nolog) :- read(user,X).

     flag_write(X,nolog) :- write(user,X).
     flag_write(X,logg) :- write(user,X),
	                   logging_file(Stream),
	                   file_write(X,Stream).

     file_write(X,File) :- write(File,X), nl(File).

     close_logging_file :- 
	 retract(logging_file(S)), 
	 write(S,***********************************************************),
	 nl(S),
         close(S). 

open_logging_file_if_needed :-
	logging_file(S) -> 
	  write(S,***********************************************************),
	  nl(S), nl(S)
        ; open('prolog.log',append,S), nl(S), 
	  assert(logging_file(S)).

%	Program 12.7 Logging a session

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	echo_ :- repeat_, read(X), echo_(X), !.

	echo_(X) :- last_input(X), !.
	echo_(X) :- write(X), nl, fail.

        repeat_.                        % repeat/0 is predefined.
	repeat_ :- repeat_.

%	Program 12.8    Basic interactive repeat loop

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
    consult_(File) :- 
	The clauses of the program in the file File are read and asserted.
*/

	consult_(File) :- open(File,read,DD), consult_loop(DD), close(DD).

	consult_loop(DD) :- repeat, read(DD,Clause), process(Clause), !.

	process(end_of_file) :- !.
	process(Clause) :- assertz(Clause), fail.

%  Program 12.9:    Consulting a file

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
