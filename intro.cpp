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
    humidity = 13;

    ack = new QByteArray(1, 0x06);
    setCommOff();
    setQuestionMarks();
    initValues();

    timer = new QTimer(this);
    i = 1;


//    QString s(*currentValues);
//    qDebug() << s;
    connect(timer, SIGNAL(timeout()), this, SLOT(testSendSlot()));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readPort()));
    connect(this, SIGNAL(newData()), this, SLOT(on_newData_arived()));

    timer->start(100);
}

Intro::~Intro(){

}

void Intro::opPort(){
    serial->setPortName("ttyUSB0");
    open = serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);

    qDebug() << "opPort()- Port is open: " << open << endl;
}

void Intro::wrToPort(QByteArray data){

    if(serial->isOpen()){
        int i = serial->write(data);
        QString str(data);
        qDebug() << "wrToPort()- Serial data : " << i << " : " <<str << endl;
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
            initValues();
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

void Intro::testSendSlot(){
   // testSend();

    if(i == 4) i=1;

    switch (i) {
    case 1:
        qDebug() << 1;
        wrToPort(*ack);
        break;
    case 2:
        qDebug() << 2;
        initValues();
        wrToPort(*currentValues);
        break;
    case 3:
        qDebug() << 3;
        wrToPort(*commOff);
        break;
    case 4:
        qDebug() << 4;
         wrToPort(*questionMarks);
        break;
    default:
        break;
    }
    i++;
}

void Intro::initValues(){

    if(dryTemp < 150 ){
        dryTemp += 1.03;
    }

    if(humidity < 95){
        humidity += 1.8;
    }
    wetTemp = dryTemp - 1.1;

    QString ss = "0A+" + QString("%1").arg(dryTemp, 6, 'f', 2, '0') + " +"
            + QString("%1").arg(wetTemp, 6, 'f', 2, '0') + " "
            + QString("%1").arg(humidity, 4, 'f', 1, '0');
    qDebug() <<"init value: " << ss<< endl;
    QByteArray baStr = ss.toLatin1();
    //baStr.prepend(0x02);
    baStr.append(0x0d);
    *currentValues = baStr;
}

void Intro::testSend(){
   // QThread *thread = this->thread();
    //forever{
        initValues();
        this->wrToPort(*this->ack);
        //thread->msleep(1000);
        this->wrToPort(*currentValues);
        //thread->msleep(1000);
        this->wrToPort(*commOff);
        //thread->msleep(1000);
        this->wrToPort(*questionMarks);
        //thread->msleep(1000);
    //}
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
