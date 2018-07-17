#ifndef AVAILABLESHIPS_HPP
#define AVAILABLESHIPS_HPP

#include <QWidget>
#include <QtWidgets>
#include <array>

struct Ship {

    QLabel Number;
    QLabel Image;

};

// This class provides the ships which can be placed on the field (with drag and drop)
class AvailableShips : public QWidget {

    Q_OBJECT

    public:
        //Constructor
        AvailableShips(QWidget *parent = 0);
        // Set the number of available ships for all four ship types
        void SetNumbersOfShips(std::array<int, 4>);
    signals:
        // Send this signal when all ships are placed on the field
        void ToG_StartGame();
    public slots:
        // Receive the image height value when the size of the window is changed (for scaling the ship image)
        void SetPictureDragHeight(int);
        // Decrease the number of available ships when a ship was placed on the field
        void FromMGS_DecreaseShipNumber(int);
    protected:
        // When the mouse button is pressed
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    private:
        std::array<Ship, 6> m_Ships;    // The first two elements are not used
        std::array<QVBoxLayout, 4> m_vLayout;
        QHBoxLayout m_hLayout;
        uint m_ImageDropHeight;

};

#endif // AVAILABLESHIPS_HPP
