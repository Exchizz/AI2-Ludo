#include "ludo_player.h"
#include <random>

ludo_player::ludo_player(){
}

int ludo_player::make_decision(){
  std::cout << "Dice roll: " << dice_roll << std::endl;

  for(int i = 0; i < 4; i++){
    std::cout << pos_start_of_turn[i] << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
  if(dice_roll == 6){
    return 0;
  }
    // if(dice_roll == 6){
    //     // All tokens are in play
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]<0){
    //             return i;
    //         }
    //     }
    //     // Check for token which is in play but not on goal(99)
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
    //             return i;
    //         }
    //     }
    // } else {
    //     // Find token which is in play, but not on goal.
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
    //             return i;
    //         }
    //     }
    //     for(int i = 0; i < 4; ++i){ //maybe they are all locked in
    //         if(pos_start_of_turn[i]<0){
    //             return i;
    //         }
    //     }
    // }    if(dice_roll == 6){
	  //    std::cout << "Player out" << std::endl;
    //     for(int i = 0; i < 4; ++i){
    //       std::cout<< "pos_start_of_rurn: " << pos_start_of_turn[i]  << std::endl;
    //         if(pos_start_of_turn[i]<0){
    //             return i;
    //         }
    //     }
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
    //             return i;
    //         }
    //     }
    // } else {
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
    //             return i;
    //         }
    //     }
    //     for(int i = 0; i < 4; ++i){ //maybe they are all locked in
    //         if(pos_start_of_turn[i]<0){
    //             return i;
    //         }
    //     }
    // }
    //  return -1;
}

void ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    std::cout << "Decision: " << decision << std::endl;
    emit select_piece(decision);
}

void ludo_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
