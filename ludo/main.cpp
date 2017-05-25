#define RUN_FAST 1
#include "dialog.h"
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "ludo_player_random.h"
#include "ludo_player_Qlearning.h"
#include "positions_and_dice.h"

Q_DECLARE_METATYPE( positions_and_dice )

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    qRegisterMetaType<positions_and_dice>();


    game g;

    //instanciate the players here
    ludo_player_Qlearning p1(&g);
    ludo_player_random p2,p3, p4;

    g.setGameDelay(0);

    // * Add a GUI <-- remove the '/' to uncomment block
    Dialog w;
    #if !RUN_FAST
      g.setGameDelay(1000);

      QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
      QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
      QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
      QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
      QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
      w.show();
    #endif
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    //*/

    //set up for each player
    QObject::connect(&g, SIGNAL(player1_start(positions_and_dice)),&p1,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p1,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player1_end(std::vector<int>)),    &p1,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p1,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player2_start(positions_and_dice)),&p2,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p2,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player2_end(std::vector<int>)),    &p2,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p2,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player3_start(positions_and_dice)),&p3,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p3,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player3_end(std::vector<int>)),    &p3,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p3,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player4_start(positions_and_dice)),&p4,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p4,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player4_end(std::vector<int>)),    &p4,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p4,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));


    std::ofstream tokenStatsFile;
    tokenStatsFile.open ("outputs/tokensInGoal.csv");
    unsigned int counter = 1;
    tokenStatsFile << "Player1, Player2, Player3, Player4" << std::endl;
    for(int i = 0; i < 1000; ++i){
      std::vector<int>player_stats{0,0,0,0};

        g.start();
        a.exec();
        while (a.closingDown());

        // Write q-table to file //
        if(counter % 500 == 0){
          std::cout << "--------------------- Writeout Q-table-----------------------" << std::endl;
          p1.dumpQTableToFile(counter);
        }
        counter++;
        // Write q-table to file //



        // Write tokens to file //
        for(int k = 0; k < 16; k++){
          if(g.player_positions[k] == 99){
            player_stats[floor(k/4)]++;
          }
        }
        tokenStatsFile << player_stats[0] << "," <<  player_stats[1] << "," <<player_stats[2] << "," <<player_stats[3] << "\n";
        // END write tokens to file //

        g.reset();
    }
    tokenStatsFile.close();

    int player_0_wins = 0;
    for(auto winner: g.winner_history){
      if(winner == 0){
          player_0_wins++;
      }
    }

    std::cout << "Winrate: " << (player_0_wins/1000.0)*100 << "%" << std::endl;
    std::cout << "Save qtable" << std::endl;
    p1.dumpQTableToFile(99999);
    return 0;
}
