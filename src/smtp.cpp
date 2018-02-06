#include "smtp.h"


#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QTcpServer>
#include <QDebug>

class smtp::smtpClass {
public:
    smtpClass() : socket( new QTcpSocket ) {

    }
    ~smtpClass() {
        if( socket )
            delete socket;
        if( stream )
            delete stream;
    }

    QTcpSocket* socket;
    QTextStream *stream;

};

smtp::smtp( const QString &from, const QString &to, const QString &subject, const QString &body ) :
    m_d( new smtpClass )
{
    connect( m_d->socket, SIGNAL( readyRead() ),
             this, SLOT( readyRead() ) );
    connect( m_d->socket, SIGNAL( connected() ),
             this, SLOT( connected() ) );
    connect( m_d->socket, SIGNAL(error( QTcpSocket::SocketError )),
             this, SLOT(errorReceived( QTcpSocket::SocketError )));
    connect( m_d->socket, SIGNAL(stateChanged( QTcpSocket::SocketState )),
             this, SLOT(stateChanged( QTcpSocket::SocketState )));
    connect( m_d->socket, SIGNAL(disconnectedFromHost()),
             this, SLOT(disconnected()));

    message = "To: " + to + "\n";
    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");
    message.append( body );
    message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
    message.replace( QString::fromLatin1( "\r\n.\r\n" ),
                     QString::fromLatin1( "\r\n..\r\n" ) );
    this->from = from;
    rcpt = to;
    state = Init;
    m_d->socket->connectToHost( "jimac-inc.net", 588);
    if(m_d->socket->waitForConnected ( 30000 )) {
        qDebug() << "connecting DONE...";
        if(m_d->socket->waitForReadyRead ( 1000 )) {
            qDebug() << "reading DONE...";

        }
        else {
            state = Init;
            readyRead();
        }
    }
    else {
        qDebug() << "connecting ERROR...";
    }
    m_d->stream = new QTextStream( m_d->socket );
}

smtp::~smtp()
{

}

void smtp::stateChanged(QTcpSocket::SocketState socketState)
{
    qDebug() <<"stateChanged " << socketState;
}

void smtp::errorReceived(QTcpSocket::SocketError socketError)
{
    qDebug() << "error " <<socketError;
}

void smtp::disconnected()
{
    qDebug() <<"disconneted";
    qDebug() << "error " << m_d->socket->errorString();
}

void smtp::connected()
{
    qDebug() << "Connected ";
}

void smtp::readyRead()
{

    qDebug() <<"readyRead";
    // smtp is line-oriented

    QString responseLine;
    do
    {
        responseLine = m_d->socket->readLine();
        response += responseLine;
    }
    while ( m_d->socket->canReadLine() && responseLine[3] != ' ' );

    responseLine.truncate( 3 );


    if ( state == Init && responseLine[0] == '2' )
    {
        // banner was okay, let's go on

        *m_d->stream << "HELO there\r\n";
        m_d->stream->flush();

        state = Mail;
    }
    else if ( state == Mail && responseLine[0] == '2' )
    {
        // HELO response was okay (well, it has to be)

        *m_d->stream << "MAIL FROM: " << from << "\r\n";
        m_d->stream->flush();
        state = Rcpt;
    }
    else if ( state == Rcpt && responseLine[0] == '2' )
    {

        *m_d->stream << "RCPT TO: " << rcpt << "\r\n"; //r
        m_d->stream->flush();
        state = Data;
    }
    else if ( state == Data && responseLine[0] == '2' )
    {

        *m_d->stream << "DATA\r\n";
        m_d->stream->flush();
        state = Body;
    }
    else if ( state == Body && responseLine[0] == '3' )
    {

        *m_d->stream << message << "\r\n.\r\n";
        m_d->stream->flush();
        state = Quit;
    }
    else if ( state == Quit && responseLine[0] == '2' )
    {

        *m_d->stream << "QUIT\r\n";
        m_d->stream->flush();
        // here, we just close.
        state = Close;
    }
    else if ( state == Close )
    {
        deleteLater();
        return;
    }
    else
    {
        // something broke.
        qDebug() << "Unexpected reply from smtp server:\n\n" + response ;
        state = Close;
    }
    response = "";
}
