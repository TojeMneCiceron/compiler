program whiletest;
var s:char;
r:real;
a,b,c:integer;
d:Boolean;
begin
r:=0.33;
a:=1;
b:=24;
c:=a+b;
while a > 0 do
b:=b+1;
while b <> 2 do
begin
c:=2;
a:=a-1;
end;
s:='A';
r:=1+2*(3-42)/5+6;
d:=true;
end.