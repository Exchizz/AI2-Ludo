#include "dialog.h"
#include <QApplication>
#include "game.h"
#include <vector>

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

int main(int argc, char *argv[])
{
    std::vector<int> relative_pos;
    int dice;
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    int iter = 200;
    while(iter--){
        w.next_turn(relative_pos, dice);
        w.next_turn(relative_pos, dice);
        w.next_turn(relative_pos, dice);
        w.next_turn(relative_pos, dice);
        w.movePiece(simple_player(relative_pos, dice));
        w.update_graphics();
    }
    for(auto i : relative_pos){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return a.exec();
}
