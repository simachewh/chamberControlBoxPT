#include "intro.h"
#include <QDebug>
#include <string>

using namespace std;

Intro::Intro(QObject *parent) : QObject(parent)

{

    serial = new QSerialPort(this);
    open = false;

    currentValues = new QByteArray();
    recievedData = new QByteArray();
    commOff = new QByteArray();
    questionMarks = new QByteArray();

    dryTemp = 20;
    wetTemp = 18;
    humidity = 67;

    ack = new QByteArray(1, 0x06);
    setCommOff();
    setQuestionMarks();
    initValues();



//    QString s(*currentValues);
//    qDebug() << s;

    connect(serial, SIGNAL(readyRead()), this, SLOT(readPort()));
    connect(this, SIGNAL(newData()), this, SLOT(on_newData_arived()));

}

Intro::~Intro(){

}

void Intro::opPort(){
    serial->setPortName("ttyUSB1");
    open = serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);

    qDebug() << "opPort()- Port is open: " << open << endl;
}

void Intro::wrToPort(QByteArray data){

    if(serial->isOpen()){
        serial->write(data);
        QString str(data);
        qDebug() << "wrToPort()- Serial data : " << str << endl;
    }else{
        qDebug() << "wrToPort()- Serial port is open : " << open << endl;
    }
}


void Intro::readPort(){
    QByteArray *lineEnd = new QByteArray(1, 0x0D);
    recievedData->append(serial->readAll());
    if(recievedData->endsWith(*lineEnd)){
        emit newData();
    }
}

void Intro::on_newData_arived(){
    if(!recievedData->isEmpty()){
        char comandId = recievedData->at(2);
        switch (comandId) {
        case 'A':
            wrToPort(*currentValues);
            break;
        case 'I':
            wrToPort(*questionMarks);
            break;
        case 'B':
            wrToPort(*commOff);
            break;
        case 'O':
            wrToPort(*ack);
            break;
        default:
            break;
        }
    }
}


void Intro::initValues(){

    if(dryTemp < 150 ){
        dryTemp += 3.95;
    }

    if(humidity < 99){
        humidity += 3.8;
    }
    wetTemp = dryTemp - humidity/20;

    QString ss = "0A+" + QString("%1").arg(dryTemp, 6, 'f', 2, '0') + " +"
            + QString("%1").arg(wetTemp, 6, 'f', 2, '0') + " "
            + QString("%1").arg(humidity, 4, 'f', 1, '0') + "\n";
    qDebug() << ss<< endl;
    QByteArray baStr = ss.toLatin1();
    *currentValues = ss.toLatin1();
}


void Intro::setCommOff(){
    /**0x02 0x30 0x42 0x3c 0x43 0x4f 0x4d 0x4d 0x55 0x4e 0x49 0x43 0x41
     * 0x54 0x49 0x4f 0x4e 0x4f 0x46 0x46 0x3e 0x03 0xdd 0x0d*/
    commOff->append(0x02).append(0x30).append(0x42).append(0x3c)
            .append(0x43).append(0x4f).append(0x4d).append(0x55)
            .append(0x4e).append(0x49).append(0x43).append(0x41)
            .append(0x54).append(0x49).append(0x4f).append(0x4e)
            .append(0x4f).append(0x4e).append(0x4f).append(0x46)
            .append(0x46).append(0x3e).append(0x03).append(0xdd)
            .append(0x0d);
}

void Intro::setQuestionMarks(){
/**0x02 0x30 0x49 0x3f 0x3f 0x3f 0x3f 0x3f 0x3f 0x3f 0x3f 0x3f 0x3f
 * 0x3f 0x3f 0x03 0x6d */
    char c = 0x3f;
    QByteArray *ba = new QByteArray(12,c);
    questionMarks->append(0x02).append(0x30).append(0x49).append(*ba)
            .append(0x03).append(0x6d).append(0x0d);

}
