#ifndef SMTP_H
#define SMTP_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTextStream>

class smtp : public QObject
{
    Q_OBJECT
    class smtpClass;
public:
    smtp( const QString &from, const QString &to, const QString &subject, const QString &body );
    ~smtp();

    enum states {
        Rcpt,
        Mail,
        Data,
        Init,
        Body,
        Quit,
        Close
    };

signals:
    void status( const QString &);

public slots:
    void stateChanged(QTcpSocket::SocketState socketState);
    void errorReceived(QTcpSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();

public:
    QString message;
    QString from;
    QString rcpt;
    QString response;
    int state;

private:
    smtpClass* m_d;
};

#endif // SMTP_H
