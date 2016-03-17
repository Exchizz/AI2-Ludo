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
#include <QGraphicsEllipseItem>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
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
public slots:
    void update_graphics(std::vector<int> player_positions);
};

#endif // DIALOG_H
