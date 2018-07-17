#include "ConnSettings.hpp"

// Constructor
ConnSettings::ConnSettings(UserInterface *pui) : QWidget(0), ui(pui),
    m_arTextButtonConAsClient{"Connect with Enemy",
                              "Cancel connection",
                              "Disconnect"},
    m_arTextButtonConAsServer{"To be challenged by an Enemy",
                              "Close port",
                              "Disconnect"}
{

    ui->tabWidget->setCurrentIndex(0);  // Select tab

    ui->lineEdit_PlayerName->setText(ui->m_sPlayerName);
    ui->lineEdit_IPServer->setText(ui->m_sServerIP);
    ui->lineEdit_PortServer->setText(ui->m_sServerPort);
    ui->lineEdit_MyServerPort->setText(ui->m_sMyPort);

    connect(ui->pushButton_ConnectToServer, SIGNAL(clicked()),       this, SLOT(ButtonConnectAsClient()));
    connect(ui->lineEdit_PortServer,        SIGNAL(returnPressed()), this, SLOT(ButtonConnectAsClient()));
    connect(ui->pushButton_ActivateServer,  SIGNAL(clicked()),       this, SLOT(ButtonConnectAsServer()));
    connect(ui->lineEdit_MyServerPort,      SIGNAL(returnPressed()), this, SLOT(ButtonConnectAsServer()));

}

// Receive different states from ConnectionHandler
void ConnSettings::receiveStateFromCH(SignalValue State) {

    switch(State) {

        case SignalValue::Connected:
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>Connected with enemy!</span><br>");

            // If connected as server
            if(ui->m_bConnectedAsServer) ui->pushButton_ActivateServer->setText(QString::fromStdString(m_arTextButtonConAsClient.at(2)));

            // If connected as client
            else ui->pushButton_ConnectToServer->setText(QString::fromStdString(m_arTextButtonConAsServer.at(2)));

            emit startGameSettings();   // Start tab "Game Settings"
            break;

        case SignalValue::ConnectionImpossible:
        case SignalValue::Disconnected:

            // (De)activate fields
            ui->lineEdit_Chat->setEnabled(false);
            ui->lineEdit_PlayerName->setEnabled(true);
            ui->lineEdit_MyServerPort->setEnabled(true);
            ui->pushButton_ActivateServer->setEnabled(true);
            ui->lineEdit_IPServer->setEnabled(true);
            ui->lineEdit_PortServer->setEnabled(true);
            ui->tab_Game->setEnabled(false);
            ui->tab_GameSettings->setEnabled(false);

            ui->tabWidget->setCurrentIndex(0);

            // Change text of buttons
            ui->pushButton_ConnectToServer->setText(QString::fromStdString(m_arTextButtonConAsClient.at(0)));
            ui->pushButton_ActivateServer->setText(QString::fromStdString(m_arTextButtonConAsServer.at(0)));

            if(State == SignalValue::ConnectionImpossible) ui->textBrowser_Chat->append("<span style='color:#04B404;'>Connection to the enemy could not be established!</span><br>");
            else ui->textBrowser_Chat->append("<span style='color:#04B404;'>Connection to the enemy was interrupted!</span><br>");

            break;
        default:
            break;

    }

}

// When button "Connect with Enemy" is pressed
void ConnSettings::ButtonConnectAsClient() {

    // If button shows "Connect with Enemy"
    if(ui->pushButton_ConnectToServer->text().toStdString() == m_arTextButtonConAsClient.at(0)) {

        QString sName, sIP, sPort;

        sName   = CheckPlayerName();                            // Check, if a name was entered
        sIP     = CheckServerIP();                              // Check, if an IP was entered
        sPort   = CheckPort(ui->lineEdit_PortServer->text());   // Check, if a valid Port number was entered

        // If not all inputs are valid
        if(sName == "" || sIP == "" || sPort == "") return;

        // Deactivate fields
        ui->lineEdit_PlayerName->setEnabled(false);
        ui->lineEdit_MyServerPort->setEnabled(false);
        ui->pushButton_ActivateServer->setEnabled(false);
        ui->lineEdit_IPServer->setEnabled(false);
        ui->lineEdit_PortServer->setEnabled(false);

        // Change text of button
        ui->pushButton_ConnectToServer->setText(QString::fromStdString(m_arTextButtonConAsClient.at(1)));

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Connect...</span><br>");

        ui->m_bConnectedAsServer = false;
        ui->m_sPlayerName        = sName;
        ui->m_sServerIP          = sIP;
        ui->m_sServerPort        = sPort;

        ui->label_MyField->setText(sName);

        emit sendPlayerNameToGE(sName);
        emit connectAsClient(sIP, static_cast<quint16>(sPort.toInt()));

    }
    // If button shows "Cancel connection" OR "Disconnect"
    else {

        if(ui->pushButton_ConnectToServer->text().toStdString() == m_arTextButtonConAsClient.at(1)) {

            ui->textBrowser_Chat->append("<span style='color:#04B404;'>Connection was canceled!</span><br>");

        }
        else ui->textBrowser_Chat->append("<span style='color:#04B404;'>Connection to the enemy was disconnected!</span><br>");

        ui->lineEdit_Chat->setEnabled(false);

        // (De)activate fields
        ui->lineEdit_PlayerName->setEnabled(true);
        ui->lineEdit_MyServerPort->setEnabled(true);
        ui->pushButton_ActivateServer->setEnabled(true);
        ui->lineEdit_IPServer->setEnabled(true);
        ui->lineEdit_PortServer->setEnabled(true);
        ui->tab_Game->setEnabled(false);
        ui->tab_GameSettings->setEnabled(false);

        // Change text of button
        ui->pushButton_ConnectToServer->setText(QString::fromStdString(m_arTextButtonConAsClient.at(0)));

        emit sendStateToCH(SignalValue::Disconnect);

    }

}

// When button "To be challenged by an Enemy" is pressed
void ConnSettings::ButtonConnectAsServer() {

    // If button shows "To be challenged by an Enemy"
    if(ui->pushButton_ActivateServer->text().toStdString() == m_arTextButtonConAsServer.at(0)) {

        QString sName, sPort;

        sName   = CheckPlayerName();                            // Check, if a name was entered
        sPort   = CheckPort(ui->lineEdit_MyServerPort->text()); // Check, if a valid Port number was entered

        // If not all inputs are valid
        if(sName == "" || sPort == "") return;

        // Deactivate fields
        ui->lineEdit_PlayerName->setEnabled(false);
        ui->lineEdit_MyServerPort->setEnabled(false);
        ui->pushButton_ConnectToServer->setEnabled(false);
        ui->lineEdit_IPServer->setEnabled(false);
        ui->lineEdit_PortServer->setEnabled(false);

        // Change text of button
        ui->pushButton_ActivateServer->setText(QString::fromStdString(m_arTextButtonConAsServer.at(1)));

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Wait until the enemy joins...</span><br>");

        ui->m_bConnectedAsServer = true;
        ui->m_sPlayerName        = sName;
        ui->m_sMyPort            = ui->lineEdit_MyServerPort->text();

        emit sendPlayerNameToGE(sName);
        emit activateServer(static_cast<quint16>(sPort.toInt()));

    }
    // If button shows "Cancel connection" OR "Close port"
    else {

        if(ui->pushButton_ActivateServer->text().toStdString() == m_arTextButtonConAsServer.at(1)) {

            ui->textBrowser_Chat->append("<span style='color:#04B404;'>Server has stopped!</span><br>");

        }
        else ui->textBrowser_Chat->append("<span style='color:#04B404;'>Connection to the enemy was disconnected!</span><br>");

        // (De)activate fields
        ui->lineEdit_Chat->setEnabled(false);
        ui->lineEdit_PlayerName->setEnabled(true);
        ui->lineEdit_MyServerPort->setEnabled(true);
        ui->pushButton_ConnectToServer->setEnabled(true);
        ui->lineEdit_IPServer->setEnabled(true);
        ui->lineEdit_PortServer->setEnabled(true);
        ui->tab_Game->setEnabled(false);
        ui->tab_GameSettings->setEnabled(false);

        // Change text of button
        ui->pushButton_ActivateServer->setText(QString::fromStdString(m_arTextButtonConAsServer.at(0)));

        emit sendStateToCH(SignalValue::Disconnect);

    }

}

// Check, if a name was entered
QString ConnSettings::CheckPlayerName() {

    QString sName = ui->lineEdit_PlayerName->text();

    if(sName.length() > 0) ui->m_sPlayerName = sName;
    else {

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Please enter your name!</span><br>");
        sName = "";

    }
    return sName;

}

// Check, if an IP was entered
QString ConnSettings::CheckServerIP() {

    QString sIP = ui->lineEdit_IPServer->text();

    if(sIP.length() > 0) ui->m_sServerIP = sIP;
    else {

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Please enter the IP of the server!</span><br>");
        sIP = "";

    }
    return sIP;

}

// Check, if a valid Port number was entered
QString ConnSettings::CheckPort(QString sPort) {

    bool isint;
    int iPort;

    // If a port number was entered
    if(sPort.length() > 0) {

        iPort = sPort.toInt(&isint);

        // If the entered characters are valid
        if(isint) {

            if(iPort <= 65535) {

                if(sPort == ui->lineEdit_PortServer->text()) ui->m_sServerPort = sPort;
                else if(sPort == ui->lineEdit_MyServerPort->text()) ui->m_sMyPort = sPort;

            }
            else {

                ui->textBrowser_Chat->append("<span style='color:#04B404;'>The port number must be less than or equal to 65535!</span><br>");
                sPort = "";

            }

        }
        // If there were entered invalid characters
        else {

            ui->textBrowser_Chat->append("<span style='color:#04B404;'>Only numbers are allowed in the field 'Port'!</span><br>");
            sPort = "";

        }

    }
    // If no port number was entered
    else {

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Please enter the port number of the server!</span><br>");
        sPort = "";

    }
    return sPort;

}
