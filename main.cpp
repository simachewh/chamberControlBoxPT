#include <QCoreApplication>
#include <intro.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Intro intro;
    intro.opPort();
    //qDebug() << "Entetr control box port name:";

    //intro.wrToPort();
    return a.exec();
}
