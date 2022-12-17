#ifndef CMINE_H
#define CMINE_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMediaPlayer>


/*
 board:
 '0'~'8': number of mines around
 'm': mine
 _board:
 '0': none
 'f': flag
 '?': ?
 'o': open
 */
class Cmine : public QWidget
{
    Q_OBJECT
public:
    explicit Cmine(QWidget *parent = nullptr,int row=9,int col=9,int num_mine=10,int d=20);
    QVector<QVector<char>> board;
    QVector<QVector<char>> _board;
    int row;
    int col;
    int mousemove_row;
    int mousemove_col;
    //int mousepress_row;
    //int mousepress_col;
    int d;
    int count;
    double offset=0.5;
    int num_mine;
    bool draw_mousemove;
    bool gameover;
    QPainter* painter;

    void calculateMineAround();
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void draw_Board();
    void draw_None(int, int);
    void draw_Ques(int, int);
    void draw_Flag(int, int);
    void draw_Open(int, int);
    void draw_Mousemove(int,int);
    void uncoverBlocks(int row, int col);
    QImage getTransparentImage(QImage image,QPoint point);
signals:
    void sig_lost();
    void sig_count();
};

#endif // CMINE_H
