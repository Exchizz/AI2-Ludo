#include "ludo_player_Qlearning.h"



#define NUMBER_OF_STATES    9
#define NUMBER_OF_ACTIONS   11

#define GAME_POSITION_HOME -1
#define GAME_POSITION_GOAL 99

#define ST_HOME         0
#define ST_GOAL         1
#define ST_GLOBE        2
#define ST_STAR         3
#define ST_WINNER_ROAD  4
#define ST_FREESPACE    5



#define MOVE_OUT_FROM_HOME                          0
#define MOVE_IN_GOAL                                 1
#define MOVE_TO_GLOBE                               2
#define MOVE_TO_STAR                                3
#define MOVE_TO_GOAL_VIA_STAR                       4
#define GET_INTO_SAFETY_WITH_AN_SAME_COLORED_TOKEN  5
#define GET_INTO_THE_WINNER_ROAD                    6
#define SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE       7
#define KILL_OPPONENT                               8
#define JUST_MOVE                                   9
#define NO_MOVE_POSSIBLE                            10



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
    gen(rd())
  {
    game_obj = obj;
    std::vector< std::vector<int> > QTable(NUMBER_OF_ACTIONS, std::vector<int>(NUMBER_OF_STATES));

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

std::vector<int> get_possible_actions(int token_pose, int dice_roll){
  int new_state = token_pose + dice_roll;

  std::vector<int> possible_actions;

  if(new_state > 0){
    possible_actions.push_back(MOVE_OUT_FROM_HOME);
  }

  if(new_state == GAME_POSITION_GOAL){
    possible_actions.push_back(MOVE_IN_GOAL);
  }

  if(game_obj->isStar(new_state)){
    possible_actions.push_back(MOVE_TO_STAR);
  }

  if(new_state > 50){ // If in goal-road
    possible_actions.push_back(GET_INTO_THE_WINNER_ROAD);
  }

  if(game_obj->isGlobe(new_state) ){ // If on goal
    state = ST_GLOBE;
  }

  //missing action with safe zone.
  possible_actions.push_back(JUST_MOVE);
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
    auto possible_actions = get_possible_actions(token_state, dice_roll);
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
