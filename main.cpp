#include "ConnectionHandler.hpp"
#include "GameEngine.hpp"
#include "UserInterface/UserInterface.hpp"
#include "UserInterface/Chat.hpp"
#include "UserInterface/ConnSettings.hpp"
#include "UserInterface/Game.hpp"
#include "UserInterface/GameSettings.hpp"
#include <QApplication>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // Create object of class UserInterface
    UserInterface *user_interface = new UserInterface;
    user_interface->setWindowTitle("Battleship");

    // "Subclasses" of UserInterface
    Chat chat(user_interface);
    ConnSettings conn_settings(user_interface);
    Game game(user_interface);
    GameSettings game_settings(user_interface);

    // Create object of class ConnectionHandler
    ConnectionHandler connection_handler;

    // Create object of class GameEngine
    GameEngine game_engine;

    // All SIGNALs which are sent out from ConnectionHandler
    QObject::connect(&connection_handler, SIGNAL(sendStateToUI(SignalValue)), &conn_settings, SLOT(receiveStateFromCH(SignalValue)));
    QObject::connect(&connection_handler, SIGNAL(sendStateToUI(SignalValue)), &game, SLOT(receiveStateFromCH(SignalValue)));
    QObject::connect(&connection_handler, SIGNAL(sendStateToUI(SignalValue)), &game_settings, SLOT(receiveStateFromCH(SignalValue)));
    QObject::connect(&connection_handler, SIGNAL(sendChatToUI(QString)), &chat, SLOT(receiveChatFromCH(QString)));

    QObject::connect(&connection_handler, SIGNAL(sendGameParamToGE(std::vector<int>)), &game_engine, SLOT(receiveGameParamFromCH(std::vector<int>)));
    QObject::connect(&connection_handler, SIGNAL(sendShotToGE(std::vector<int>)), &game_engine, SLOT(receiveShotFromCH(std::vector<int>)));
    QObject::connect(&connection_handler, SIGNAL(sendStateToGE(SignalValue, std::vector<int>)), &game_engine, SLOT(receiveStateFromCH(SignalValue, std::vector<int>)));
    QObject::connect(&connection_handler, SIGNAL(sendEnemyNameToGE(QString)), &game_engine, SLOT(receiveEnemyNameFromCH(QString)));

    // All SIGNALs which are sent out from UserInterface
    QObject::connect(&conn_settings, SIGNAL(activateServer(quint16)), &connection_handler, SLOT(activateServer(quint16)));
    QObject::connect(&conn_settings, SIGNAL(connectAsClient(QString, quint16)), &connection_handler, SLOT(connectAsClient(QString, quint16)));
    QObject::connect(&conn_settings, SIGNAL(sendStateToCH(SignalValue)), &connection_handler, SLOT(receiveStateFromUI(SignalValue)));
    QObject::connect(&game, SIGNAL(sendStateToCH(SignalValue)), &connection_handler, SLOT(receiveStateFromUI(SignalValue)));
    QObject::connect(&game_settings, SIGNAL(sendStateToCH(SignalValue)), &connection_handler, SLOT(receiveStateFromUI(SignalValue)));
    QObject::connect(&chat, SIGNAL(sendChatToCH(QString)), &connection_handler, SLOT(receiveChatFromUI(QString)));

    QObject::connect(&game, SIGNAL(sendStateToGE(SignalValue, std::vector<int>)), &game_engine, SLOT(receiveStateFromUI(SignalValue, std::vector<int>)));
    QObject::connect(&conn_settings, SIGNAL(sendPlayerNameToGE(QString)), &game_engine, SLOT(receivePlayerNameFromUI(QString)));
    QObject::connect(&game_settings, SIGNAL(sendGameParamToGE(std::vector<int>)), &game_engine, SLOT(receiveGameParamFromUI(std::vector<int>)));
    QObject::connect(&game, SIGNAL(sendShotToGE(std::vector<int>)), &game_engine, SLOT(receiveShotFromUI(std::vector<int>)));
    QObject::connect(&game, SIGNAL(sendShipPosToGE(std::vector<int>)), &game_engine, SLOT(receiveShipPosFromUI(std::vector<int>)));

    // All SIGNALs which are sent out from GameEngine
    QObject::connect(&game_engine, SIGNAL(sendGameParamToCH(std::vector<int>)), &connection_handler, SLOT(receiveGameParamFromGE(std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendShotToCH(std::vector<int>)), &connection_handler, SLOT(receiveShotFromGE(std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendStateToCH(SignalValue, std::vector<int>)), &connection_handler, SLOT(receiveStateFromGE(SignalValue, std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendPlayerNameToCH(QString)), &connection_handler, SLOT(receivePlayerNameFromGE(QString)));

    QObject::connect(&game_engine, SIGNAL(sendGameParamToUI(std::vector<int>)), &game_settings, SLOT(receiveGameParamFromGE(std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendShotToUI(std::vector<int>)), &game, SLOT(receiveShotFromGE(std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendShipPosToUI(std::vector<int>)), &game, SLOT(receiveShipPosFromGE(std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendStateToUI(SignalValue, std::vector<int>)), &game, SLOT(receiveStateFromGE(SignalValue, std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendStateToUI(SignalValue, std::vector<int>)), &game_settings, SLOT(receiveStateFromGE(SignalValue, std::vector<int>)));
    QObject::connect(&game_engine, SIGNAL(sendEnemyNameToUI(QString)), user_interface, SLOT(receiveEnemyNameFromGE(QString)));

    // SIGNALs and SLOTs inside UserInterface
    QObject::connect(&conn_settings, SIGNAL(startGameSettings()), &game_settings, SLOT(startGameSettings()));
    QObject::connect(&game_settings, SIGNAL(startGameTab()), &game, SLOT(startGameTab()));
    QObject::connect(user_interface, SIGNAL(WindowResized()), &game, SLOT(WindowResized()));

    // Show window
    user_interface->show();

    return application.exec();

}
