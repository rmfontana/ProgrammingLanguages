% Rule determines if a square [Width, Height] fits within a rectangle [UpperLeft, UpperRight, Width, Height]

rectangle([X, Y, W, H]).

containsSquare(rectangle([X, Y, W, H]), A, B) :-   

A > X,   

A =< W + X,   

B > Y,   

B =< H+ Y.
