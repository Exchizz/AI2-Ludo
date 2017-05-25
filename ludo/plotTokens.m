close all; clear; clc;

data = csvread('build/outputs/test-2000ittr-20ittrlearn/tokensInGoal.csv',1);

player1 = data(:,1);
player2 = data(:,2);
player3 = data(:,3);
player4 = data(:,4);

tokens = zeros(4,4);
for i = 1:4 % Player
    for k = 1:4 % Number of tokens
        tokens(k,i) = (sum(data(:,i) == k)/2000)*100;
    end
end

bar(tokens')
set(gca,'xticklabel',{'Q-learning player','Random player','Random player','Random player'},'fontsize', 15)
ylabel('Percent %')
legend({'1 token in goal','2 tokens in goal','3 tokens in goal','4 tokens in goal'},'fontsize',15);
title('20 Itterations learning');
(data)