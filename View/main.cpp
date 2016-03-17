#include "dialog.h"
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "positions_and_dice.h"

int simple_player(std::vector<int> relative_pos, int dice){
    if(dice == 6){
        for(int i = 0; i < 4; ++i){
            if(relative_pos[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(relative_pos[i]>=0 && relative_pos[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(relative_pos[i]>=0 && relative_pos[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //in case they are all locked in.
            if(relative_pos[i]<0){
                std::cout << "all is locked in" << std::endl;
                return i;
            }
        }
    }

    return -1; //no valid moves
}


Q_DECLARE_METATYPE( positions_and_dice )

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    qRegisterMetaType<positions_and_dice>();

    ludo_player p1, p2, p3, p4;
    game g;
    g.setGameDelay(4000);
    QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));

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

//    int iter = 200;
//    while(iter--){
//        w.next_turn(relative_pos, dice);
//        w.movePiece(simple_player(relative_pos, dice));
//        w.update_graphics();
//    }
    std::cout << std::endl;
    return a.exec();
}
