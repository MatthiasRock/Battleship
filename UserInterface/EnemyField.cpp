#include "EnemyField.hpp"

// Constructor
EnemyField::EnemyField(QWidget *parent, UserInterface *pui) : Field(parent, pui) {

    m_ShootOK    = false;
    m_iLastShotX = -1;
    m_iLastShotY = -1;

    // Deactivate drag and drop
    setAcceptDrops(false);
    m_pGraphics_view->setAcceptDrops(false);

    // Plot grid
    InitializeGrid();

}

// Send coordinates of the shot and set an X on the position
void EnemyField::mousePressEvent(QMouseEvent *) {

    // If it is not allowed to shoot
    if(!m_ShootOK) return;

    std::vector<int> iShotPos;
    int iPosX, iPosY;

    iPosX = m_LastClickPosX/m_iPictureHeight;
    iPosY = m_LastClickPosY/m_iPictureHeight;

    m_iLastShotX = iPosX;
    m_iLastShotY = iPosY;

    iShotPos.push_back(iPosX);
    iShotPos.push_back(iPosY);

    // Place the shot on the field
    PlaceShot(iShotPos);

    m_ShootOK = false;

    // Reset cursor
    setShotCursor(false);

    // Send position
    emit ToG_SendShot(iShotPos);

}

// Show image of the sunk ship when the coordinates have been sent
void EnemyField::FromG_ShipPos(std::vector<int> arrPos) {

    int iRotation, iMinX, iMinY, iScenePosX, iScenePosY, iLength;

    iMinX = arrPos.at(0);
    iMinY = arrPos.at(1);

    iLength = arrPos.size()/2;

    // Get the rotation of the ship
    if(arrPos.at(0) == arrPos.at(2)) iRotation = 90;
    else iRotation = 0;

    if(iRotation == 0) {

        for(uint i = 0; i < arrPos.size(); i += 2) {

            if(arrPos.at(i) < iMinX) iMinX = arrPos.at(i);

        }
        iScenePosX = iMinX*m_iPictureHeight;
        iScenePosY = arrPos.at(1)*m_iPictureHeight;

    }
    else {

        for(uint i = 1; i < arrPos.size(); i +=2) {

            if(arrPos.at(i) < iMinY) iMinY = arrPos.at(i);

        }
        iScenePosX = (arrPos.at(0) + 1)*m_iPictureHeight;
        iScenePosY = iMinY*m_iPictureHeight;

    }

    QGraphicsPixmapItem *item = m_pGridScene->addPixmap(QPixmap(QString::fromStdString(":/Ship_size" + std::to_string(iLength) + ".png")));

    item->setPos(iScenePosX, iScenePosY);
    item->setRotation(iRotation);

}

// If the last shot was a hit, then color the X green
void EnemyField::FromG_LastShotHit() {

    QGraphicsPixmapItem *item = m_pGridScene->addPixmap(QPixmap(":/ShotHit.png"));
    int XPos, YPos;

    XPos = m_iLastShotX*m_iPictureHeight;
    YPos = m_iLastShotY*m_iPictureHeight;

    item->setPos(XPos, YPos);
    item->setZValue(2000);

}

// Change cursor in field of enemy if shooting is possible
void EnemyField::setShotCursor(bool bValue) {

    if(bValue) {

        int iSize = (this->width()*m_iPictureHeight)/m_iSceneWidth;
        this->setCursor(QCursor(QPixmap(":/Shot.png").scaledToHeight(iSize)));

    }
    else this->setCursor(Qt::ArrowCursor);

}
