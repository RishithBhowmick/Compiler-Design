program const_circle;
const
PI = 3.141592654;
(* This is pi, the value which is c/d for a circle *)
var
r: integer;   {variable declaration: radius, dia, circumference}
c , d: real;
color: integer;
e: boolean;

begin
   {r := 's';}
   r := 0;
   d := 4;
   color := 1;
   c := 1.0;
   e := False;
   if color = 1 then
            c := c + 2
   else
      c := c + 3;        
   for a := 10  to 20 do
   begin
      writeln('value of r: ', r);
   end;
   d := r * 2;
   {r :=  PI * d;}
   {e := e + 0;}
   writeln('The circumference of the circle is ',c);
   c := 2.0;
   writeln('The circumference of the circle is ',c);
end.