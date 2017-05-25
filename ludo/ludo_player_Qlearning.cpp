#include "ludo_player_Qlearning.h"

#define NUMBER_OF_STATES    7
#define NUMBER_OF_ACTIONS   7

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
#define ST_SAFE         6

// Actions
#define MOVE_OUT_FROM_HOME                          0
#define MOVE_IN_GOAL                                1
#define MOVE_TO_GLOBE                               2
#define MOVE_TO_STAR                                3
#define MOVE_TO_SAFETY                              4
#define MOVE_TO_WINNER_ROAD                         5
#define JUST_MOVE                                   6
// Not currently implemented
#define SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE       7


bool is_star(int pos){
  std::vector<int> v{ 5, 11, 18, 24, 31, 37, 44, 50}; // stars, start pose(when 6) not included
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
    QTable(NUMBER_OF_STATES, std::vector<float>(NUMBER_OF_ACTIONS)){
    game_obj = obj;

    // State 2, action 3 = 200;
    // QTable[2][3] = 200;
    if(false){
      std::cerr << "-------------- Importing qtable from file" << std::endl;

      QTabledumper(QTable);
      importQTableFromFile(99999);
      QTabledumper(QTable);
    }
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

      case ST_SAFE:
        state_str = "ST_SAFE";
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

      case MOVE_TO_SAFETY:
        action_str = "MOVE_TO_SAFETY";
      break;

      case MOVE_TO_WINNER_ROAD:
        action_str = "MOVE_TO_WINNER_ROAD";
      break;

      case SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE:
        action_str = "SUICIDE_IF_THE_OPPONENT_IS_ON_A_GLOBE";
      break;

      case JUST_MOVE:
        action_str = "JUST_MOVE";
      break;

      default:
        std::cout << "UNDEFINED ACTION: " << token_action <<  "AT LINE: " << __LINE__ << " Function: " << __FUNCTION__ << std::endl;
    }

    return action_str;
  }


bool ludo_player_Qlearning::InSafety(int position, int token){
  for(int i = 0; i < 4; i++){
    if(pos_start_of_turn[i] > 0 && pos_start_of_turn[i] < 50){ // if out in freespace, and not in goalzone.
      if(pos_start_of_turn[i] == position && i != token){
        return true;
      }
    }
  }
  return false;
}
int ludo_player_Qlearning::get_current_state(int token_state, int token){
  int state = ST_FREESPACE;

  // If token is out and is on globe
  // Might divide this up later..
  if( token_state > 0 && is_globe(token_state) ){
    std::cout << "IN GLOBE STATE" << std::endl;
    state = ST_GLOBE;
  }

  if(token_state == 56 || token_state == 99){ // both is goal, for some reason
    state = ST_GOAL;
  }

  if (token_state == GAME_POSITION_HOME){
    state = ST_HOME;
  }

  if(InSafety(token_state,token)){
    std::cout << "in action safe and: " << state_int_to_string(state) << std::endl;
    state = ST_SAFE;
  }

  if(game_obj->isStar(token_state)){
    state = ST_STAR;
  }

  if(token_state > 50 && token_state < 99){ // If higher than 50, then in winner road
    state = ST_WINNER_ROAD;
  }
  return state;
}
int ludo_player_Qlearning::get_possible_action(int token_pose, int _dice_roll,int token){
  float new_pose = token_pose + _dice_roll;
  int action = -2;

  if(token_pose >= 56 ){ // if in goal, the token can't do anything
    return -1; // no actions possible
  }

  if(new_pose  == (-1 + 6)){ // in home = -1, 6 to move out
    action = MOVE_OUT_FROM_HOME;
  }

  if(InSafety(new_pose,token)){
    action = MOVE_TO_SAFETY;
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
    action = MOVE_TO_WINNER_ROAD;
  }

  if( is_globe(new_pose) ){
    action = MOVE_TO_GLOBE;
  }

  // If no other actions are possible.
  if(token_pose > -1 && action == -2){ // if token is out of home and no else action has been set.
    //missing action with safe zone.
    action = JUST_MOVE;
  }
  return action;
}


state_action ludo_player_Qlearning::randomQ(std::vector<state_action> &possible_moves){
  if(possible_moves.size() == 0){
    std::cout << "EMPTY MSG RETURNED" << std::endl;
    state_action empty;
    return empty;
  }

  std::uniform_int_distribution<> dis(1, possible_moves.size());
  int random_action = dis(gen);
  std::cout << "Random action: " << random_action << action_int_to_string(ACTION(possible_moves[random_action-1])) << std::endl;

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

  if(ACTION(best_move) == MOVE_OUT_FROM_HOME){
    reward+=100;
  }

  if(ACTION(best_move) == MOVE_TO_WINNER_ROAD){
    reward+=50;
  }

  if(ACTION(best_move) == MOVE_TO_STAR){
    reward+=80;
  }

  if(ACTION(best_move) == MOVE_TO_SAFETY){
    std::cerr << "Reward for moving to safety" << std::endl;
    reward+=100;
  }

  std::cout << "Reward: " << reward << std::endl;
  return reward;
}

int ludo_player_Qlearning::make_decision(){

  std::vector<state_action>possiblePlayerMoves;
  // For each token
  for(int token_i = 0; token_i < 4; token_i++){
    // Get current state
    int current_state = get_current_state(pos_start_of_turn[token_i],token_i);

    // Calculate possible actions based on state and dice_roll
    auto possible_action = get_possible_action(pos_start_of_turn[token_i], dice_roll,token_i);

    // std::cout << "token#" << token_i << " action: " << possible_action << std::endl;
    if(possible_action >= 0){
      // std::cerr << "Possible action: " << action_int_to_string(possible_action) << std::endl;

      possiblePlayerMoves.push_back(std::make_tuple(current_state, possible_action, token_i, pos_start_of_turn[token_i]));
    }
  }

  if(possiblePlayerMoves.size() == 0){
    std::cout << "No move can be done" << std::endl;
    return 0;
  }
  // for(auto possible_action : possiblePlayerMoves){
  //   std::cout << "Token: " << TOKEN(possible_action) << " can do action " << action_int_to_string(ACTION(possible_action)) << std::endl;
  // }

  std::uniform_int_distribution<> dis(1, 100);
  int epsilon = dis(gen);
  state_action best_move;
  if(epsilon < 70 ){
    // Get best action given all possible actions
    best_move = maxQ(possiblePlayerMoves);
  } else {
    // Get random action from all possbiel actions
    best_move = randomQ(possiblePlayerMoves);
  }

  // std::cout << "Token " << TOKEN(best_move) << " should be moved, action: " << action_int_to_string(ACTION(best_move)) << std::endl;
  // std::cout << std::endl;

  int tokenToMove = TOKEN(best_move);

  state_action current_pose;// = prev_token_positions[tokenToMove];
  for(auto move: possiblePlayerMoves){
    // std::cout << "\t\tDEBUG: token: " << TOKEN(move) << " state: " << STATE(move) << " position: " << POSITION(move) << std::endl;
    if(TOKEN(move) == TOKEN(best_move)){
      current_pose = move;
    }
  }

  float immediate_reward = CalculateImmediateReward(current_pose);
  float a = 0.4; //0.4
  float lambda = 0.1;

  // std::cout << "\t" << " Best state: " << state_int_to_string(STATE(best_move)) << " Best action: " << action_int_to_string(ACTION(best_move)) << std::endl;
  // std::cout << "\t" << " Cur. state: " << state_int_to_string(STATE(current_pose)) << " did action: " << action_int_to_string(ACTION(current_pose)) << std::endl;

  QTable[STATE(current_pose)][ACTION(best_move)] += a*(immediate_reward +\
  lambda*QTable[STATE(best_move)][ACTION(best_move)] - QTable[STATE(current_pose)][ACTION(best_move)]);

  std::cout << "tokenToMove: " << tokenToMove << " best_move's token: " << TOKEN(best_move)<< std::endl;
  //QTabledumper(QTable);
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


void ludo_player_Qlearning::dumpQTableToFile(int fileid){
  std::cout << "Saving Qtable to file....." << std::endl;
  std::ofstream outz(std::string("outputs/Qtable") + std::to_string(fileid) + std::string(".txt"));

  if(!outz){
    std::cout << "------------------------- Cannot save qtable" << std::endl;
  }

  for(const auto& row : QTable) {
    std::copy(row.cbegin(), row.cend(), std::ostream_iterator<double>(outz, " "));
  }
  outz.close();
}

void ludo_player_Qlearning::importQTableFromFile(int fileid){
  std::ifstream fin (std::string("outputs/learn-5000ittr/Qtable") + std::to_string(fileid) + std::string(".txt"));
  if (fin.is_open()){
    for (int state = 0; state < NUMBER_OF_STATES; state++){
      for (int action = 0; action < NUMBER_OF_ACTIONS; action++){
        float item = 0.0;
        fin >> item;
        QTable[state][action] = item;
      }
    }
    fin.close();
  } else {
    std::cerr << "Unable to read qtable file" << std::endl;
  }
}
