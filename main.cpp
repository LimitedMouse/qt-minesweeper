#include "mainwindow.h"
#include "cmine.h"
#include <QApplication>
#include <QMediaPlayer>
int main(int argc, char *argv[])
{


    QMediaPlayer *startSound=new QMediaPlayer;  //创建一个音乐播放器
    //connect(startSound,SIGNAL(positionChanged(qint64)),this,SLOT(postitionChanged(qint64)));
    startSound->setMedia(QUrl::fromLocalFile("C:\\Users\\86178\\Music\\Weight of the World.wav"));
    startSound->setVolume(50);  //音量
    startSound->play();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //Cmine cmine;
    //cmine.show();
    return a.exec();
}
