#include "dialog.h"
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "positions_and_dice.h"

Q_DECLARE_METATYPE( positions_and_dice )

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    qRegisterMetaType<positions_and_dice>();

    //instanciate the players here
    ludo_player p1, p2, p3, p4;

    game g;
    g.setGameDelay(000); //if you want to see the game, set a delay

    QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
    QObject::connect(&g,SIGNAL(set_color(int)),&w,SLOT(get_color(int)));
    QObject::connect(&g,SIGNAL(set_dice_result(int)),&w,SLOT(get_dice_result(int)));
    QObject::connect(&g,SIGNAL(declare_winner(int)),&w,SLOT(get_winner(int)));


    //set up for each player
    QObject::connect(&g,SIGNAL(player1_start(positions_and_dice)),&p1, SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p1,SIGNAL(select_piece(int)),&g,SLOT(movePiece(int)));
    QObject::connect(&g,SIGNAL(player1_end(std::vector<int>)),&p1,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p1,SIGNAL(turn_complete(bool)),&g,SLOT(turnComplete(bool)));

    QObject::connect(&g,SIGNAL(player2_start(positions_and_dice)),&p2, SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p2,SIGNAL(select_piece(int)),&g,SLOT(movePiece(int)));
    QObject::connect(&g,SIGNAL(player2_end(std::vector<int>)),&p2,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p2,SIGNAL(turn_complete(bool)),&g,SLOT(turnComplete(bool)));

    QObject::connect(&g,SIGNAL(player3_start(positions_and_dice)),&p3, SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p3,SIGNAL(select_piece(int)),&g,SLOT(movePiece(int)));
    QObject::connect(&g,SIGNAL(player3_end(std::vector<int>)),&p3,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p3,SIGNAL(turn_complete(bool)),&g,SLOT(turnComplete(bool)));

    QObject::connect(&g,SIGNAL(player4_start(positions_and_dice)),&p4, SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p4,SIGNAL(select_piece(int)),&g,SLOT(movePiece(int)));
    QObject::connect(&g,SIGNAL(player4_end(std::vector<int>)),&p4,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p4,SIGNAL(turn_complete(bool)),&g,SLOT(turnComplete(bool)));

    g.start();

    std::cout << std::endl;
    return a.exec();
}
