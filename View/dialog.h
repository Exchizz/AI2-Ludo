#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>
#include <vector>
#include <utility> //std::pair
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include "game.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog, public game
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void move_goal(int color);
    void move_home(int color);
    void move_piece(int piece_index,int field_index);
    void update_graphics();
private:
    void addHomeField(int,int,QBrush);
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    void create_graphic_players();
    std::vector<QGraphicsEllipseItem *> graphic_player;
    std::vector<QPointF> home_fields;
    std::vector<QPointF> fieldPos;

};

#endif // DIALOG_H
