#ifndef ENEMYFIELD_HPP
#define ENEMYFIELD_HPP

#include "Field.hpp"

class EnemyField : public Field {

    Q_OBJECT

    public:
        EnemyField(QWidget *parent = 0, UserInterface *pui = 0);
        bool m_ShootOK;
        // Change cursor in field of enemy if shooting is possible
        void setShotCursor(bool);
    signals:
        // Send coordinates of shot to Game
        void ToG_SendShot(std::vector<int>);
    public slots:
        // Show image of the sunk ship when the coordinates have been sent
        void FromG_ShipPos(std::vector<int>);
        // If the last shot was a hit, then color the X green
        void FromG_LastShotHit();
    protected:
        // Send coordinates of the shot and set an X on the position
        virtual void mousePressEvent(QMouseEvent *event);
    private:
        int m_iLastShotX;
        int m_iLastShotY;

};

#endif // ENEMYFIELD_HPP

