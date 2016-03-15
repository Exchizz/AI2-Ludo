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

    std::vector<std::pair<QColor,QColor> >base_colors {
        std::make_pair(QColor(92,170,119),QColor(185,219,125)), //G
        std::make_pair(QColor(237,235,89),QColor(237,234,138)),  //Y
        std::make_pair(QColor(92,93,170),QColor(111,111,170)),  //B
        std::make_pair(QColor(237,57,60),QColor(237,114,125))   //R
    };

    QBrush greenBrush(QColor(132,195,38));
    QBrush white(Qt::white);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);
//    QPen GreenStart(QColor(92,170,119));
//    GreenStart.setWidth(2);

    // Cross
    scene->addRect(415,-155,160,960,blackPen,QBrush(QColor(195,195,194)));
    scene->addRect(15,245,960,160,blackPen,QBrush(QColor(195,195,194)));
    scene->addRect(415,245,160,160,QPen(QColor(195,195,194)),QBrush(QColor(195,195,194))); //clean center

    // Goal stretch
    scene->addRect(50,290,350,70,blackPen,QBrush(base_colors[0].first));
    scene->addRect(460,-120,70,350,blackPen,QBrush(base_colors[1].first));
    scene->addRect(590,290,350,70,blackPen,QBrush(base_colors[2].first));
    scene->addRect(460,420,70,350,blackPen,QBrush(base_colors[3].first));

    int x_pos = -10; //start place for green
    int y_pos = 220;
    int offset = 70;
    int small_offset = 50;
    int large_offset = 80;

    for(int x=60; x<=340; x+=offset)
        scene->addEllipse(x,300,50,50,blackPen,white);
    for(int y=-110; y<=170; y+=offset)
        scene->addEllipse(470,y,50,50,blackPen,white);
    for(int x=600; x<=950; x+=offset)
        scene->addEllipse(x,300,50,50,blackPen,white);
    for(int y=430; y<=780; y+=offset)
        scene->addEllipse(470,y,50,50,blackPen,white);

    //home fields
    addHomeField(0,0,QBrush(base_colors[0].first));
    addHomeField(630,-170,QBrush(base_colors[1].first));
    addHomeField(800,445,QBrush(base_colors[2].first));
    addHomeField(190,630,QBrush(base_colors[3].first));

    // Playing fields
    // x,y
    std::vector<std::pair<int,int> > fieldPos;


    std::vector<std::pair<char,char> > directions{std::make_pair(1,-1),std::make_pair(1,1),std::make_pair(-1,1),std::make_pair(-1,-1) };
    for(size_t d =0; d < directions.size(); ++d){
        for(int i=0; i<5;++i){
            if(d % 2 == 0)
                x_pos += directions[d].first * offset;
            else
                y_pos += directions[d].second * offset;
            fieldPos.push_back(std::make_pair(x_pos,y_pos));
        }
        x_pos += directions[d].first * small_offset;
        y_pos += directions[d].second * small_offset;
        for(int i=0; i<5;++i){
            fieldPos.push_back(std::make_pair(x_pos,y_pos));
            if(d % 2 == 0)
                y_pos += directions[d].second * offset;
            else
                x_pos += directions[d].first * offset;
        }
        for(int i=0; i<2;++i){
            fieldPos.push_back(std::make_pair(x_pos,y_pos));
            if(d % 2 == 0)
                x_pos += directions[d].first * large_offset;
            else
                y_pos += directions[d].second * large_offset;
        }
        fieldPos.push_back(std::make_pair(x_pos,y_pos));
    }

    for(size_t c = 0; c < base_colors.size(); ++c){
        scene->addEllipse(fieldPos[0+13*c].first,fieldPos[0+13*c].second,50,50,QPen(base_colors[c].first),QBrush(base_colors[c].second));
        for(int i=1; i < 13; ++i){
            std::cout << fieldPos[i+13*c].first << "," << fieldPos[i+13*c].second << std::endl;
            scene->addEllipse(fieldPos[i+13*c].first,fieldPos[i+13*c].second,50,50,blackPen,white);
        }
    }
    /*
    for(unsigned int i = 0; i < fieldPos.size(); i++){
        std::cout << fieldPos[i].first << "," << fieldPos[i].second << std::endl;
        if(i % 13 == 0){
            scene->addEllipse(fieldPos[i].first,fieldPos[i].second,50,50,GreenStart,QBrush(QColor(185,219,125)));
        }else{
            scene->addEllipse(fieldPos[i].first,fieldPos[i].second,50,50,blackPen,white);
        }
    }
    */
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
