#include "GameEngine.hpp"

// Receives the game parameters from the ConnectionHandler
void GameEngine::receiveGameParamFromCH(std::vector<int> aParam) {

    bool valid;

    // Check the game parameters
    valid = checkGameParameters(aParam);

    // If the parameters are valid
    if(valid) emit sendGameParamToUI(aParam);

    // If the parameters are not valid
    else {

        emit sendStateToCH(SignalValue::NotSupported, {});
        emit sendStateToUI(SignalValue::NotSupported, {});

    }

}

// Receives and checks the shot of the enemy
void GameEngine::receiveShotFromCH(std::vector<int> aPos) {

    int iShipID   = m_MyField[aPos[0]][aPos[1]][1];
    bool bSunk    = true;
    bool bSunkAll = true;
    std::vector<int> iCoord;

    emit sendShotToUI(aPos);

    // If there was already shot
    if(m_MyField[aPos[0]][aPos[1]][0] == 1) emit sendStateToUI(SignalValue::AlreadyShot, {});

    m_MyField[aPos[0]][aPos[1]][0] = 1; // Shot

    // If there is no ship at this position
    if(iShipID == 0) {

        emit sendStateToCH(SignalValue::NotHit, {});
        return;

    }

    // Check for all fields
    for(int i = 0; i < m_iFieldX; i++) {

        for(int j = 0; j < m_iFieldY; j++) {

            // If there is any ship which did not sink yet
            if(m_MyField[i][j][1] > 0 && m_MyField[i][j][0] == 0) bSunkAll = false;

            // If this field has the same ShipID
            if(m_MyField[i][j][1] == iShipID) {

                // If this ship did not sink
                if(m_MyField[i][j][0] == 0) bSunk = false;

                iCoord.push_back(i);
                iCoord.push_back(j);

            }

        }

    }

    // If all ships did sink
    if(bSunkAll) {

        emit sendStateToCH(SignalValue::SunkGameEnd, iCoord);
        emit sendStateToUI(SignalValue::SunkGameEnd, {});

    }
    // If this ship did sink
    else if(bSunk) emit sendStateToCH(SignalValue::Sunk, iCoord);

    // If this was a hit
    else emit sendStateToCH(SignalValue::Hit, {});

}

// Receives different state messages from the ConnectionHandler
void GameEngine::receiveStateFromCH(SignalValue iState, std::vector<int> aInfo) {

    switch(iState) {

        case SignalValue::Hit:
            emit sendStateToUI(SignalValue::Hit, {});
            break;
        case SignalValue::NotHit:
            emit sendStateToUI(SignalValue::NotHit, {});
            break;
        case SignalValue::Sunk:
            emit sendStateToUI(SignalValue::Hit, {});
            emit sendShipPosToUI(aInfo);
            break;
        case SignalValue::SunkGameEnd:
            emit sendStateToUI(SignalValue::Hit, {});
            emit sendShipPosToUI(aInfo);
            emit sendStateToUI(SignalValue::GameEnd, {});
            break;
        default:
            break;

    }

}

// Receives the enemy's name from the ConnectionHandler
void GameEngine::receiveEnemyNameFromCH(QString sName) {

    m_sEnemyName = sName;

}

// Receive the player name from der UserInterface
void GameEngine::receivePlayerNameFromUI(QString sName) {

    m_sPlayerName = sName;

}

// Receives the game parameters from the UserInterface
void GameEngine::receiveGameParamFromUI(std::vector<int> aParam) {

    bool valid;

    // Check the game parameters
    valid = checkGameParameters(aParam);

    // If the parameters are valid
    if(valid) emit sendGameParamToCH(aParam);

    // If the parameters are not valid
    else emit sendStateToUI(SignalValue::NotSupported, {});

}

// Receives and checks the shot from the UserInterface
void GameEngine::receiveShotFromUI(std::vector<int> aPos) {

    // If there was already shot
    if(m_EnemyField[aPos[0]][aPos[1]][0] == 1) {

        emit sendStateToUI(SignalValue::AlreadyShot, {});
        return;

    }

    m_EnemyField[aPos[0]][aPos[1]][0] = 1;
    emit sendShotToCH(aPos);

}

// Receives the ship positions from the UserInterface
void GameEngine::receiveShipPosFromUI(std::vector<int> aPos) {

    // Place the ship on the field
    for(uint i = 0; i < aPos.size(); i += 2) m_MyField[aPos[i]][aPos[i+1]][1] = m_iCurrentShipID;

    m_iCurrentShipID++;

}

// Receives different state messages from the UserInterface
void GameEngine::receiveStateFromUI(SignalValue/*iState*/, std::vector<int>/*aInfo*/) {



}

// Check the game parameters
bool GameEngine::checkGameParameters(std::vector<int> aParam) {

    bool valid = true;

    m_iFieldX        = aParam.at(0);
    m_iFieldY        = aParam.at(1);
    m_iNrBattleships = aParam.at(2);
    m_iNrCruisers    = aParam.at(3);
    m_iNrDestroyers  = aParam.at(4);
    m_iNrSubmarines  = aParam.at(5);

    ConstParam const_param;
    int iNumberOfFields, iNumberOfFieldsNeeded;

    iNumberOfFields = m_iFieldX * m_iFieldY;
    iNumberOfFieldsNeeded = m_iNrBattleships*5 + m_iNrCruisers*4 + m_iNrDestroyers*3 + m_iNrSubmarines*2;

    // If there are too many ships
    if(iNumberOfFieldsNeeded > iNumberOfFields) valid = false;

    // If the parameters are invalid
    if(m_iFieldX        < const_param.m_ciMinFieldSizeX     || m_iFieldX        > const_param.m_ciMaxFieldSizeX   ||
       m_iFieldY        < const_param.m_ciMinFieldSizeY     || m_iFieldY        > const_param.m_ciMaxFieldSizeY   ||
       m_iNrBattleships < const_param.m_ciMinNumberShip_5   || m_iNrBattleships > const_param.m_ciMaxNumberShip_5 ||
       m_iNrCruisers    < const_param.m_ciMinNumberShip_4   || m_iNrCruisers    > const_param.m_ciMaxNumberShip_4 ||
       m_iNrDestroyers  < const_param.m_ciMinNumberShip_3   || m_iNrDestroyers  > const_param.m_ciMaxNumberShip_3 ||
       m_iNrSubmarines  < const_param.m_ciMinNumberShip_2   || m_iNrSubmarines  > const_param.m_ciMaxNumberShip_2) {

        valid = false;

    }

    // Resize MyField
    m_MyField.resize(m_iFieldX);
    for(int i = 0; i < m_iFieldX; i++) m_MyField.at(i).resize(m_iFieldY);

    // Resize EnemyField
    m_EnemyField.resize(m_iFieldX);
    for(int i = 0; i < m_iFieldX; i++) m_EnemyField.at(i).resize(m_iFieldY);

    // Set all elements to 0
    for(int i = 0; i < m_iFieldX; i++) {

        for(int j = 0; j < m_iFieldY; j++) {

            m_MyField[i][j][0]    = 0;  // Shot
            m_MyField[i][j][1]    = 0;  // ShipID
            m_EnemyField[i][j][0] = 0;  // Shot
            m_EnemyField[i][j][1] = 0;  // Hit

        }

    }
    m_iCurrentShipID = 1;

    return valid;

}
