#ifndef FIELD_HPP
#define FIELD_HPP

#include <QWidget>
#include <QtWidgets>
#include <vector>
#include "UserInterface.hpp"
#include "MyGraphicsScene.hpp"

class Field : public QWidget {

    Q_OBJECT

public:
    Field(QWidget *parent = 0, UserInterface *pui = 0);
    // Adjust aspect ratio when the window size is changed
    void AdjustAspectRatio();
    // Create the grid of the fields
    void InitializeGrid();

    QGraphicsView *m_pGraphics_view;
    MyGraphicsScene *m_pGridScene;
    bool m_bGameStarted;

    friend class MyGraphicsScene;
public slots:
    // Passes on the info of MyGraphicsScene to AvailableShips
    void FromMGS_DecreaseShipNumber(int);
    // Draw the shot
    void PlaceShot(std::vector<int>);
    // Receive signal when the window size is changed and do some operations
    void WindowResized();
signals:
    // Send the height to which the ship image must be scaled on drag and drop
    void SendPictureDragHeight(int);
    // Passes on the info of MyGraphicsScene to AvailableShips
    void ToAS_DecreaseShipNumber(int);
protected:
    UserInterface *ui;
    const int m_iPictureHeight;
    int m_iSceneHeight;
    int m_iSceneWidth;
    int m_LastClickPosX;
    int m_LastClickPosY;

};

#endif // FIELD_HPP
