program const_circle;
const
PI = 3.141592654;
(* This is pi, the value which is c/d for a circle *)
var
r, d : integer;   {variable declaration: radius, dia, circumference}
c : real;
color: integer;

begin
   r := 10;
   d := 4;
   color := 1;
   c := 1;
   if color = 1 then
      if color = 2 then
            c := c + 1
         else
            c := c + 2
   else
      c := c + 3;        
   d := 2 * r;
   c :=  PI * d;
   writeln('The circumference of the circle is ',c);
end.