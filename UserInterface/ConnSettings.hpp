#ifndef CONNSETTINGS_HPP
#define CONNSETTINGS_HPP

#include <QWidget>
#include <array>
#include "UserInterface.hpp"
#include "SignalValue.hpp"
#include "ConstParam.hpp"

class ConnSettings : public QWidget {

    Q_OBJECT

    public:
        ConnSettings(UserInterface *pui);
    signals:
        // Open server port
        void activateServer(quint16);
        // Connect as client
        void connectAsClient(QString, quint16);
        // Send name to GameEngine
        void sendPlayerNameToGE(QString);
        // Send different states to ConnectionHandler
        void sendStateToCH(SignalValue);
        // Send different states to GameEngine
        void startGameSettings();
    public slots:
        // Receive different states from ConnectionHandler
        void receiveStateFromCH(SignalValue);
    private slots:
        // When button "Connect with Enemy" is pressed
        void ButtonConnectAsClient();
        // When button "To be challenged by an Enemy" is pressed
        void ButtonConnectAsServer();
    private:
        // Check, if a name was entered
        QString CheckPlayerName();
        // Check, if an IP was entered
        QString CheckServerIP();
        // Check, if a valid Port number was entered
        QString CheckPort(const QString);

        UserInterface *ui;
        const std::array<std::string, 3> m_arTextButtonConAsClient;
        const std::array<std::string, 3> m_arTextButtonConAsServer;

};

#endif // CONNSETTINGS_HPP

