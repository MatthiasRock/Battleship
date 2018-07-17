#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <QWidget>
#include <vector>
#include <array>
#include "ui_UserInterface.h"
#include "ConstParam.hpp"

class Chat;
class ConnSettings;
class Game;
class GameSettings;
class Field;

class UserInterface : public QWidget, public Ui::UserInterface {

    Q_OBJECT

    public:
        explicit UserInterface(QWidget *parent = 0);
        virtual void resizeEvent(QResizeEvent *event);
        friend class Chat;
        friend class ConnSettings;
        friend class Game;
        friend class GameSettings;
        friend class Field;
    signals:
        void WindowResized();
    public slots:
        void receiveEnemyNameFromGE(QString);
    private:
        int m_iAspectRatioX;
        int m_iAspectRatioY;
        int m_iEnemyGroupNumb;
        QString m_sPlayerName;
        QString m_sEnemyName;
        QString m_sServerIP;
        QString m_sServerPort;
        QString m_sMyPort;
        bool m_bConnectedAsServer;
        bool m_bIsTabGame;
        std::array<int, 6> m_arrGameParameter;  // SizeX, SizeY, NumShip5, NumShip4, NumShip3, NumShip2

};


#endif // USERINTERFACE_HPP
