program fcalc;
uses crt;
const
PI=3.14;
var
num, i, sum: ineger;
condition: boolea;
begin
num:=5;
sum:=0;
for i:=1 to num do
begin
    sum := sum + i;
end;
condition := true;
if (condition = true) then 
    writeln("TRUE BLOCK");
end.
