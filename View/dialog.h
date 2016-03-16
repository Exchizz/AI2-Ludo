#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>
#include <vector>
#include <utility> //std::pair
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void move_goal(int color);
    void move_home(int color);
    void move_piece(int piece_index,int field_index){
        graphic_player[piece_index]->setPos(fieldPos[field_index]);
    }

private:
    void addHomeField(int,int,QBrush);
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
};

#endif // DIALOG_H
