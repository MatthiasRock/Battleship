#ifndef MYFIELD_HPP
#define MYFIELD_HPP

#include <array>
#include "Field.hpp"
#include "SignalValue.hpp"

class MyField : public Field {

    Q_OBJECT

    public:
        MyField(QWidget *parent = 0, UserInterface *pui = 0);
    signals:
        // Send status message to Game
        void ToG_StatusText(QString);
        // Send postition of the placed ship to Game
        void ToG_ShipPos(std::vector<int>);
        // When all ships are placed without overlapping
        void ToG_SendGameBeginToCH();
    public slots:
        // Start Game if the ships do not overlap
        void FromG_StartGame();

};

#endif // MYFIELD_HPP

