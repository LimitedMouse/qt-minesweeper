#include "cmine.h"

#include <QDebug>
#include <QMouseEvent>


Cmine::Cmine(QWidget *parent,int row,int col,int num_mine,int d) : QWidget(parent),
    row(row),
    col(col),
    d(d),
    num_mine(num_mine)
{
    //row=9;
    //col=9;
    //d=20;
    draw_mousemove=false;
    gameover=false;
    count=row*col-num_mine;
    offset=0.5;
    int size=row*col;
    //num_mine=10;
    board.fill(QVector<char>(col,'0'),row);
    _board.fill(QVector<char>(col,'0'),row);
    //_board[0][2]='f';
    for(int i=0;i<num_mine;i++)
    {
        int randint=rand()%size;
        int _row=randint/col;
        int _col=randint%col;
        if(board[_row][_col]!='m'){
            board[_row][_col]='m';}
        else{
            i--;
        }
    }
    calculateMineAround();
    this->setFixedSize(col*d,row*d);
    this->setWindowFlag(Qt::FramelessWindowHint);
    setMouseTracking(true);

}

void Cmine::calculateMineAround()
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(board[i][j]=='0')
            {
                int count=0;
                if(j+1<col&&board[i][j+1]=='m')
                {
                    count++;
                }
                if(j-1>=0&&board[i][j-1]=='m')
                {
                    count++;
                }
                for(int k=-1;k<=1;k++)
                {
                    if(i-1>=0&&j+k>=0&&j+k<col&&board[i-1][j+k]=='m')
                    {
                        count++;
                    }
                    if(i+1<row&&j+k>=0&&j+k<col&&board[i+1][j+k]=='m')
                    {
                        count++;
                    }
                }
                board[i][j]='0'+count;
            }

        }
    }
}

void Cmine::paintEvent(QPaintEvent *)
{
    painter=new QPainter;

    draw_Board();



    if(draw_mousemove)
    {
        //qDebug()<<"mousemove";
        draw_Mousemove(mousemove_row,mousemove_col);
        draw_mousemove=false;
    }
}

void Cmine::mouseMoveEvent(QMouseEvent *event)
{
    double x=event->x();
    double y=event->y();
    mousemove_row=y/d;
    mousemove_col=x/d;
    draw_mousemove=true;
    update();
}

void Cmine::uncoverBlocks(int row, int col)
{
    // Check if the specified row and column are within the board bounds.
    if (row >= 0 && row < this->row && col >= 0 && col < this->col)
    {
        if(_board[row][col]!='o'){
                if(board[row][col]!='0'){
                    count--;
                    emit sig_count();
                    _board[row][col]='o';
                }
                else{
                    _board[row][col]='o';
                    count--;
                    emit sig_count();

                    // Uncover the blocks around this block.
                    uncoverBlocks(row - 1, col);
                    uncoverBlocks(row - 1, col-1);
                    uncoverBlocks(row - 1, col+1);
                    uncoverBlocks(row + 1, col);
                    uncoverBlocks(row + 1, col-1);
                    uncoverBlocks(row + 1, col+1);
                    uncoverBlocks(row, col - 1);
                    uncoverBlocks(row, col + 1);
                }
            }
    }






        }



void Cmine::mousePressEvent(QMouseEvent *event)
{
    double x=event->x();
    double y=event->y();
    int mousepress_row=y/d;
    int mousepress_col=x/d;
    if(event->button()==Qt::LeftButton)
    {


        if(board[mousepress_row][mousepress_col]=='m')
        {
            gameover=true;
            _board[mousepress_row][mousepress_col]='o';
            emit sig_lost();
        }
        else
        {
            uncoverBlocks(mousepress_row,mousepress_col);





        }
    }


    if(event->button()==Qt::RightButton)
    {


        if(_board[mousepress_row][mousepress_col]=='?')
        {
            //qDebug()<<"f";
            _board[mousepress_row][mousepress_col]='f';
        }
        else if(_board[mousepress_row][mousepress_col]=='f'||_board[mousepress_row][mousepress_col]=='0')
        {
            _board[mousepress_row][mousepress_col]='?';
        }
        else
        {

        }

    }

    update();
}

void Cmine::draw_Board()
{

    painter->begin(this);
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            //qDebug()<<_board[i][j];
            switch (_board[i][j])
            {
            case '0':
                //qDebug()<<"0";
                draw_None(i,j);
                break;
            case '?':
                //qDebug()<<"?";
                //if(board[i][j])
                draw_Ques(i,j);
                break;
            case 'f':
                //qDebug()<<"ff";
                draw_Flag(i,j);
                break;
            case 'o':
                //qDebug()<<"o";
                draw_Open(i,j);
                break;
            default:
                //qDebug()<<"draw";
                break;
            }
        }
    }
    painter->end();
}



void Cmine::draw_None(int _row, int _col)
{
    painter->setBrush(Qt::gray);
    painter->drawRect(_col*d+offset,_row*d+offset,d-offset*2,d-offset*2);
    painter->setBrush(Qt::NoBrush);
}

void Cmine::draw_Ques(int _row, int _col)
{
    draw_None(_row,_col);
    QPen pen=painter->pen();
    painter->setPen(QPen(Qt::red));
    QFont font=painter->font();
    QFont font1=font;
    //qDebug()<<font.pointSize()<<font.weight();
    font.setPointSize(15);

    painter->setFont(font);
    painter->drawText(_col*d,_row*d,d,d,5,"?");
    painter->setPen(pen);
    painter->setFont(font1);
}

void Cmine::draw_Flag(int _row, int _col)
{
    draw_None(_row,_col);

    QImage _image(":/icons/flag.jpg",".jpg");
    _image=_image.scaled(d,d);
    //qDebug()<<_image.depth();
    auto data=_image.bits();
    QImage image(data,d,d,QImage::Format_ARGB32);
    //qDebug()<<image.depth();
    image=getTransparentImage(image,QPoint(1,1));
    //qDebug()<<image.pixelColor(1,1);
    //qDebug()<<image.size();
    painter->drawImage(_col*d,_row*d,image);
}

void Cmine::draw_Open(int _row, int _col)
{
    if(board[_row][_col]=='m')
    {
        QImage image(":/icons/mine.jpg");
        image=image.scaled(d-offset*2,d-offset*2);
        //qDebug()<<image.size();
        //QPen pen=painter->pen();
        //auto pen1=pen;
        //pen.setWidth(5);
        //painter->setPen(pen);
        //painter->setBrush(QBrush(Qt::black));
        painter->drawRect(_col*d+offset*0.5,_row*d+offset*0.5,d-offset,d-offset);
        //painter->setPen(pen1);
        //painter->setBrush(Qt::NoBrush);
        painter->drawImage(_col*d+offset,_row*d+offset,image);

        _board.fill(QVector<char>(col,'o'),row);
    }
    else
    {
        //painter->setBrush(Qt::gray);
        painter->drawRect(_col*d+offset,_row*d+offset,d-offset*2,d-offset*2);
        //painter->setBrush(Qt::NoBrush);
        QPen pen=painter->pen();
        painter->setPen(QPen(Qt::blue));
        QFont font=painter->font();
        QFont font1=font;
        //qDebug()<<font.pointSize()<<font.weight();
        font.setPointSize(15);

        painter->setFont(font);
        painter->drawText(_col*d,_row*d,d,d,5,QString(board[_row][_col]));
        painter->setPen(pen);
        painter->setFont(font1);


    }
}

void Cmine::draw_Mousemove(int _row, int _col)
{
    painter->begin(this);
    //QPen pen=painter->pen();
    //painter->setPen(QColor(45,45,45,0));
    painter->setBrush(QBrush(QColor(255,255,255,150)));
    painter->drawRect(_col*d-offset,_row*d-offset,d-offset*2,d-offset*2);
    //qDebug()<<"mouse";
    //painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->end();
}

QImage Cmine::getTransparentImage(QImage image, QPoint point)
{
    if(image.valid(point))
    {
        QRgb rgb=image.pixel(point);
        //qDebug()<<rgb;
        //QVector<QRgb> vec_rgb=image.colorTable();
        //qDebug()<<vec_rgb;
        for(int i=0; i<image.width();i++)
        {
            for(int j=0;j<image.height();j++)
            {
                if(rgb==image.pixel(i,j))
                {
                    QColor rgba = QColor(0, 1, 1, 1);
                    //qDebug()<<i<<j;
                    image.setPixelColor(i,j,rgba);
                }
            }
        }
    }
    return image;
}
