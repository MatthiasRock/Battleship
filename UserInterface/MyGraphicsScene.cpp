#include "MyGraphicsScene.hpp"
#include "Field.hpp"

// Constructor
MyGraphicsScene::MyGraphicsScene(Field *parent) : QGraphicsScene(), m_ClassField(parent) {

}

void MyGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {

    if (event->mimeData()->hasFormat("application/x-dnditemdata")) event->acceptProposedAction();
    else QGraphicsScene::dragEnterEvent(event);

}

void MyGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {

    if (event->mimeData()->hasFormat("application/x-dnditemdata")) event->acceptProposedAction();
    else QGraphicsScene::dragMoveEvent(event);;

}

void MyGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event) {

    if(event->mimeData()->hasFormat("application/x-dnditemdata")) {

            QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);

            QPixmap pixmap;
            QPoint offset;
            int iPictureNumber;
            dataStream >> pixmap >> offset >> iPictureNumber;

            m_vecShips.push_back(this->addPixmap(QPixmap(QString::fromStdString(":/Ship_size" + std::to_string(iPictureNumber) + ".png"))));
            m_vecShips.back()->setFlag(QGraphicsItem::ItemIsMovable, true);

            // Align with grid
            int iPosX, iPosY;
            iPosX = event->scenePos().x();
            iPosY = event->scenePos().y();

            m_vecShips.back()->setPos(iPosX, iPosY);

            // Align the ship with the grid
            AlignShip(m_vecShips.back());

            event->acceptProposedAction();

            // Decrease the number of available ships of this type by one
            emit ToAS_DecreaseShipNumber(iPictureNumber);

    }
    else event->ignore();

}

// If the mouse button was pressed
void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    // Save coordinates of the last click
    m_ClassField->m_LastClickPosX = event->scenePos().x();
    m_ClassField->m_LastClickPosY = event->scenePos().y();

    QGraphicsPixmapItem *item = qgraphicsitem_cast<QGraphicsPixmapItem*>(itemAt(event->scenePos(), QTransform()));

    if(item == NULL) return;

    if(event->buttons() == Qt::LeftButton) {

        // Put the clicked ship on the top layer
        uint iZIndex = 1;
        for(uint i = 0; i < m_vecShips.size(); i++) {

            m_vecShips.at(i)->setZValue(iZIndex);
            iZIndex++;

        }
        item->setZValue(iZIndex);

        m_currentdraggeditem = item;
        QGraphicsScene::mousePressEvent(event);

    }

    // Rotate the ship by 90° in case of a right-click on the ship
    if(event->buttons() == Qt::RightButton && !m_ClassField->m_bGameStarted) {

        // Rotate by 90°
        int rotation = (static_cast<int>(item->rotation())+90)%360;
        item->setRotation(rotation);

        // Align the ship with the grid
        AlignShip(item);

    }

}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {

    QGraphicsPixmapItem *item = qgraphicsitem_cast<QGraphicsPixmapItem*>(itemAt(event->scenePos(), QTransform()));

    QGraphicsScene::mouseReleaseEvent(event);

    // If the item is the same when releasing the mouse button as when clicking
    if(item && item == m_currentdraggeditem) {

        // Align the ship with the grid
        AlignShip(item);

    }

}

// Align the ship with the grid
void MyGraphicsScene::AlignShip(QGraphicsPixmapItem *item) {

    int iPosX, iPosY;
    int rotation = static_cast<int>(item->rotation())%360;

    iPosX = static_cast<int>(item->scenePos().x());
    iPosY = static_cast<int>(item->scenePos().y());

    iPosX = iPosX - (iPosX%m_ClassField->m_iPictureHeight);
    iPosY = iPosY - (iPosY%m_ClassField->m_iPictureHeight);

    if(rotation == 0) {

        while(iPosX + item->pixmap().width() > m_ClassField->m_iSceneWidth) iPosX -= m_ClassField->m_iPictureHeight;
        while(iPosX < 0) iPosX += m_ClassField->m_iPictureHeight;
        while(iPosY + item->pixmap().height() > m_ClassField->m_iSceneHeight) iPosY -= m_ClassField->m_iPictureHeight;
        while(iPosY < 0) iPosY += m_ClassField->m_iPictureHeight;

    }
    else if(rotation == 90) {

        while(iPosX - item->pixmap().height() < 0) iPosX += m_ClassField->m_iPictureHeight;
        while(iPosX > m_ClassField->m_iSceneWidth) iPosX -= m_ClassField->m_iPictureHeight;
        while(iPosY + item->pixmap().width() > m_ClassField->m_iSceneHeight) iPosY -= m_ClassField->m_iPictureHeight;
        while(iPosY < 0) iPosY += m_ClassField->m_iPictureHeight;

    }
    else if(rotation == 180) {

        while(iPosX - item->pixmap().width() < 0) iPosX += m_ClassField->m_iPictureHeight;
        while(iPosX > m_ClassField->m_iSceneWidth) iPosX -= m_ClassField->m_iPictureHeight;
        while(iPosY - item->pixmap().height() < 0) iPosY += m_ClassField->m_iPictureHeight;
        while(iPosY > m_ClassField->m_iSceneHeight) iPosY -= m_ClassField->m_iPictureHeight;

    }
    else {

        while(iPosX + item->pixmap().height() > m_ClassField->m_iSceneWidth) iPosX -= m_ClassField->m_iPictureHeight;
        while(iPosX < 0) iPosX += m_ClassField->m_iPictureHeight;
        while(iPosY - item->pixmap().width() < 0) iPosY += m_ClassField->m_iPictureHeight;
        while(iPosY > m_ClassField->m_iSceneHeight) iPosY -= m_ClassField->m_iPictureHeight;

    }
    item->setPos(iPosX, iPosY);

}
