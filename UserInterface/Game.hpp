#ifndef GAME_HPP
#define GAME_HPP

#include <QWidget>
#include "UserInterface.hpp"
#include "AvailableShips.hpp"
#include "SignalValue.hpp"
#include "MyField.hpp"
#include "EnemyField.hpp"


class Game : public QWidget {

    Q_OBJECT

    public:
        Game(UserInterface *pui);
    signals:
        // Send the coordinates of the shot to the GameEngine
        void sendShotToGE(std::vector<int>);
        // Send the coordinates of a placed ship to the GameEngine
        void sendShipPosToGE(std::vector<int>);
        // Send different state messages to the ConnectionHandler
        void sendStateToCH(SignalValue);
        // Send different state messages to the GameEngine
        void sendStateToGE(SignalValue, std::vector<int>);
        // Pass on a signal when the window size changes
        void TransWindowResized();
        // Send a signal to MyField when the Button "Start Game" is pressed
        void ToMF_StartGame();
        // Send the coordinates of the enemy's shot to MyField
        void ToMF_PlaceShot(std::vector<int>);
        // Send the coordinates of an enemy's sunk ship to EnemyField
        void ToEF_ShipPos(std::vector<int>);
        // Passes on the info when the last shot was a hit
        void ToEF_LastShotHit();
    public slots:
        // Receive the position of a shot from the enemy
        void receiveShotFromGE(std::vector<int>);
        // Receive the coordinates of a sunk ship of the enemy
        void receiveShipPosFromGE(std::vector<int>);
        // Receive different state messages from the ConnectionHandler
        void receiveStateFromCH(SignalValue);
        // Receive different state messages from the GameEngine
        void receiveStateFromGE(SignalValue, std::vector<int>);
        // Initialize the Game tab
        void startGameTab();
        // When the window size is changed
        void WindowResized();
        // Activate the button "Start Game" (when all ships are placed)
        void FromAS_StartGame();
        // Is called when the button "Start Game" is pressed
        void ButtonBeginGame();
        // Print messages from MyField to the Chat
        void FromMF_StatusText(QString);
        // Passes on the position of a ship from MyField to the GameEngine
        void FromMF_ShipPos(std::vector<int>);
        // Passes on the info about the game start to the ConnectionHandler
        void FromMF_SendGameBeginToCH();
        // Passes on the position of a shot to the ConnectionHandler
        void FromEF_SendShot(std::vector<int>);
    private:
        UserInterface *ui;
        AvailableShips *available_ships;
        MyField *my_field;
        EnemyField *enemy_field;

};

#endif // GAME_HPP

