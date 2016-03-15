#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(240,240,239)));

    // Colors
    QBrush greenBrush(QColor(132,195,38));
    QBrush white(Qt::white);
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);
    QPen GreenStart(QColor(92,170,119));
    GreenStart.setWidth(2);

    // Cross
    scene->addRect(15,245,1000,140,blackPen,QBrush(QColor(195,195,194)));
    scene->addRect(415,-155,160,400,blackPen,QBrush(QColor(195,195,194)));
    scene->addRect(415,385,160,400,blackPen,QBrush(QColor(195,195,194)));

    // Goal stretch
    scene->addRect(50,280,350,70,blackPen,greenBrush);
    scene->addEllipse(60,290,50,50,blackPen,white);
    scene->addEllipse(130,290,50,50,blackPen,white);
    scene->addEllipse(200,290,50,50,blackPen,white);
    scene->addEllipse(270,290,50,50,blackPen,white);
    scene->addEllipse(340,290,50,50,blackPen,white);

    // Green home
    addHomeField(0,0,greenBrush);

    // Playing fields
    std::vector<std::pair<int,int> > fieldPos {
        // 51,0-4
        std::make_pair(-10,220),
        std::make_pair(60,220),
        std::make_pair(130,220),
        std::make_pair(200,220),
        std::make_pair(270,220),
        std::make_pair(340,220),
        // 5-10
        std::make_pair(390,170),
        std::make_pair(390,100),
        std::make_pair(390,30),
        std::make_pair(390,-40),
        std::make_pair(390,-110),
        std::make_pair(390,-180),
        // 11
        // 12-17
        // 18-23
        // 24
        // 25-30
        // 31-36
        // 37
        // 38-43
        // 44-49
        std::make_pair(340,360),
        std::make_pair(270,360),
        std::make_pair(200,360),
        std::make_pair(130,360),
        std::make_pair(60,360),
        std::make_pair(-10,360),
        // 50
        std::make_pair(-10,290)
    };
    for(unsigned int i = 0; i < fieldPos.size(); i++){
        std::cout << fieldPos[i].first << "," << fieldPos[i].second << std::endl;
        if(i == 1){
            scene->addEllipse(fieldPos[i].first,fieldPos[i].second,50,50,GreenStart,QBrush(QColor(185,219,125)));
        }else{
            scene->addEllipse(fieldPos[i].first,fieldPos[i].second,50,50,blackPen,white);
        }
    }

    //ui->graphicsView->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showEvent(QShowEvent *) {
    ui->graphicsView->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

void Dialog::resizeEvent(QResizeEvent *){
    ui->graphicsView->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

void Dialog::addHomeField(int x, int y,QBrush brush){
    QBrush whiteBrush(Qt::white);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);

    scene->addEllipse(x,y,180,180,blackPen,brush);
    scene->addEllipse(x+65,y+15,50,50,blackPen,whiteBrush);
    scene->addEllipse(x+65,y+115,50,50,blackPen,whiteBrush);
    scene->addEllipse(x+15,y+65,50,50,blackPen,whiteBrush);
    scene->addEllipse(x+115,y+65,50,50,blackPen,whiteBrush);
}
