program iftest;
var s:char;
r:real;
a,b,c:integer;
d:Boolean;
begin
r:=0.33;
a:=1;
if a = 1 div 3 + 2 then
b:=24
else
c:=a+b;
if a = 1 then
b:=24;
if a >= 1 then
begin
b:=24
end
else
begin
c:=a+b;
s:='A'
end;
if c<=b then
a:=a+1
else if b<c then
a:=a-1;
r:=1+2*(3-42)/5+6;
d:=true
end.