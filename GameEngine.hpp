#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include <QWidget>
#include <QString>
#include <array>
#include <vector>
#include <map>
#include <iostream>
#include <cstdint>
#include "SignalValue.hpp"
#include "ConstParam.hpp"

class GameEngine : public QObject {

    Q_OBJECT

public slots:
    // Receives the game parameters from the ConnectionHandler
    void receiveGameParamFromCH(std::vector<int>);
    // Receives and checks the shot of the enemy
    void receiveShotFromCH(std::vector<int>);
    // Receives different state messages from the ConnectionHandler
    void receiveStateFromCH(SignalValue, std::vector<int>);
    // Receives the enemy's name from the ConnectionHandler
    void receiveEnemyNameFromCH(QString);

    // Receive the player name from der UserInterface
    void receivePlayerNameFromUI(QString);
    // Receives the game parameters from the UserInterface
    void receiveGameParamFromUI(std::vector<int>);
    // Receives and checks the shot from the UserInterface
    void receiveShotFromUI(std::vector<int>);
    // Receives the ship positions from the UserInterface
    void receiveShipPosFromUI(std::vector<int>);
    // Receives different state messages from the UserInterface
    void receiveStateFromUI(SignalValue, std::vector<int>);
signals:
    void sendGameParamToCH(std::vector<int>);
    void sendShotToCH(std::vector<int>);
    void sendStateToCH(SignalValue, std::vector<int>);
    void sendPlayerNameToCH(QString);

    void sendGameParamToUI(std::vector<int>);
    void sendShotToUI(std::vector<int>);
    void sendShipPosToUI(std::vector<int>);
    void sendStateToUI(SignalValue, std::vector<int>);
    void sendEnemyNameToUI(QString);
private:
    // Check the game parameters
    bool checkGameParameters(std::vector<int>);

    std::vector<std::vector<std::array<int, 2>>> m_MyField;     // Shot, ShipID
    std::vector<std::vector<std::array<int, 2>>> m_EnemyField;  // Shot, Hit
    int m_iFieldX;
    int m_iFieldY;
    int m_iNrBattleships;
    int m_iNrCruisers;
    int m_iNrDestroyers;
    int m_iNrSubmarines;
    int m_iCurrentShipID;
    QString m_sPlayerName;
    QString m_sEnemyName;

};

#endif // GAMEENGINE_HPP

