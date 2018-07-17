#include "Chat.hpp"

// Constructor
Chat::Chat(UserInterface *pui) : QWidget(0), ui(pui) {

    ui->textBrowser_Chat->append("<span style='color:#04B404;'>Please enter your name and the IP address of your enemy OR a port which you want to open.</span><br>");

    connect(ui->lineEdit_Chat, SIGNAL(returnPressed()), this, SLOT(sendChatMessage()));

}

// Receive the message from the ConnectionHandler
void Chat::receiveChatFromCH(QString sMessage) {

    ui->textBrowser_Chat->append("<span style='color:#B40404;'><u>" + ui->m_sEnemyName + ":</u><br>" + sMessage + "</span><br>");

}


// Read the entered message
void Chat::sendChatMessage() {

    const QString message = ui->lineEdit_Chat->text();

    if(message.length() > 0) {

        ui->textBrowser_Chat->append("<span style='color:#3104B4;'><u>" + ui->m_sPlayerName + ":</u><br>" + message + "</span><br>");
        ui->lineEdit_Chat->setText("");
        emit sendChatToCH(message);

    }

}
