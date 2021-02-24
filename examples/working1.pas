program const_circle;
const
PI = 3.141592654;
(* This is pi, the value which is c/d for a circle *)
var
r, d, c : integer;   {variable declaration: radius, dia, circumference}
color: string;

begin
   r:=2;
   d:=4;
   if color = red then
      if color = blue then
            writeln('You have chosen a blue car')
         else
            writeln('Please choose a color for your car');
   
   d := 2 * r;
   c :=  PI * d;
   writeln('The circumference of the circle is ',c);
end.