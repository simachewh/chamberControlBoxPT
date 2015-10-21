#ifndef INRO_H
#define INRO_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QThread>

class Intro : public QObject
{
    Q_OBJECT
public:
    enum chCommands {ACK, B = 'B', A = 'A', I = 'I'};
    enum pcCommands {O = 'O', BR, AQ, IY};
    QSerialPort *serial;
    bool open;
    QByteArray *recievedData;
    QTimer *timer;
    int i;

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



    /**
     * @brief wrToPort function.
     * Writes the given data on the serial device.
     * @param data
     */
    void wrToPort(QByteArray data);


    void testSend();

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
    void chCommandState(Intro::chCommands);
    void pcCommandState(Intro::pcCommands);

public slots:
    void readPort();
    void on_newData_arived();
    void testSendSlot();
    void on_pcCommandChanged(Intro::pcCommands);
    void on_chCommandChanged(Intro::chCommands);
    /**
     * @brief initValues function.
     * reinitializes the process values, temperature and humidity.
     */
    void initValues();
};

#endif // INRO_H
