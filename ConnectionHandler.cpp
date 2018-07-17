#include "ConnectionHandler.hpp"

// Constructor
ConnectionHandler::ConnectionHandler() : QObject() {

    m_Server    = new QTcpServer; //TCP_Server;
    m_Socket    = new QTcpSocket; //TCP_Client;
    m_IsClient  = false;
    m_SameGame  = true;
    m_HasClient = false;

    connect(this,     SIGNAL(connectedAsServer()), this, SLOT(connected()));
    connect(m_Server, SIGNAL(newConnection()),     this, SLOT(incommingConnection()));
    connect(m_Socket, SIGNAL(connected()),         this, SLOT(connected()));
    connect(m_Socket, SIGNAL(disconnected()),      this, SLOT(disconnected()));
    connect(m_Socket, SIGNAL(disconnected()),      m_Socket, SLOT(deleteLater()));
    connect(m_Socket, SIGNAL(readyRead()),         this, SLOT(receiveTcpMessage()));
    connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorHandler(QAbstractSocket::SocketError)));

}

// Activate server
void ConnectionHandler::activateServer(quint16 iPort) {

    // If the server could not be started
    if(!m_Server->listen(QHostAddress::Any, iPort)) emit sendChatToUI("# Server - could not be started!");

    // If the server started
    else {

        QString ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

        emit sendChatToUI("# Server - started :)");
        emit sendChatToUI(QString("# Server - Address: %1 - Port: %2").arg(ipAddress).arg(m_Server->serverPort()));

   }

}

// Connect as client
void ConnectionHandler::connectAsClient(QString iIP, quint16 iPort) {

    m_IsClient = true;
    m_Socket->connectToHost(iIP, iPort);

}

// Receives different state messages from the UserInterface
void ConnectionHandler::receiveStateFromUI(SignalValue iState){

    switch(iState) {

        case SignalValue::StartGame:
            sendTcpMessage(TcpCommand::RequirementGameStart);
            break;
        case SignalValue::Disconnect:
        case SignalValue::GameEnd:
            if(m_IsClient) {
                m_Socket->disconnectFromHost();
                m_IsClient = false;
            }
            else {
                m_Server->close();
                m_HasClient = false;
            }
            break;
        case SignalValue::OK:
            sendTcpMessage(TcpCommand::AnswerRequest, SignalValue::OK);
            break;
        case SignalValue::RejectedNotReady:
            sendTcpMessage(TcpCommand::AnswerRequest, SignalValue::RejectedNotReady);
            break;
        case SignalValue::ErrorUnknown:
            sendTcpMessage(TcpCommand::AnswerRequest, SignalValue::ErrorUnknown);
            sendTcpMessage(TcpCommand::AnswerShot,    SignalValue::ErrorUnknown);
            break;
        default:
            break;

    }

}

// Receive message from user interface
void ConnectionHandler::receiveChatFromUI(QString sMessage) {

    QByteArray qByteArrayToSend;

    qByteArrayToSend.push_back(static_cast<char>(TcpCommand::ChatMessage));
    qByteArrayToSend.push_back(static_cast<char>(sMessage.size()));

    // Add single chars of the messge
    for(char cIt = 0; cIt < sMessage.size(); cIt++) qByteArrayToSend.push_back(sMessage.at(cIt).toLatin1());

    m_Socket->write(qByteArrayToSend);

}

// Receive game parameters from GameEngine
void ConnectionHandler::receiveGameParamFromGE(std::vector<int> arrParam) {

    sendTcpMessage(TcpCommand::NegotiateGameParameters, SignalValue::PlaceHolder, arrParam);

}

// Receive Shot from GameEngine
void ConnectionHandler::receiveShotFromGE(std::vector<int> arrPos){

    sendTcpMessage(TcpCommand::Shot, SignalValue::PlaceHolder, arrPos);

}

// Receives different state messages from the GameEngine
void ConnectionHandler::receiveStateFromGE(SignalValue iState, std::vector<int> arrInfo){

    switch(iState){
      case SignalValue::NotSupported:
            sendTcpMessage(TcpCommand::AnswerRequest, SignalValue::NotSupported);
            break;
      case SignalValue::Hit:
            sendTcpMessage(TcpCommand::AnswerShot, SignalValue::Hit);
            break;
      case SignalValue::NotHit:
            sendTcpMessage(TcpCommand::AnswerShot, SignalValue::NotHit);
            break;
      case SignalValue::Sunk:
            sendTcpMessage(TcpCommand::AnswerShot, SignalValue::Sunk, arrInfo);
            break;
      case SignalValue::SunkGameEnd:
            sendTcpMessage(TcpCommand::AnswerShot, SignalValue::SunkGameEnd, arrInfo);
            break;
      default:
            break;

    }

}

// Receive player name from GameEngine
void ConnectionHandler::receivePlayerNameFromGE(QString /*sName*/){



}

// If connected
void ConnectionHandler::connected() {

    emit sendStateToUI(SignalValue::Connected);

}

// If disconnected
void ConnectionHandler::disconnected() {

    emit sendStateToUI(SignalValue::Disconnected);
    emit sendChatToUI("Connection closed!");

}

// Incomming connection
void ConnectionHandler::incommingConnection() {

    // If no connection exists
    if(!m_HasClient) {

        m_HasClient = true;

        delete m_Socket;
        m_Socket = m_Server->nextPendingConnection();

        connect(m_Socket, SIGNAL(connected()),        this, SLOT(connected()));
        connect(m_Socket, SIGNAL(disconnected()),     this, SLOT(disconnected()));
        connect(m_Socket, SIGNAL(disconnected()),     m_Socket, SLOT(deleteLater()));
        connect(m_Socket, SIGNAL(readyRead()),        this, SLOT(receiveTcpMessage()));
        connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorHandler(QAbstractSocket::SocketError)));

        emit sendStateToUI(SignalValue::Connected);
        emit connectedAsServer();
        emit sendChatToUI("# Server - Incoming connection accepted.");

    }
    // If a connection already exists
    else emit sendChatToUI("# Server - Incoming connection blocked because there is already a connection.");

}

// Error handler
void ConnectionHandler::errorHandler(QAbstractSocket::SocketError socketError) {

    switch(socketError) {

        case QAbstractSocket::HostNotFoundError:
            emit sendStateToUI(SignalValue::ConnectionImpossible);
            break;
        case QAbstractSocket::NetworkError:
            emit sendStateToUI(SignalValue::Disconnected);
            emit sendChatToUI("Connection closed!");
        default:
            if(m_IsClient) {

                m_Socket->disconnectFromHost();
                m_IsClient = false;

            }
            else {
                m_Server->close();
                m_HasClient = false;
            }
            emit sendStateToUI(SignalValue::Disconnected);
            emit sendChatToUI("Connection closed!");
            break;

    }

}

// Send TCP message
void ConnectionHandler::sendTcpMessage(TcpCommand tcpCommand, SignalValue iState, std::vector<int> iVector) {

    QByteArray qByteArrayToSend;

    switch(tcpCommand) {

        case TcpCommand::NegotiateGameParameters:
            qByteArrayToSend.push_back(static_cast<char>(TcpCommand::NegotiateGameParameters)); // Command
            qByteArrayToSend.push_back(static_cast<char>(iVector.size()));                      // Length

            // Add field size, number ships
            for(int iVectorValue : iVector) qByteArrayToSend.push_back(static_cast<char>(iVectorValue));

            break;
        case TcpCommand::RequirementGameStart:
            qByteArrayToSend.push_back(static_cast<char>(TcpCommand::RequirementGameStart));    // Command
            qByteArrayToSend.push_back(static_cast<char>(0x00));                                // Length
            break;
        case TcpCommand::Shot:
            qByteArrayToSend.push_back(static_cast<char>(TcpCommand::Shot));                    // Command
            qByteArrayToSend.push_back(static_cast<char>(0x02));                                // Length

            // X/Y coordinate
            for(int iVectorValue : iVector) qByteArrayToSend.push_back(static_cast<char>(iVectorValue));

            break;
        case TcpCommand::AnswerRequest:
            qByteArrayToSend.push_back(static_cast<char>(TcpCommand::AnswerRequest));           // Command
            qByteArrayToSend.push_back(static_cast<char>(0x01));                                // Length

            // Add state code
            switch(iState) {

                case SignalValue::OK:
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::OK));
                    break;
                case SignalValue::RejectedNotReady:
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::RejectedNotReady));
                    break;
                case SignalValue::NotSupported:
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::NotSupported));
                    break;
                case SignalValue::MessageIncomplete:
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::MessageIncomplete));
                    break;
                case SignalValue::TimeOutOnConfirmation:
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::TimeOutOnConfirmation));
                    break;
                case SignalValue::ErrorUnknown:
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::ErrorUnknown));
                    break;
                default:
                break;

            }
            break;
        case TcpCommand::AnswerShot:
            qByteArrayToSend.push_back(static_cast<char>(TcpCommand::AnswerShot));              // Command

            switch(iState) {

                case SignalValue::NotHit:
                    qByteArrayToSend.push_back(static_cast<char>(0x01));                        // Length
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::NotHit));         // State code
                    break;
                case SignalValue::Hit:
                    qByteArrayToSend.push_back(static_cast<char>(0x01));                        // Length
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::Hit));            // State code
                    break;
                case SignalValue::Sunk:
                    qByteArrayToSend.push_back(static_cast<char>(1 + iVector.size()));          // Length
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::Sunk));           // State code

                    // Add X/Y coordinates
                    for(int iVectorValue : iVector) qByteArrayToSend.push_back(static_cast<char>(iVectorValue));

                    break;
                case SignalValue::SunkGameEnd:
                    qByteArrayToSend.push_back(static_cast<char>(1 + iVector.size()));          // Length
                    qByteArrayToSend.push_back(static_cast<char>(SignalValue::SunkGameEnd));    // State code

                    // Add X/Y coordinates
                    for(int iVectorValue : iVector) qByteArrayToSend.push_back(static_cast<char>(iVectorValue));

                    break;
                case SignalValue::ErrorUnknown:
                    qByteArrayToSend.push_back(static_cast<char>(0x01));                        // Length
                    qByteArrayToSend.push_back(static_cast<char>(0x20));                        // State code
                    break;
                default:
                    break;

            }
            break;
        default:
            break;

    }
    qint64 writtenData = 0;

    // Write bytes into socket
    do writtenData = m_Socket->write(qByteArrayToSend);
    while(writtenData < qByteArrayToSend.size());

}

// Receive TCP messge
void ConnectionHandler::receiveTcpMessage() {

    QByteArray qByteArrayReceived = m_Socket->read(265); // Read a maximum of 265 bytes
    std::vector<int> iMessageVector;
    char cCommand     = 0;
    char cDataLenght  = 0;
    int iByteArrayPos = 0;

    // If the message is incomplete
    if(qByteArrayReceived.size() < 2 || qByteArrayReceived.size() < (qByteArrayReceived[1] + 2)) {

        sendTcpMessage(TcpCommand::AnswerRequest, SignalValue::MessageIncomplete);
        return;

    }

    do {

        cCommand    = qByteArrayReceived[iByteArrayPos];
        cDataLenght = qByteArrayReceived[++iByteArrayPos];

        // Store bytes
        for(int iIt = ++iByteArrayPos; iIt < (iByteArrayPos+cDataLenght); ++iIt) {

            iMessageVector.push_back(static_cast<int>(qByteArrayReceived[iIt]));

        }
        iByteArrayPos += cDataLenght;

        switch(cCommand) {

            case static_cast<char>(TcpCommand::NegotiateGameParameters):
                emit sendGameParamToGE(iMessageVector);
                break;
            case static_cast<char>(TcpCommand::RequirementGameStart):
                emit sendStateToUI(SignalValue::StartGame);
                break;
            case static_cast<char>(TcpCommand::Shot):
                emit sendShotToGE(iMessageVector);
                break;
            case static_cast<char>(TcpCommand::AnswerRequest):

                switch(iMessageVector[0]) {

                  case static_cast<char>(SignalValue::OK):
                    emit sendStateToUI(SignalValue::OK);
                    break;
                  case static_cast<char>(SignalValue::RejectedNotReady):
                    emit sendStateToUI(SignalValue::RejectedNotReady);
                    break;
                  case static_cast<char>(SignalValue::NotSupported):
                    emit sendStateToUI(SignalValue::NotSupported);
                    break;
                  case static_cast<char>(SignalValue::MessageIncomplete):
                    // TODO
                    break;
                  case static_cast<char>(SignalValue::TimeOutOnConfirmation):
                    // TODO
                    break;
                  case static_cast<char>(SignalValue::ErrorUnknown):
                    // TODO
                    break;
                  default:
                    break;

                }
                break;
            case static_cast<char>(TcpCommand::AnswerShot):

                switch(iMessageVector[0]) {

                  case static_cast<char>(SignalValue::NotHit):
                    emit sendStateToGE(SignalValue::NotHit);
                    break;
                  case static_cast<char>(SignalValue::Hit):
                    emit sendStateToGE(SignalValue::Hit);
                    break;
                  case static_cast<char>(SignalValue::Sunk):
                    iMessageVector.erase(iMessageVector.begin());   // Remove first element
                    emit sendStateToGE(SignalValue::Sunk, iMessageVector);
                    break;
                  case static_cast<char>(SignalValue::SunkGameEnd):
                    iMessageVector.erase(iMessageVector.begin());   // Remove first element
                    emit sendStateToGE(SignalValue::SunkGameEnd, iMessageVector);
                    break;
                  case static_cast<char>(SignalValue::ErrorUnknown):
                    // TODO
                    break;
                  default:
                    break;

                }
                break;
            default:
                if(!m_SameGame) sendTcpMessage(TcpCommand::AnswerRequest, SignalValue::NotSupported);
                else if (cCommand == static_cast<char>(TcpCommand::ChatMessage)){

                    QString qcChatMessage;

                    for(char cIt = 0; cIt < cDataLenght; cIt++) {

                        qcChatMessage.push_back(iMessageVector[cIt]);

                    }
                    emit sendChatToUI(qcChatMessage);

                }
                else sendTcpMessage(TcpCommand::AnswerRequest, SignalValue::NotSupported);

        }

    }
    while(iByteArrayPos < (qByteArrayReceived.size()-1));

}
