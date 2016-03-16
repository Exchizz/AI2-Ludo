#include "game.h"

game::game(){
    QBrush piece;
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);
    for(int c = 0; c<4; ++c){
        if(c == 0){
            piece = QBrush(QColor(Qt::green));
        } else if(c == 1){
            piece = QBrush(QColor(Qt::yellow));
        } else if(c == 2){
            piece = QBrush(QColor(Qt::red));
        } else if(c == 3){
            piece = QBrush(QColor(Qt::blue));
        }
        for(int i = 0; i<4; ++i)
            graphic_player.push_back(scene->addEllipse(5,5,40,40,blackPen,piece));
    }
}

int game::rel_to_fixed(int relative_piece_index){
    return relative_piece_index + color * 4;
}

int game::isStar(int index){
    if(index == 5 ||
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
    for(auto i : player_positions){
        if(i == index){
            ++number_of_people;
        }
    }
    return number_of_people;
}

bool game::isGlobe(int index){
    if(index % 13 == 0 || (index - 8) % 13 == 0 || isOccupied(index) > 1){
        return true;
    }
}

void game::send_them_home(int index){
    for(size_t i = 0; i < player_positions; ++i){
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
        std::cerr << "Invalid move!\n";
    }
}

void game::movePiece(int relative_piece){
    int fixed_piece = rel_to_fixed(relative_piece);
    int modifier = color * 13;
    int relative = player_positions[fixed_piece];
    int target_pos;

    if(player_positions[fixed_piece] == -1){
        move_start(fixed_piece);
    } else {
        //convert to relative position
        if(relative < modifier)
            relative = relative + 52 - modifier;
        else if(relative > modifier)
            relative = relative - modifier;
        else if(relative + modifier > 50)
            relative = relative - color * 5;

        //add dice roll
        relative += dice_result;
        int jump = isStar(relative); //return 0 | 6 | 7

        if(jump + relative == 57){
            relative = 56;
        } else {
            relative += jump;
        }
        //special case checks
        if(relative > 56) // go back
                target_pos = 56-(relative-56) + color * 5; //trust me
        else if(relative > 50) // goal stretch
                target_pos = relative + color * 5;
        else
            target_pos = relative + color * 13;

        //check for game stuff

        if(isOccupied(target_pos)){
            if(isGlobe(target_pos)){
                target_pos = -1; //send me home
            } else {
                send_them_home(target_pos);
            }
        }

        //move piece
        player_positions[fixed_piece] = target_pos;
    }
    //update graphic
    for(auto i : player_positions){
        if(i == -1){
//            move_home()
        } else if(i == 56){
//            move_goal();
        } else {
//            move_piece(fixed_piece, i);
        }
    }
}

std::vector<int> game::relativePosition(){
    std::vector<int> relative_positons = player_positions;
    int modifier = color * 13;
    for(size_t i = 0; i < player_positions.size(); ++i){
        if(relative_positons[i] < modifier)
            relative_positons[i] = relative_positons[i] + 52 - modifier;
        else if(relative_positons[i] > modifier)
            relative_positons[i] = relative_positons[i] - modifier;
        else if(relative_positons[i] + modifier > 50)
            relative_positons[i] = relative_positons[i] - color * 5;
    }
    return std::move(relative_positons);
}
