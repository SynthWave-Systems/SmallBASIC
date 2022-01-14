#!/usr/bin/sbasic
' MATRICES
? cat(1);"MATRICES";cat(0)
?
A = [ -6; 2; -7]
B = [  5; 4; -6]
print A; " + "; B; " = "; A + B
print A; " - "; B; " = "; A - B
print A; " * "; .5; " = "; A * .5
print "A = ";A; ", -A = "; -A
'print "A = ";A; ", A^-1 = "; A

?
A = [ -2; 3]
B = [  7, 6, -3, 5]
print A; " * "; B; " = "; A * B

?
A = [  2, 4; -1, -2]
B = [ -2, 2, 4; 1, -1, -2]
print A; " * "; B; " = "; A * B

?
A = [ -3,  0; 2, -1]
B = [  4, -2; 3,  5]
print A; " * "; B; " = "; A * B

?
? "Solve this:"
? "  5x - 2y + 3z = -2"
? " -2x + 7y + 5z =  7"
? "  3x + 5y + 6z =  9"
? 
A = [ 5, -2, 3; -2, 7, 5; 3, 5, 6]
B = [-2; 7; 9]
'? A; " * X = "; B
'?
C=LinEqn(a,b)
print "[x; y; z] = "; C
?

rem -- handling for dot product and multiplication of two 1D arrays
a1=[1,2,4]
a2=[1,4,5]
if (a1 * a2 != [1,8,20]) then throw "err"
if (a1 % a2 != 29) then throw "err"

rem - Scalar * Vector doesnt work #131 (https://github.com/smallbasic/SmallBASIC/issues/131)
dim r(2)
v = [5, 10, 10]
s = 2
r = s * v
if (r[0] != 10) then throw "err1"
if (r[1] != 20) then throw "err2"
if (r[2] != 20) then throw "err3"
