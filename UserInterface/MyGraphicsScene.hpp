#ifndef MYGRAPHICSSCENE_HPP
#define MYGRAPHICSSCENE_HPP

#include <QWidget>
#include <QtWidgets>
#include <vector>

class Field;

class MyGraphicsScene : public QGraphicsScene {

    Q_OBJECT

    public:
        MyGraphicsScene(Field *parent = 0);
        std::vector<QGraphicsPixmapItem*> m_vecShips;
    signals:
        // Decrease the number of available ships of a type when it is moved via drag and drop to the field
        void ToAS_DecreaseShipNumber(int);
    protected:
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    private:
        Field *m_ClassField;
        QGraphicsPixmapItem *m_currentdraggeditem;
        // Align the ship with the grid
        void AlignShip(QGraphicsPixmapItem *);
};

#endif // MYGRAPHICSSCENE_HPP

