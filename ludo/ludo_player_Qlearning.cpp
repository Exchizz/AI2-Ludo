#include "ludo_player_Qlearning.h"


#define GAME_POSITION_HOME -1
#define GAME_POSITION_GOAL 99

#define ST_HOME         0
#define ST_GOAL         1
#define ST_GLOBE        2
#define ST_STAR         3
#define ST_WINNER_ROAD  4
#define ST_FREESPACE    5



ludo_player_Qlearning::ludo_player_Qlearning(game *obj):
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0),
    rd(),
    gen(rd())
{
    game_obj = obj;
}

int ludo_player_Qlearning::get_current_state(int token_state){
  int state = ST_FREESPACE;

  #warning Needs occupied state(as globe)

  // If token is on globe
  if((token_state - 8) % 13 == 0 ){
    state = ST_GLOBE;
  }

  if(token_state == GAME_POSITION_GOAL){
    state = ST_GOAL;
  }

  if (token_state == GAME_POSITION_HOME){
    state = ST_HOME;
  }


  if(game_obj->isStar(token_state)){
    state = ST_STAR;
  }

  if(token_state > 50){ // If higher than 50, then in winner road
    state = ST_WINNER_ROAD;
  }
  return state;
}

std::string state_int_to_string(int token_state){
  std::string state_str;
  switch(token_state){
    case ST_HOME:
      state_str = "ST_HOME";
    break;

    case ST_GOAL:
      state_str = "ST_GOAL";
    break;

    case ST_GLOBE:
      state_str = "ST_GLOBE";
    break;

    case ST_STAR:
      state_str = "ST_STAR";
    break;

    case ST_WINNER_ROAD:
      state_str = "ST_WINNER_ROAD";
    break;

    default:
      state_str = "ST_FREESPACE";
    break;
  }

  return state_str;
}

int ludo_player_Qlearning::make_decision(){
  // For each token
  for(int i = 0; i < 4; i++){
    // Get current state
    int current_state = get_current_state(pos_start_of_turn[i]);
    std::cout << "Player #" << i << " state: " << state_int_to_string(current_state) << "\t";
    // Calculate possible actions based on state and dice_roll
    //auto possible_actions = get_possible_actions(current_state, dice_roll);
  }
  std::cout << std::endl;

  //
  // if(e_greedy() == EXPLOIT){
  //   // From actions possible, select the best one
  //   auto action = QTableLookup(possible_actions);
  // } else { // EXPLORE
  //   auto action = pick_random_action();
  // }
  //
  // return action.player;
  return 1;
}

void ludo_player_Qlearning::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player_Qlearning::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
