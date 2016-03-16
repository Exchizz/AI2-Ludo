#include "dialog.h"
#include <QApplication>
#include "game.h"

int main(int argc, char *argv[])
{
    game ludo;
    ludo.rollDice();
    ludo.movePiece(2);
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
