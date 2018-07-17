#include "AvailableShips.hpp"

// Constructor
AvailableShips::AvailableShips(QWidget *parent) : QWidget(parent) {

    m_ImageDropHeight = 60;

    // For all ships
    for(uint i = 2; i <= 5; i++) {

        // Set image
        m_Ships.at(i).Image.setPixmap(QPixmap(QString::fromStdString(":/Ship_size" + std::to_string(i) + ".png")).scaledToHeight(m_ImageDropHeight));

        // Set the number of available ships
        m_Ships.at(i).Number.setFixedSize(m_Ships.at(i).Image.pixmap()->width(), 20);
        m_Ships.at(i).Number.setText("0");
        m_Ships.at(i).Number.setStyleSheet("qproperty-alignment:AlignCenter;Font-weight:bold;Font-size:15px;");

        // Add elements to vertical layout
        m_vLayout.at(i-2).addWidget(&(m_Ships.at(i).Image));
        m_vLayout.at(i-2).addWidget(&(m_Ships.at(i).Number));
        m_vLayout.at(i-2).setContentsMargins(0, 0, 0, 0);

        // Add vertical layout to horizontal layout
        m_hLayout.addLayout(&(m_vLayout.at(i-2)), 1);

    }
    m_hLayout.setContentsMargins(0, 0, 0, 0);
    this->setLayout(&m_hLayout);

}

// When the mouse button is pressed
void AvailableShips::mousePressEvent(QMouseEvent *event) {

    // If not the left button was pressed
    if(event->button() != Qt::LeftButton) return;

    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));

    // Check, if it was clicked on a ship
    bool valid = false;
    int i;
    for(i = 2; i <= 5; i++) {

        if(child == &(m_Ships.at(i).Image)) {

            valid = true;
            break;

        }

    }

    // If it was not clicked on a ship OR if the number of available ships is <= 0
    if(!valid || m_Ships.at(i).Number.text().toInt() <= 0) return;

    QPixmap pixmap = *child->pixmap();
    QPoint pixel_offset = (event->pos() - child->pos())/m_ImageDropHeight;

    // Store data in QByteArray
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << pixel_offset << i;

    // Set MIME data
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    // Drag event
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap.scaledToHeight(m_ImageDropHeight));
    drag->setHotSpot(pixel_offset);

    // Start the drag and drop operation
    drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

}

// Receive the image height value when the size of the window is changed (for scaling the ship image)
void AvailableShips::SetPictureDragHeight(int value) {

    m_ImageDropHeight = value;

}

// Decrease the number of available ships after drag and drop operation
void AvailableShips::FromMGS_DecreaseShipNumber(int value) {

    if(value < 2 || value > 5) return;

    int OldNumber = m_Ships.at(value).Number.text().toInt();

    if(OldNumber <= 0) return;

    m_Ships.at(value).Number.setText(QString::fromStdString(std::to_string(OldNumber-1)));

    // Check, if all ships are placed on the field
    bool StartGame = true;

    for(int i = 2; i <= 5; i++) {

        if(m_Ships.at(i).Number.text().toInt() > 0) StartGame = false;

    }
    // If all ships are placed
    if(StartGame) emit ToG_StartGame();

}

// Set the number of all available ships
void AvailableShips::SetNumbersOfShips(std::array<int, 4> value) {    // NumShip5, NumShip4, NumShip3, NumShip2

   m_Ships.at(2).Number.setText(QString::fromStdString(std::to_string(value.at(3))));
   m_Ships.at(3).Number.setText(QString::fromStdString(std::to_string(value.at(2))));
   m_Ships.at(4).Number.setText(QString::fromStdString(std::to_string(value.at(1))));
   m_Ships.at(5).Number.setText(QString::fromStdString(std::to_string(value.at(0))));

}
