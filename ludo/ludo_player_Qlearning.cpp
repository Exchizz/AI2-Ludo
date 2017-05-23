#include "ludo_player_Qlearning.h"

#define NUMBER_OF_STATES    9
#define NUMBER_OF_ACTIONS   11

#define GAME_POSITION_HOME -1
#define GAME_POSITION_GOAL 56

#define ST_HOME         0
#define ST_GOAL         1
#define ST_GLOBE        2
#define ST_STAR         3
#define ST_WINNER_ROAD  4
#define ST_FREESPACE    5

#define STATE(var)  std::get<0>(var)
#define ACTION(var)  std::get<1>(var)
#define TOKEN(var)  std::get<2>(var)


#define MOVE_OUT_FROM_HOME                          0
#define MOVE_IN_GOAL                                1
#define MOVE_TO_GLOBE                               2
#define MOVE_TO_STAR                                3
#define MOVE_TO_GOAL_VIA_STAR                       4
#define GET_INTO_SAFETY_WITH_AN_SAME_COLORED_TOKEN  5
#define GET_INTO_THE_WINNER_ROAD                    6
#define SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE       7
#define KILL_OPPONENT                               8
#define JUST_MOVE                                   9
#define NO_MOVE_POSSIBLE                            10


bool is_star(int pos){
  std::vector<int> v{ 5, 11, 18, 24, 31, 37, 44 }; // Globes, start pose(when 6) not included
  return (std::find(v.begin(), v.end(), pos) != v.end());
}

bool is_globe(int pos){
  std::vector<int> v{ 8, 21, 34, 47 }; // Globes, start pose(when 6) not included
  return (std::find(v.begin(), v.end(), pos) != v.end());
}

void QTabledumper(std::vector< std::vector<int> > &Qtable){
  std::cout << "QTable --------------------------------" << std::endl;
  for(unsigned int state = 0; state < Qtable.size(); state++){
    for(unsigned int action = 0; action < Qtable[0].size(); action++){
      std::cout << "Val: " << Qtable[state][action] << "\t";
    }
    std::cout << std::endl;
  }
  std::cout << "QTable end ----------------------------" << std::endl;

}
ludo_player_Qlearning::ludo_player_Qlearning(game *obj):
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0),
    rd(),
    gen(rd()),
    QTable(NUMBER_OF_ACTIONS, std::vector<int>(NUMBER_OF_STATES))
  {
    game_obj = obj;

    // State 2, action 3 = 200;
    // QTable[2][3] = 200;
    QTabledumper(QTable);
  }
int ludo_player_Qlearning::get_current_state(int token_state){
  int state = ST_FREESPACE;

  // If token is out and is on globe
  // Might divide this up later..
  if( token_state > 0 && game_obj->isGlobe(token_state) ){
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
std::vector<int> ludo_player_Qlearning::get_possible_actions(int token_pose, int _dice_roll){
  int new_pose = token_pose + _dice_roll;

  std::cout << "\t" << new_pose << " token_pose: " << token_pose <<  std::endl;
  std::vector<int> possible_actions;

  if(new_pose  == (-1 + 6)){ // in home = -1, 6 to move out
    possible_actions.push_back(MOVE_OUT_FROM_HOME);
    return possible_actions;
  }

  if(new_pose == 56){ // 56 is actual goal
    getchar();
    possible_actions.push_back(MOVE_IN_GOAL);
    return possible_actions;
  }

  if(is_star(new_pose)){
    possible_actions.push_back(MOVE_TO_STAR);
  }

  if(new_pose > 50){ // If in goal-road
    possible_actions.push_back(GET_INTO_THE_WINNER_ROAD);
  }

  if(is_globe(new_pose) ){ // If on goal
    possible_actions.push_back(MOVE_TO_GLOBE);
  }

  // If no other actions are possible.
  if(possible_actions.size() == 0 and token_pose != -1){
    //missing action with safe zone.
    possible_actions.push_back(JUST_MOVE);
  }
  return possible_actions;
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
std::string action_int_to_string(int token_action){
  std::string action_str;
  switch(token_action){
    case MOVE_OUT_FROM_HOME:
      action_str = "MOVE_OUT_FROM_HOME";
    break;

    case MOVE_IN_GOAL:
      action_str = "MOVE_IN_GOAL";
    break;

    case MOVE_TO_GLOBE:
      action_str = "MOVE_TO_GLOBE";
    break;

    case MOVE_TO_STAR:
      action_str = "MOVE_TO_STAR";
    break;

    case MOVE_TO_GOAL_VIA_STAR:
      action_str = "MOVE_TO_GOAL_VIA_STAR";
    break;

    case GET_INTO_SAFETY_WITH_AN_SAME_COLORED_TOKEN:
      action_str = "GET_INTO_SAFETY_WITH_AN_SAME_COLORED_TOKEN";
    break;

    case GET_INTO_THE_WINNER_ROAD:
      action_str = "GET_INTO_THE_WINNER_ROAD";
    break;

    case SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE:
      action_str = "SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE";
    break;

    case KILL_OPPONENT:
      action_str = "KILL_OPPONENT";
    break;

    case JUST_MOVE:
      action_str = "JUST_MOVE";
    break;

    case NO_MOVE_POSSIBLE:
      action_str = "NO_MOVE_POSSIBLE";
    break;

    default:
      std::cout << "UNDEFINED ACTION AT LINE" << __LINE__ << " function: " << __FUNCTION__ << std::endl;
  }

  return action_str;
}


state_action ludo_player_Qlearning::maxQ(std::vector<state_action> &possible_moves){
  int max = 0;
  state_action max_state_action;
  for(auto action : possible_moves){
    int Tmpstate = STATE(action);
    int Tmpaction = ACTION(action);

    if(QTable[Tmpstate][Tmpaction] > max){
      max = QTable[Tmpstate][Tmpaction];
      max_state_action = action;
    }
  }
  return max_state_action;
}


int ludo_player_Qlearning::make_decision(){

  std::vector<state_action>possiblePlayerMoves;
  // For each token
  for(int token_i = 0; token_i < 4; token_i++){
    // Get current state
    int current_state = get_current_state(pos_start_of_turn[token_i]);
    std::cout << "Token #" << token_i << " state: " << state_int_to_string(current_state) << std::endl;
    // Calculate possible actions based on state and dice_roll
    auto possible_actions = get_possible_actions(pos_start_of_turn[token_i], dice_roll);

    std::cout << "Number of possible actions: " << possible_actions.size() << std::endl;

    for(auto possible_action : possible_actions){
      possiblePlayerMoves.push_back(std::make_tuple(current_state, possible_action, token_i));
    }
  }

  for(auto possible_action : possiblePlayerMoves){
    std::cout << "Token: " << TOKEN(possible_action) << " can do action " << action_int_to_string(ACTION(possible_action)) << std::endl;
    // possiblePlayerMoves.push_back(std::make_tuple(token_i, possible_action, token_i));
  }

  // Get best action given all possible actions
  auto best_move = maxQ(possiblePlayerMoves);
  std::cout << "Token " << TOKEN(best_move) << " should be moved" << std::endl;
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
  return TOKEN(best_move);
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
