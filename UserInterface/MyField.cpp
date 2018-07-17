#include "MyField.hpp"

// Constructor
MyField::MyField(QWidget *parent, UserInterface *pui) : Field(parent, pui) {

    // Activate drag and drop
    setAcceptDrops(false);
    m_pGraphics_view->setAcceptDrops(true);

    // Draw grid
    InitializeGrid();

}


// Start Game if the ships do not overlap
void MyField::FromG_StartGame() {

    // Check for all fields if there are more than one ship
    for(int iX = 3; iX < m_iSceneWidth; iX += m_iPictureHeight) {

        for(int iY = 3; iY < m_iSceneHeight; iY += m_iPictureHeight) {

            if(m_pGridScene->items(iX, iY, m_iPictureHeight-6, m_iPictureHeight-6, Qt::IntersectsItemShape, Qt::AscendingOrder, QTransform()).count() > 1) {

                emit ToG_StatusText("At least two ships overlap!<br>Please change ship position!");

                return;

            }

        }

    }
    // Send coordinates if the ships do not overlap
    for(uint i = 0; i < m_pGridScene->m_vecShips.size(); i++) {

        QGraphicsPixmapItem *CurrentShip = m_pGridScene->m_vecShips.at(i);

        std::vector<int> vecKoord;

        int iPosX, iPosY, iLength;
        int rotation = static_cast<int>(CurrentShip->rotation())%360;

        iPosX = static_cast<int>(CurrentShip->scenePos().x())/m_iPictureHeight;
        iPosY = static_cast<int>(CurrentShip->scenePos().y())/m_iPictureHeight;
        iLength = CurrentShip->pixmap().width()/CurrentShip->pixmap().height();

        if(rotation == 0) {

            for(int x = 0; x < iLength; x++) {

                vecKoord.push_back(iPosX);
                vecKoord.push_back(iPosY);
                iPosX++;

            }

        }
        else if(rotation == 90) {

            iPosX--;
            for(int x = 0; x < iLength; x++) {

                vecKoord.push_back(iPosX);
                vecKoord.push_back(iPosY);
                iPosY++;

            }

        }
        else if(rotation == 180) {

            iPosX--;
            iPosY--;
            for(int x = 0; x < iLength; x++) {

                vecKoord.push_back(iPosX);
                vecKoord.push_back(iPosY);
                iPosX--;

            }

        }
        else {

            iPosY--;
            for(int x = 0; x < iLength; x++) {

                vecKoord.push_back(iPosX);
                vecKoord.push_back(iPosY);
                iPosY--;

            }

        }
        // Send ship coordinates
        emit ToG_ShipPos(vecKoord);

    }
    // Send beginning of the game
    emit ToG_SendGameBeginToCH();

    // Make sure that the ships can not be changed
    for(uint i = 0; i < m_pGridScene->m_vecShips.size(); i++) {

        m_pGridScene->m_vecShips.at(i)->setFlag(QGraphicsItem::ItemIsMovable, false);

    }

}
