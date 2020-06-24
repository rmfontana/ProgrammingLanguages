% Drone scouts through cave made of rectangles [[TopLeft, TopRight, Width, Height], ...]
% Drone cannot hit any of these rectangles (cave walls)

cave([[ _, _, _, _]]).

wall_checker([X, Y, W, H], [A, B]) :-
  A > X, 
  A =< W + X, 
  B > Y,
  B =< H + Y.
  
check_bounds([CaveWidth, CaveHeight], [A, B]) :-
  A >= 0, B >= 0,
  A =< CaveWidth, 
  B =< CaveHeight.
  
move([X, Y], [A, Y], [A, Y]) :- 
  A is X + 1.

move([X, Y], [X, B], [X, B]) :- 
  B is Y + 1.

move([X, Y], [X, B], [X, B]) :- 
  B is Y - 1.
  
safe([],[CaveWidth, CaveHeight], [A, B]) :- 
  check_bounds([CaveWidth, CaveHeight], [A, B]).
  
safe([H|T], [CaveWidth, CaveHeight], [A, B]) :-
  not(wall_checker(H, [A, B])), check_bounds([CaveWidth, CaveHeight], [A, B]), safe(T, [CaveWidth, CaveHeight], [A, B]).

not_member([], []) :- !.

not_member([_, _], []) :- !.

not_member(H, [H2|T2]) :- 
  H \= H2, not_member(H, T2).
  
fly(Cave, [CaveWidth, CaveHeight], StartY, Path) :- 
  fly_2(Cave, [CaveWidth, CaveHeight], [0, StartY], [], Path).
  
fly_2(_,[CaveWidth, _],[StartX, _], _, []) :- 
  StartX = CaveWidth, !.
  
fly_2(Cave,[CaveWidth, CaveHeight], [StartX, StartY], Visited, [First|Other]) :-
  move([StartX, StartY], First, Next), safe(Cave, [CaveWidth, CaveHeight], Next), not_member(Next, Visited), New_Visited = [Next|Visited], fly_2(Cave, [CaveWidth, CaveHeight], Next,       New_Visited, Other).
