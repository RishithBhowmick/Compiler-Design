program fcalc;
uses crt;
const
PI=314e-2;
var
num, i, sum: integer;
condition: boolean;
human_name: string;
begin
num:=5;
sum:=0;
human_name:='isima';
for i:=1 to num do
begin
    sum := sum + i;
end;
condition := true;
if (condition = true) then 
    writeln('"Hello"');
else
    writeln('Bye');
end.
