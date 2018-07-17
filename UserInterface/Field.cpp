#include "Field.hpp"

// Constructor
Field::Field(QWidget *parent, UserInterface *pui) : QWidget(parent), ui(pui), m_iPictureHeight(90) {

    m_iSceneWidth   = ui->m_iAspectRatioX*m_iPictureHeight;
    m_iSceneHeight  = ui->m_iAspectRatioY*m_iPictureHeight;
    m_LastClickPosX = 0;
    m_LastClickPosY = 0;
    m_bGameStarted  = false;

    // Create GraphicsView
    m_pGraphics_view = new QGraphicsView(this);
    m_pGraphics_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pGraphics_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pGraphics_view->setFrameShape(QGraphicsView::NoFrame);
    m_pGraphics_view->setStyleSheet("background:#81BEF7");

    // Assign layout
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(m_pGraphics_view);
    this->setLayout(hLayout);

    // Create scene
    m_pGridScene = new MyGraphicsScene(this);

    connect(m_pGridScene, SIGNAL(ToAS_DecreaseShipNumber(int)), this, SLOT(FromMGS_DecreaseShipNumber(int)));

}

// Receive signal when the window size is changed and do some operations
void Field::WindowResized() {

    AdjustAspectRatio();

}

// Adjust aspect ratio when the window size is changed
void Field::AdjustAspectRatio() {

    int iSizeOfFieldX, iSizeOfFieldY;
    double dMaxWidth, dMaxHeight, dSizeOfFieldX, dSizeOfFieldY, dAspectRatio;

    dAspectRatio = static_cast<double>(ui->m_iAspectRatioX)/ui->m_iAspectRatioY;
    dMaxWidth    = 0.45*ui->width();
    dMaxHeight   = static_cast<double>(ui->height() - ui->groupBox_AvailableShips->height() - ui->groupBox_Chat->height() - 130);

    dSizeOfFieldX = dMaxWidth;
    dSizeOfFieldY = dMaxWidth/dAspectRatio;

    if(dSizeOfFieldY > dMaxHeight) {

        double dScaleFactor;

        dScaleFactor = dSizeOfFieldY/dMaxHeight;
        dSizeOfFieldX = dSizeOfFieldX/dScaleFactor;
        dSizeOfFieldY = dSizeOfFieldY/dScaleFactor;

    }
    iSizeOfFieldX = static_cast<int>(dSizeOfFieldX);
    iSizeOfFieldY = static_cast<int>(dSizeOfFieldY);

    this->setFixedWidth(iSizeOfFieldX);
    this->setFixedHeight(iSizeOfFieldY);

    // Send the height to which the ship image must be scaled on drag and drop
    emit SendPictureDragHeight(static_cast<int>(dSizeOfFieldY/ui->m_iAspectRatioY));

    // Scale Scene in GraphicsView
    m_pGraphics_view->fitInView(0, 0, m_iSceneWidth, m_iSceneHeight);

}

// Create the grid of the fields
void Field::InitializeGrid() {

    // Delete old grid and old ships
    m_pGridScene->clear();

    m_iSceneWidth  = ui->m_iAspectRatioX*m_iPictureHeight;
    m_iSceneHeight = ui->m_iAspectRatioY*m_iPictureHeight;

    // Set Scene view to the correct size
    m_pGridScene->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);

    // Draw verticale lines
    int iXPos = 0;
    for(int i = 0; i <= ui->m_iAspectRatioX; i++) {

        m_pGridScene->addLine(iXPos, 0, iXPos, m_iSceneHeight, QPen(Qt::black, 0));
        iXPos += m_iPictureHeight;

    }
    // Draw horizontal lines
    int iYPos = 0;
    for(int i = 0; i <= ui->m_iAspectRatioY; i++) {

        m_pGridScene->addLine(0, iYPos, m_iSceneWidth, iYPos, QPen(Qt::black, 0));
        iYPos += m_iPictureHeight;

    }
    // Scale Scene in GraphicsView
    m_pGraphics_view->setScene(m_pGridScene);
    m_pGraphics_view->fitInView(0, 0, m_iSceneWidth, m_iSceneHeight);
    m_pGraphics_view->show();

}

// Passes on the info of MyGraphicsScene to AvailableShips
void Field::FromMGS_DecreaseShipNumber(int value) {

    emit ToAS_DecreaseShipNumber(value);

}

// Draw the shot
void Field::PlaceShot(std::vector<int> iCoord) {

    QGraphicsPixmapItem *item = m_pGridScene->addPixmap(QPixmap(":/Shot.png"));
    int XPos, YPos;

    XPos = iCoord.at(0)*m_iPictureHeight;
    YPos = iCoord.at(1)*m_iPictureHeight;

    item->setPos(XPos, YPos);
    item->setZValue(1000);

}
