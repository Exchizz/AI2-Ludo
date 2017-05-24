#include "ludo_player_Qlearning.h"

#define NUMBER_OF_STATES    6
#define NUMBER_OF_ACTIONS   10

#define GAME_POSITION_HOME -1
#define GAME_POSITION_GOAL 56

#define STATE(var)        std::get<0>(var)
#define ACTION(var)       std::get<1>(var)
#define TOKEN(var)        std::get<2>(var)
#define POSITION(var)     std::get<3>(var)



// States
#define ST_HOME         0
#define ST_GOAL         1
#define ST_GLOBE        2
#define ST_STAR         3
#define ST_WINNER_ROAD  4
#define ST_FREESPACE    5

// Actions
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

bool is_star(int pos){
  std::vector<int> v{ 5, 11, 18, 24, 31, 37, 44, 50}; // Globes, start pose(when 6) not included
  return (std::find(v.begin(), v.end(), pos) != v.end());
}
bool is_globe(int pos){
  std::vector<int> v{ 8, 21, 34, 47 }; // Globes, start pose(when 6) not included
  return (std::find(v.begin(), v.end(), pos) != v.end());
}
void QTabledumper(std::vector< std::vector<float> > &Qtable){
  std::cout << "QTable -----------Action---------------------" << std::endl;
  std::cout << "   ";
  for(unsigned int i = 0; i < NUMBER_OF_ACTIONS ;i++){
    std::cout<< " " << i << "\t";
  }
  std::cout << std::endl;

  for(unsigned int i = 0; i < NUMBER_OF_ACTIONS*7+11 ;i++){
    std::cout<< "-";
  }
  std::cout << std::endl;
  for(unsigned int state = 0; state < Qtable.size(); state++){
    std::cout << state << " |";
    for(unsigned int action = 0; action < Qtable[0].size(); action++){
      std::cout << " " << Qtable[state][action] << "\t";
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
    QTable(NUMBER_OF_STATES, std::vector<float>(NUMBER_OF_ACTIONS))
  {
    game_obj = obj;

    // State 2, action 3 = 200;
    // QTable[2][3] = 200;
    // QTable[ST_WINNER_ROAD][MOVE_IN_GOAL] = 100;
    // QTable[ST_FREESPACE][MOVE_TO_GOAL_VIA_STAR] = 100;
    // QTable[ST_FREESPACE][MOVE_IN_GOAL] = 100;
    prev_token_positions.resize(4); // Four tokens

    QTabledumper(QTable);
  }

bool ludo_player_Qlearning::InSafety(int position){
  for(int i = 0; i < 4; i++){
    if(pos_start_of_turn[i] > 0 && pos_start_of_turn[i] < 50){ // if out in freespace, and not in goalzone.
      if(pos_start_of_turn[i] == position){
        return true;
      }
    }
  }
  return false;
}
int ludo_player_Qlearning::get_current_state(int token_state){
  int state = ST_FREESPACE;

  // If token is out and is on globe
  // Might divide this up later..
  if( token_state > 0 && game_obj->isGlobe(token_state) ){
    state = ST_GLOBE;
  }

  if(token_state == 56 || token_state == 99){ // both is goal, for some reason
    state = ST_GOAL;
  }

  if (token_state == GAME_POSITION_HOME){
    state = ST_HOME;
  }

  if(game_obj->isStar(token_state)){
    state = ST_STAR;
  }

  if(token_state > 50 && token_state < 99){ // If higher than 50, then in winner road
    state = ST_WINNER_ROAD;
  }
  return state;
}
int ludo_player_Qlearning::get_possible_action(int token_pose, int _dice_roll){
  float new_pose = token_pose + _dice_roll;
  int action = -2;

  if(token_pose >= 56 ){ // if in goal, the token can't do anything
    return -1; // no actions possible
  }

  if(new_pose  == (-1 + 6)){ // in home = -1, 6 to move out
    action = MOVE_OUT_FROM_HOME;
  }


  if(new_pose == 50){ // 50 is star which jumps token to goal
    action = MOVE_TO_GOAL_VIA_STAR;
  }

  if(InSafety(new_pose)){
    action = GET_INTO_SAFETY_WITH_AN_SAME_COLORED_TOKEN;
  }

  if(new_pose == 56){ // 56 is actual goal,
    //getchar();
    action = MOVE_IN_GOAL;
    return action;
  }

  if(is_star(new_pose) && token_pose != GAME_POSITION_HOME){
    std::cout << "Is star"  << new_pose<< std::endl;
    action = MOVE_TO_STAR;
  }

  if(new_pose > 50 and token_pose <= 50){ // If can move in goal road but currently isn't in goal road.
    action = GET_INTO_THE_WINNER_ROAD;
  }

  if( is_globe(new_pose) ){
    action = MOVE_TO_GLOBE;
  }

  // If no other actions are possible.
  if(action == -2){
    //missing action with safe zone.
    action = JUST_MOVE;
  }
  return action;
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


    default:
      std::cout << "UNDEFINED ACTION AT LINE" << __LINE__ << " function: " << __FUNCTION__ << std::endl;
  }

  return action_str;
}


state_action ludo_player_Qlearning::randomQ(std::vector<state_action> &possible_moves){
  if(possible_moves.size() == 0){
    std::cout << "EMPTY MSG RETURNED" << std::endl;
    state_action empty;
    return empty;
  }

  std::uniform_int_distribution<> dis(1, possible_moves.size());
  int random_action = dis(gen);


  return possible_moves[random_action-1];
}

state_action ludo_player_Qlearning::maxQ(std::vector<state_action> &possible_moves){
  int max = 0;
  state_action max_state_action;
  for(auto action : possible_moves){
    std::cout << "Current state: " << state_int_to_string(STATE(action)) << " action: " << action_int_to_string(ACTION(action)) << std::endl;
     int Tmpstate = STATE(action);
    int Tmpaction = ACTION(action);

    if(QTable[Tmpstate][Tmpaction] >= max){
      max = QTable[Tmpstate][Tmpaction];
      max_state_action = action;
    }
  }
  return max_state_action;
}


float ludo_player_Qlearning::CalculateImmediateReward(state_action & best_move){
  float reward = POSITION(best_move) + dice_roll;
  if(ACTION(best_move) == MOVE_IN_GOAL){
      reward += 100;
      std::cout << "\t" << "Increment reward by 100" << std::endl;
  }

  if(ACTION(best_move) == MOVE_TO_GOAL_VIA_STAR){
      reward += 150;
      std::cout << "\t" << "Increment reward by 150" << std::endl;
  }

  if(ACTION(best_move) == MOVE_OUT_FROM_HOME){
    reward+=10;
  }

  if(ACTION(best_move) == MOVE_TO_STAR){
    reward+=20;
  }

  if(ACTION(best_move) == GET_INTO_SAFETY_WITH_AN_SAME_COLORED_TOKEN){
    reward+=20;
  }

  std::cout << "Reward: " << reward << std::endl;
  return reward;
}

int ludo_player_Qlearning::make_decision(){
  std::vector<state_action>possiblePlayerMoves;
  // For each token
  for(int token_i = 0; token_i < 4; token_i++){
    // Get current state
    int current_state = get_current_state(pos_start_of_turn[token_i]);

    // Calculate possible actions based on state and dice_roll
    auto possible_action = get_possible_action(pos_start_of_turn[token_i], dice_roll);

    std::cout << "token#" << token_i << " action: " << possible_action << std::endl;
    possiblePlayerMoves.push_back(std::make_tuple(current_state, possible_action, token_i, pos_start_of_turn[token_i]));
    // for(auto possible_action : possible_actions){
    //   possiblePlayerMoves.push_back(std::make_tuple(current_state, possible_actions.first, token_i,pos_start_of_turn[token_i], possible_action.second));
    // }
  }

  for(auto possible_action : possiblePlayerMoves){
    std::cout << "Token: " << TOKEN(possible_action) << " can do action " << action_int_to_string(ACTION(possible_action)) << std::endl;
  }

  std::uniform_int_distribution<> dis(1, 100);
  int epsilon = dis(gen);
  state_action best_move;
  if(true){
    // Get best action given all possible actions
    best_move = maxQ(possiblePlayerMoves);
  } else {
    // Get random action from all ossbiel actions
    best_move = randomQ(possiblePlayerMoves);
  }

  std::cout << "Token " << TOKEN(best_move) << " should be moved, action: " << action_int_to_string(ACTION(best_move)) << std::endl;
  std::cout << std::endl;

  int tokenToMove = TOKEN(best_move);

  state_action current_pose;// = prev_token_positions[tokenToMove];


  for(auto move: possiblePlayerMoves){
    std::cout << "\t\tDEBUG: token: " << TOKEN(move) << " state: " << STATE(move) << " position: " << POSITION(move) << std::endl;
    if(TOKEN(move) == TOKEN(best_move)){
      current_pose = move;
    }
  }

  float immediate_reward = CalculateImmediateReward(current_pose);
  float a = 0.5;
  float lambda = 0.1;

  std::cout << "\t" << " Best state: " << state_int_to_string(STATE(best_move)) << " Best action: " << action_int_to_string(ACTION(best_move)) << std::endl;
  std::cout << "\t" << " Cur. state: " << state_int_to_string(STATE(current_pose)) << " did action: " << action_int_to_string(ACTION(current_pose)) << std::endl;

  QTable[STATE(current_pose)][ACTION(best_move)] += a*(immediate_reward +\
  lambda*QTable[STATE(best_move)][ACTION(best_move)] - QTable[STATE(current_pose)][ACTION(best_move)]);

  std::cout << "tokenToMove: " << tokenToMove << " best_move's token: " << TOKEN(best_move)<< std::endl;
  prev_token_positions[tokenToMove] = best_move;
  QTabledumper(QTable);

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
    //
    // for(int i = 0; i < 4; i++){
    //   auto current_state = get_current_state(pos_end_of_turn[i]);
    //   std::cout << "post current state: " << state_int_to_string(current_state) << std::endl;
    // }
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
