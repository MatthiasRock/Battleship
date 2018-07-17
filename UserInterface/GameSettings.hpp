#ifndef GAMESETTINGS_HPP
#define GAMESETTINGS_HPP

#include <QWidget>
#include "UserInterface.hpp"
#include "SignalValue.hpp"
#include "ConstParam.hpp"

class GameSettings : public QWidget {

    Q_OBJECT

    public:
        GameSettings(UserInterface *pui);
    signals:
        // Send the game parameters to the GameEngine
        void sendGameParamToGE(std::vector<int>);
        // Send different state messages to the ConnectionHandler
        void sendStateToCH(SignalValue);
        // Start the Game tab
        void startGameTab();
    public slots:
        // Receive the game parameters from the enemy
        void receiveGameParamFromGE(std::vector<int>);
        // Receive different state messages from the ConnectionHandler
        void receiveStateFromCH(SignalValue);
        // Receive different state messages from the GameEngine
        void receiveStateFromGE(SignalValue, std::vector<int>);
        // Initialize the tab "Game Settings"
        void startGameSettings();
    private slots:
        // When button "Accept parameters of the enemy" is pressed
        void ButtonAcceptParam();
        // When button "Reject parameters of the enemy" is pressed
        void ButtonRejectParam();
        // Send the game parameters to the enemy in case we are the server
        void ButtonSendParToEnemy();
    private:
        UserInterface *ui;

};

#endif // GAMESETTINGS_HPP

