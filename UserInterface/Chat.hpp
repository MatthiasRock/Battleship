#ifndef CHAT_HPP
#define CHAT_HPP

#include <QWidget>
#include "UserInterface.hpp"

class Chat : public QWidget {

    Q_OBJECT

    public:
        Chat(UserInterface *pui);
    signals:
        // Send the message to the ConnectionHandler
        void sendChatToCH(QString);
    public slots:
        // Receive the message from the ConnectionHandler
        void receiveChatFromCH(QString);
    private slots:
        // Read the entered message
        void sendChatMessage();
    private:
        UserInterface *ui;

};

#endif // CHAT_HPP

