#ifndef CONNECTIONHANLDER_HPP
#define CONNECTIONHANLDER_HPP

#include <QObject>
#include <string>
#include <vector>
#include <QAbstractSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include "SignalValue.hpp"
#include <cinttypes>

enum class TcpCommand : char {

    NegotiateGameParameters = 0x01,
    RequirementGameStart    = 0x02,
    Shot                    = 0x03,
    AnswerRequest           = 0x10,
    AnswerShot              = 0x11,
    ChatMessage             = 0x21,
    Default

};

class ConnectionHandler : public QObject {

    Q_OBJECT

    public:
        ConnectionHandler();
    public: signals:
        void sendStateToUI(SignalValue iState);
        void sendChatToUI(QString sMessage);
        void sendGameParamToGE(std::vector<int> arrParam);
        void sendShotToGE(std::vector<int> arrPos);
        void sendStateToGE(SignalValue iState, std::vector<int> arrInfo = {0});
        void sendEnemyNameToGE(QString sName);
    private: signals:
        void connectedAsServer();
    public slots:
        void activateServer(quint16 iPort);
        void connectAsClient(QString iIP, quint16 iPort);
        void receiveStateFromUI(SignalValue iState);
        void receiveChatFromUI(QString sMessage);
        void receiveGameParamFromGE(std::vector<int> arrParam);
        void receiveShotFromGE(std::vector<int> arrPos);
        void receiveStateFromGE(SignalValue iState, std::vector<int> arrInfo = {0});
        void receivePlayerNameFromGE(QString sName);
    private slots:
        void connected();
        void disconnected();
        void errorHandler(QAbstractSocket::SocketError socketError);
        void receiveTcpMessage();
        void incommingConnection();
    private:
        QTcpServer *m_Server;
        QTcpSocket *m_Socket;
        bool m_IsClient;
        bool m_SameGame;
        bool m_HasClient;
        bool m_WasConnectedAsClient;
        void sendTcpMessage(TcpCommand tcpCommand, SignalValue iState = SignalValue::PlaceHolder, std::vector<int> iVector = {});

};

#endif // CONNECTIONHANLDER_HPP
