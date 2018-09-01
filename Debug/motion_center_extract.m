xc = dlmread('motion_centerx.txt');
yc = dlmread('motion_centery.txt');

plot(xc,'b*-');
hold on;
%yc=load('y');
plot(yc,'r*-');
xlabel('No. of Frames');
ylabel('X & Y motion centers');
legend('xc','yc');