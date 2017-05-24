#ifndef LUDO_PLAYER_QLEARNING_H
#define LUDO_PLAYER_QLEARNING_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include "game.h"
#include <tuple>

// State, Action, Token
typedef std::tuple<int,int,int,int,float> state_action;


class ludo_player_Qlearning : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    std::random_device rd;
    std::mt19937 gen;
    game* game_obj;
    std::vector< std::vector<float> > QTable;
    bool InSafety(int position);

    state_action last_move;

    int make_decision();
public:
    ludo_player_Qlearning(game *game_obj);
    std::vector<state_action> prev_token_positions;
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
    int get_current_state(int token_state);
    state_action maxQ(std::vector<state_action> &possible_moves);
    state_action randomQ(std::vector<state_action> &possible_moves);
    float CalculateImmediateReward(state_action & best_move);


    std::vector<std::pair<int,float>> get_possible_actions(int token_pose, int);

};

#endif // LUDO_PLAYER_QLEARNING_H
