#ifndef LUDO_PLAYER_QLEARNING_H
#define LUDO_PLAYER_QLEARNING_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include "game.h"


class ludo_player_Qlearning : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    std::random_device rd;
    std::mt19937 gen;
    game* game_obj;

    int make_decision();
public:
    ludo_player_Qlearning(game *game_obj);
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
    int get_current_state(int token_state);
};

#endif // LUDO_PLAYER_QLEARNING_H
