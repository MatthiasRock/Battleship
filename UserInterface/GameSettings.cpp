#include "GameSettings.hpp"

// Constructor
GameSettings::GameSettings(UserInterface *pui) : QWidget(0), ui(pui) {

    // Deactivate tab
    ui->tab_GameSettings->setEnabled(false);

    ConstParam const_param;

    // Set possible values of the game parameters
    ui->horizontalSlider_FieldsizeX->setRange(const_param.m_ciMinFieldSizeX, const_param.m_ciMaxFieldSizeX);
    ui->spinBox_FieldsizeX->setRange(const_param.m_ciMinFieldSizeX, const_param.m_ciMaxFieldSizeX);
    ui->horizontalSlider_FieldsizeY->setRange(const_param.m_ciMinFieldSizeY, const_param.m_ciMaxFieldSizeY);
    ui->spinBox_FieldsizeY->setRange(const_param.m_ciMinFieldSizeY, const_param.m_ciMaxFieldSizeY);
    ui->horizontalSlider_NumberShip_2->setRange(const_param.m_ciMinNumberShip_2, const_param.m_ciMaxNumberShip_2);
    ui->spinBox_NumberShip_2->setRange(const_param.m_ciMinNumberShip_2, const_param.m_ciMaxNumberShip_2);
    ui->horizontalSlider_NumberShip_3->setRange(const_param.m_ciMinNumberShip_3, const_param.m_ciMaxNumberShip_3);
    ui->spinBox_NumberShip_3->setRange(const_param.m_ciMinNumberShip_3, const_param.m_ciMaxNumberShip_3);
    ui->horizontalSlider_NumberShip_4->setRange(const_param.m_ciMinNumberShip_4, const_param.m_ciMaxNumberShip_4);
    ui->spinBox_NumberShip_4->setRange(const_param.m_ciMinNumberShip_4, const_param.m_ciMaxNumberShip_4);
    ui->horizontalSlider_NumberShip_5->setRange(const_param.m_ciMinNumberShip_5, const_param.m_ciMaxNumberShip_5);
    ui->spinBox_NumberShip_5->setRange(const_param.m_ciMinNumberShip_5, const_param.m_ciMaxNumberShip_5);

    // Connect sliders to input fields
    connect(ui->horizontalSlider_FieldsizeX, SIGNAL(valueChanged(int)), ui->spinBox_FieldsizeX, SLOT(setValue(int)));
    connect(ui->spinBox_FieldsizeX, SIGNAL(valueChanged(int)), ui->horizontalSlider_FieldsizeX, SLOT(setValue(int)));
    connect(ui->horizontalSlider_FieldsizeY, SIGNAL(valueChanged(int)), ui->spinBox_FieldsizeY, SLOT(setValue(int)));
    connect(ui->spinBox_FieldsizeY, SIGNAL(valueChanged(int)), ui->horizontalSlider_FieldsizeY, SLOT(setValue(int)));
    connect(ui->horizontalSlider_NumberShip_2, SIGNAL(valueChanged(int)), ui->spinBox_NumberShip_2, SLOT(setValue(int)));
    connect(ui->spinBox_NumberShip_2, SIGNAL(valueChanged(int)), ui->horizontalSlider_NumberShip_2, SLOT(setValue(int)));
    connect(ui->horizontalSlider_NumberShip_3, SIGNAL(valueChanged(int)), ui->spinBox_NumberShip_3, SLOT(setValue(int)));
    connect(ui->spinBox_NumberShip_3, SIGNAL(valueChanged(int)), ui->horizontalSlider_NumberShip_3, SLOT(setValue(int)));
    connect(ui->horizontalSlider_NumberShip_4, SIGNAL(valueChanged(int)), ui->spinBox_NumberShip_4, SLOT(setValue(int)));
    connect(ui->spinBox_NumberShip_4, SIGNAL(valueChanged(int)), ui->horizontalSlider_NumberShip_4, SLOT(setValue(int)));
    connect(ui->horizontalSlider_NumberShip_5, SIGNAL(valueChanged(int)), ui->spinBox_NumberShip_5, SLOT(setValue(int)));
    connect(ui->spinBox_NumberShip_5, SIGNAL(valueChanged(int)), ui->horizontalSlider_NumberShip_5, SLOT(setValue(int)));

    // Set default values
    ui->spinBox_FieldsizeX->setValue(const_param.m_ciStandardFieldSizeX);
    ui->spinBox_FieldsizeY->setValue(const_param.m_ciStandardFieldSizeY);
    ui->spinBox_NumberShip_2->setValue(const_param.m_ciStandardNumberShip_2);
    ui->spinBox_NumberShip_3->setValue(const_param.m_ciStandardNumberShip_3);
    ui->spinBox_NumberShip_4->setValue(const_param.m_ciStandardNumberShip_4);
    ui->spinBox_NumberShip_5->setValue(const_param.m_ciStandardNumberShip_5);

    connect(ui->pushButton_AcceptGame, SIGNAL(clicked()), this, SLOT(ButtonAcceptParam()));
    connect(ui->pushButton_RejectGame, SIGNAL(clicked()), this, SLOT(ButtonRejectParam()));
    connect(ui->pushButton_AcceptGameSettings, SIGNAL(clicked()), this, SLOT(ButtonSendParToEnemy()));

}

// Receive the game parameters from the enemy
void GameSettings::receiveGameParamFromGE(std::vector<int> arrParam) {

    // Save parameters
    for(int i = 0; i < 6; i++) ui->m_arrGameParameter.at(i) = arrParam.at(i);

    ui->spinBox_FieldsizeX->setValue(arrParam.at(0));
    ui->spinBox_FieldsizeY->setValue(arrParam.at(1));
    ui->spinBox_NumberShip_5->setValue(arrParam.at(2));
    ui->spinBox_NumberShip_4->setValue(arrParam.at(3));
    ui->spinBox_NumberShip_3->setValue(arrParam.at(4));
    ui->spinBox_NumberShip_2->setValue(arrParam.at(5));
    ui->textBrowser_Chat->append("<span style='color:#04B404;'>The game parameters displayed above were determined by the enemy."
                                 "<br>Do you want to accept or reject this?</span><br>");
    ui->pushButton_AcceptGame->setEnabled(true);
    ui->pushButton_RejectGame->setEnabled(true);

}

// Receive different state messages from the ConnectionHandler
void GameSettings::receiveStateFromCH(SignalValue iState) {

    // Ignore if the Game tab is active
    if(ui->m_bIsTabGame) return;

    switch(iState) {

        case SignalValue::NotSupported:
        case SignalValue::RejectedNotReady:
            ui->groupBox_GameParameter->setEnabled(true);

            if(iState == SignalValue::NotSupported) ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy does not support these game parameters!</span>");
            else ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy has rejected these game parameters!</span>");
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>Please send other game parameters to the enemy</span><br>");
            break;
        case SignalValue::OK:
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy agrees with the game parameters!</span><br>");
            ui->tab_GameSettings->setEnabled(false);

            // Start tab "Game"
            emit startGameTab();
            break;
        default:
            break;

    }

}

// Receive different state messages from the GameEngine
void GameSettings::receiveStateFromGE(SignalValue iState, std::vector<int>) {

    switch(iState) {

        case SignalValue::NotSupported:
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>The game parameters of the enemy are not supported!"
                                             "<br>Wait for new game parameters...</span><br>");
            break;
        default:
            break;

    }

}

// When button "Accept parameters of the enemy" is pressed
void GameSettings::ButtonAcceptParam() {

    ui->textBrowser_Chat->append("<span style='color:#04B404;'>You have accepted the game parameters of the enemy.</span><br>");
    ui->pushButton_AcceptGame->setEnabled(false);
    ui->pushButton_RejectGame->setEnabled(false);
    ui->tab_GameSettings->setEnabled(false);

    emit sendStateToCH(SignalValue::OK);
    emit startGameTab();   // Start tab "Game"

}

// When button "Reject parameters of the enemy" is pressed
void GameSettings::ButtonRejectParam() {

    ui->textBrowser_Chat->append("<span style='color:#04B404;'>You have rejected the game parameters of the enemy."
                                 "<br>Wait for new game parameters of the enemy...</span><br>");
    ui->pushButton_AcceptGame->setEnabled(false);
    ui->pushButton_RejectGame->setEnabled(false);

    emit sendStateToCH(SignalValue::RejectedNotReady);

}

// Initialize the tab "Game Settings"
void GameSettings::startGameSettings() {

    ui->m_bIsTabGame = false;

    ui->tab_GameSettings->setEnabled(true);
    ui->tabWidget->setCurrentIndex(1);  // Go to tab "Game Settings"

    ui->pushButton_AcceptGame->setEnabled(false);
    ui->pushButton_RejectGame->setEnabled(false);

    // If connected as server
    if(ui->m_bConnectedAsServer) {

        ui->groupBox_GameParameter->setEnabled(true);
        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Please set the field parameters and send them to the enemy.</span><br>");

    }
    // If connected as client
    else {

        ui->groupBox_GameParameter->setEnabled(false);
        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Wait until the enemy sends its game parameters...</span><br>");

    }

}

// Send the game parameters to the enemy in case we are the server
void GameSettings::ButtonSendParToEnemy() {

    int iNumberOfFields, iNumberOfFieldsNeeded;

    iNumberOfFields = ui->spinBox_FieldsizeX->value() * ui->spinBox_FieldsizeY->value();
    iNumberOfFieldsNeeded = ui->spinBox_NumberShip_2->value() + ui->spinBox_NumberShip_3->value() +
                            ui->spinBox_NumberShip_4->value() + ui->spinBox_NumberShip_5->value();
    // If too many ships are selected
    if(iNumberOfFieldsNeeded > iNumberOfFields) {

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>You can not place so many ships!"
                                     "<br>Please select fewer ships.</span><br>");

    }
    // If the number of ships is OK
    else {

        ui->groupBox_GameParameter->setEnabled(false);

        std::vector<int> arrParam(6);

        arrParam.at(0) = ui->spinBox_FieldsizeX->value();
        arrParam.at(1) = ui->spinBox_FieldsizeY->value();
        arrParam.at(2) = ui->spinBox_NumberShip_5->value();
        arrParam.at(3) = ui->spinBox_NumberShip_4->value();
        arrParam.at(4) = ui->spinBox_NumberShip_3->value();
        arrParam.at(5) = ui->spinBox_NumberShip_2->value();

        // Save parameters
        for(int i = 0; i < 6; i++) ui->m_arrGameParameter.at(i) = arrParam.at(i);

        emit sendGameParamToGE(arrParam);

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>The game parameters were sent to the enemy."
                                     "<br>Wait for an answer...</span><br>");

    }

}
