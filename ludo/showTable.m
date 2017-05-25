close all; clc;

%data = importfile('build/outputs/learn-20ittr/Qtable99999.txt');
data = Qtable99999;
qtable = zeros(7,7);

for i = 0:6
    qtable(1+i,:) = table2array(data(1,i*7+1:i*7+7));
end

%%
LastName = {'ST_HOME';'ST_GOAL';'ST_GLOBE';'ST_STAR';'ST_WINNER_ROAD';'ST_FREESPACE'; 'ST_SAFE'};

MOVE_OUT_FROM_HOME = qtable(:,1);
MOVE_IN_GOAL = qtable(:,2);
MOVE_TO_GLOBE = qtable(:,3);
MOVE_TO_STAR = qtable(:,4);
MOVE_TO_SAFETY = qtable(:,5);
MOVE_TO_WINNER_ROAD = qtable(:,6);
JUST_MOVE = qtable(:,7);

T = table(MOVE_OUT_FROM_HOME,MOVE_IN_GOAL,MOVE_TO_GLOBE,MOVE_TO_STAR,MOVE_TO_SAFETY,MOVE_TO_WINNER_ROAD,JUST_MOVE,'RowNames',LastName)
%%
figure
[xi,yi] = meshgrid(1:0.1:7,  1:0.1:7);
z = qtable;
x = 1:7;
y = 1:7;
zi = griddata(x,y,z,xi,yi);
surf(xi,yi,zi);
xlabel('State')
ylabel('Action')
zlabel('Q-table value')