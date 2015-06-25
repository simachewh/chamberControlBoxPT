#ifndef INRO_H
#define INRO_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>

class Intro : public QObject
{
    Q_OBJECT
public:

    QSerialPort *serial;
    bool open;
    QByteArray *recievedData;
//    QTimer *timer;

    /**
     * @brief opPort function. A function to setup serial port parameters
     * and open a connection.
     */
    void opPort();

    /**
     * @brief setCommOff function. A functoion to initialize the
     * communication off command.
     */
    void setCommOff();

    void setQuestionMarks();


    void initValues();
    void wrToPort(QByteArray data);

    /**
     * @brief Intro constructor.
     * @param parent
     */
    explicit Intro(QObject *parent = 0);
    ~Intro();

private:
    /**
     * @brief wetTemp. A property to hold value for current wet temperature.
     */
    double wetTemp;

    /**
     * @brief dryTemp. A property to hold value for current dry temperature.
     */
    double dryTemp;

    /**
     * @brief humidity. A property to hold value of current humidity.
     */
    double humidity;


    QByteArray *commOff;
    QByteArray *currentValues;
    QByteArray *ack;
    QByteArray *questionMarks;


signals:
    void newData();
public slots:
    void readPort();
    void on_newData_arived();
};

#endif // INRO_H
