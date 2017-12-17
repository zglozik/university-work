
fakt(0,1).
fakt(N,M) :- N > 0, K is N-1, fakt(K,L), M is N*L.
