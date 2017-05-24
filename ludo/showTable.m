close all; clear; clc

data = importfile('build/Qtable8500.txt');
qtable = zeros(6,10);
for i = 0:5
    qtable(1+i,:) = table2array(data(1,i*10+1:i*10+10));
end
%%
fprintf("MOVE_OUT_FROM_HOME\t ok");
qtable
%%
clc
LastName = {'ST_HOME';'ST_GOAL';'ST_GLOBE';'ST_STAR';'ST_WINNER_ROAD';'ST_FREESPACE'};

MOVE_OUT_FROM_HOME = qtable(:,1);
MOVE_IN_GOAL = qtable(:,2);
MOVE_TO_GLOBE = qtable(:,3);
MOVE_TO_STAR = qtable(:,4);
MOVE_TO_GOAL_VIA_STAR = qtable(:,5);
GET_INTO_SAFETY_WITH_A_SAME_COLORED_TOKEN = qtable(:,6);
GET_INTO_THE_WINNER_ROAD = qtable(:,7);
SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE = qtable(:,8);
KILL_OPPONENT = qtable(:,9);
JUST_MOVE = qtable(:,10);

T = table(MOVE_OUT_FROM_HOME,MOVE_IN_GOAL,MOVE_TO_GLOBE,MOVE_TO_STAR,MOVE_TO_GOAL_VIA_STAR,GET_INTO_SAFETY_WITH_A_SAME_COLORED_TOKEN,GET_INTO_THE_WINNER_ROAD,SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE,KILL_OPPONENT,JUST_MOVE,'RowNames',LastName)