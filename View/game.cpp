#include "game.h"

game::game(){
    for(int i=0; i < 16; ++i){
        player_positions.push_back(-1);
    }
    player_positions[0] = 0;
    player_positions[4] = 13;
    player_positions[8] = 26;
    player_positions[12] = 39;
    rollDice();
    color = 2;
}

int game::rel_to_fixed(int relative_piece_index){
    return relative_piece_index + color * 4;
}

int game::isStar(int index){
    if(index == 5  ||
       index == 18 ||
       index == 31 ||
       index == 44){
        return 6;
    } else if(index == 11 ||
              index == 24 ||
              index == 37 ||
              index == 50){
        return 7;
    }
    return 0;
}

int game::isOccupied(int index){ //returns number of people
    int number_of_people = 0;
    if(index != 99){
        for(auto i : player_positions){
            if(i == index){
                ++number_of_people;
            }
        }
    }
    return number_of_people;
}

bool game::isGlobe(int index){
    if(index % 13 == 0 || (index - 8) % 13 == 0 || isOccupied(index) > 1){
        return true;
    }
    return false;
}

void game::send_them_home(int index){
    for(size_t i = 0; i < player_positions.size(); ++i){
        if(i < color*4 || i > color*4 + 4){ //different color
            if(player_positions[i] == index){
                player_positions[i] = -1;
            }
        }
    }
}

void game::move_start(int fixed_piece){
    if(dice_result == 6 && player_positions[fixed_piece] < 0){
        player_positions[fixed_piece] = color*13; //move me to start
        send_them_home(color*13); //send pieces home if they are on our start
    } else {
        std::cerr << "Invalid move!" << std::endl;
    }
}

int game::next_turn(std::vector<int> &relative_pos, int &dice){
    switch(color){
        case 0:
        case 1:
        case 2:
            ++color;
            break;
        case 3:
        default:
            color = 0;
            break;
    }
    rollDice();
    dice = getDiceRoll();
    relative_pos = relativePosition();
    return 0;
}

void game::movePiece(int relative_piece){
    if(relative_piece == -1){
        std::cerr << "\n\n\n GAME WON \n\n" << std::endl;
        return;
    }
    int fixed_piece = rel_to_fixed(relative_piece);
    int modifier = color * 13;
    int relative = player_positions[fixed_piece];
    int target_pos = 0;
std::cout << "p: " << color << " " << dice_result << " ";
    if(player_positions[fixed_piece] == -1){
        move_start(fixed_piece);
std::cout << "all_posis: " << player_positions[color * 4] << " "
          << player_positions[color * 4 + 1 ] << " "
          << player_positions[color * 4 + 2 ] << " "
          << player_positions[color * 4 + 3 ] << "\n";
    } else {
        //convert to relative position
std::cout << "fix: " << relative << " ";
        if(relative == 99){
            std::cout << "I tought this would be it ";
        } else if(relative < modifier) {
            relative = relative + 52 - modifier;
        } else if( (relative - modifier) > 50) {
            relative = relative - color * 5;
        } else {//if(relative >= modifier)
            relative = relative - modifier;
        }
std::cout << "pos: " << relative << " ";
        //add dice roll
        relative += dice_result;

std::cout << relative << " ";

        int jump = isStar(relative); //return 0 | 6 | 7
        if(jump){
            if(jump + relative == 57){
                relative = 56;
            } else {
                relative += jump;
            }
        }
        //special case checks
        if(relative > 56 && relative < 72){ // go back
std::cout << "go back ";
            target_pos = 56-(relative-56) + color * 5; //trust me
        }else if(relative == 56 || relative >= 99){
std::cout << "GOAL!!! ";
            target_pos = 99;
        }else if(relative > 50){ // goal stretch
std::cout << "goal stretch ";
            target_pos = relative + color * 5 + 1;
        } else {
std::cout << "normal move ";
            target_pos = relative + color * 13;
        }
        //check for game stuff

        if(isOccupied(target_pos)){
            if(isGlobe(target_pos)){
                target_pos = -1; //send me home
            } else {
                send_them_home(target_pos);
            }
        }

        if(target_pos - player_positions[fixed_piece] == 0){
            std::cout << "\rall_posis: " << player_positions[color * 4] << " "
                      << player_positions[color * 4 + 1 ] << " "
                      << player_positions[color * 4 + 2 ] << " "
                      << player_positions[color * 4 + 3 ] << "\n";
        } else {
        //move piece
        std::cout << "moved the piece " << fixed_piece << " " << target_pos - player_positions[fixed_piece] << " end: " << target_pos << std::endl;
        }
        player_positions[fixed_piece] = target_pos;
    }
}

std::vector<int> game::relativePosition(){
    std::vector<int> relative_positons;
    int modifier = color * 13;

    //from start id to end
    for(int i = color*4; i < player_positions.size(); ++i){
        relative_positons.push_back(player_positions[i]);
    }
    //from 0 to start id
    for(int i = 0; i < color*4; ++i){
        relative_positons.push_back(player_positions[i]);
    }
    for(size_t i = 0; i < relative_positons.size(); ++i){
        if(relative_positons[i] == 99 || relative_positons[i] == -1){
            relative_positons[i] = (relative_positons[i]);
        } else if(relative_positons[i] < modifier) {
            relative_positons[i] = (relative_positons[i]+52-modifier);
        } else if(relative_positons[i] > modifier) {
            relative_positons[i] = (relative_positons[i]-modifier);
        } else if(relative_positons[i] + modifier > 50) {
            relative_positons[i] = (relative_positons[i]-color*5);
        }
    }
    return std::move(relative_positons);
}
