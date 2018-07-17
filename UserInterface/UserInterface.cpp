#include "UserInterface.hpp"

UserInterface::UserInterface(QWidget *parent) : QWidget(parent) {

    setupUi(this);

    ConstParam const_param;

    m_iAspectRatioX = const_param.m_ciStandardFieldSizeX;
    m_iAspectRatioY = const_param.m_ciStandardFieldSizeY;
    m_sPlayerName = "MyName";
    m_sEnemyName = "EnemysName";
    m_sServerIP = "localhost";
    m_sServerPort = "";
    m_sMyPort = "";
    m_bConnectedAsServer = true;
    m_bIsTabGame = false;
    m_arrGameParameter.at(0) = const_param.m_ciStandardFieldSizeX;
    m_arrGameParameter.at(1) = const_param.m_ciStandardFieldSizeY;
    m_arrGameParameter.at(2) = const_param.m_ciStandardNumberShip_5;
    m_arrGameParameter.at(3) = const_param.m_ciStandardNumberShip_4;
    m_arrGameParameter.at(4) = const_param.m_ciStandardNumberShip_3;
    m_arrGameParameter.at(5) = const_param.m_ciStandardNumberShip_2;

}

// Called when the window size changes
void UserInterface::resizeEvent(QResizeEvent *) {

    emit WindowResized();

}


void UserInterface::receiveEnemyNameFromGE(QString) {



}

