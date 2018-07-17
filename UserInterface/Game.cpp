#include "Game.hpp"

// Constructor
Game::Game(UserInterface *pui) : QWidget(0), ui(pui) {

    // Deactivate tab
    ui->tab_Game->setEnabled(false);

    // Create object of class "AvailableShips"
    available_ships = new AvailableShips(ui->scrollArea_AvailableShipsL);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(ui->scrollArea_AvailableShipsL);
    horizontalLayout->addWidget(available_ships);
    horizontalLayout->setContentsMargins(5,5,5,5);
    ui->scrollArea_AvailableShipsL->setLayout(horizontalLayout);

    // Create the two fields
    my_field    = new MyField(ui->tab_Game, ui);
    enemy_field = new EnemyField(ui->tab_Game, ui);

    ui->gridLayout_2->addWidget(my_field, 1, 0);
    ui->gridLayout_2->addWidget(enemy_field, 1, 1);

    connect(this, SIGNAL(TransWindowResized()), my_field, SLOT(WindowResized()));
    connect(this, SIGNAL(TransWindowResized()), enemy_field, SLOT(WindowResized()));
    connect(my_field, SIGNAL(SendPictureDragHeight(int)), available_ships, SLOT(SetPictureDragHeight(int)));
    connect(enemy_field, SIGNAL(SendPictureDragHeight(int)), available_ships, SLOT(SetPictureDragHeight(int)));
    connect(my_field, SIGNAL(ToAS_DecreaseShipNumber(int)), available_ships, SLOT(FromMGS_DecreaseShipNumber(int)));
    connect(available_ships, SIGNAL(ToG_StartGame()), this, SLOT(FromAS_StartGame()));
    connect(ui->pushButton_StartGame, SIGNAL(clicked()), this, SLOT(ButtonBeginGame()));
    connect(this, SIGNAL(ToMF_StartGame()), my_field, SLOT(FromG_StartGame()));
    connect(my_field, SIGNAL(ToG_StatusText(QString)), this, SLOT(FromMF_StatusText(QString)));
    connect(my_field, SIGNAL(ToG_ShipPos(std::vector<int>)), this, SLOT(FromMF_ShipPos(std::vector<int>)));
    connect(this, SIGNAL(ToMF_PlaceShot(std::vector<int>)), my_field, SLOT(PlaceShot(std::vector<int>)));
    connect(enemy_field, SIGNAL(ToG_SendShot(std::vector<int>)), this, SLOT(FromEF_SendShot(std::vector<int>)));
    connect(this, SIGNAL(ToEF_ShipPos(std::vector<int>)), enemy_field, SLOT(FromG_ShipPos(std::vector<int>)));
    connect(this, SIGNAL(ToEF_LastShotHit()), enemy_field, SLOT(FromG_LastShotHit()));
    connect(my_field, SIGNAL(ToG_SendGameBeginToCH()), this, SLOT(FromMF_SendGameBeginToCH()));

}

// Receive the position of a shot from the enemy
void Game::receiveShotFromGE(std::vector<int> iCoord) {

    emit ToMF_PlaceShot(iCoord);
    ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy has fired!</span><br>");

    // Allow shooting
    enemy_field->m_ShootOK = true;

    // Change cursor
    enemy_field->setShotCursor(true);

}

void Game::receiveShipPosFromGE(std::vector<int> arrPos) {

    ui->textBrowser_Chat->append("<span style='color:#04B404;'>A ship of the enemy has sunk!</span><br>");

    // Pass on coordinates
    emit ToEF_ShipPos(arrPos);

}

// Receive different state messages from the ConnectionHandler
void Game::receiveStateFromCH(SignalValue iState) {

    // If not the Game tab is active
    if(!ui->m_bIsTabGame) return;

    switch(iState) {

        case SignalValue::StartGame:
            ui->pushButton_StartGame->setText("Finish\nwith\nplacing");
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy has already placed his ships!</span><br>");
            break;
        case SignalValue::OK:
            if(ui->pushButton_StartGame->text() == "Stop\nGame") {

                ui->textBrowser_Chat->append("<span style='color:#04B404;'>You can shoot now!</span><br>");

                // Allow shooting
                enemy_field->m_ShootOK = true;

                // Change cursor
                enemy_field->setShotCursor(true);

            }
            break;
        case SignalValue::RejectedNotReady:
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy has rejected the game request!"
                                         "<br>Please send him a request again!</span><br>");
            ui->pushButton_StartGame->setText("Start Game");
            break;
        default:
            break;

    }

}

// Receive different state messages from the GameEngine
void Game::receiveStateFromGE(SignalValue iState, std::vector<int>) {

    switch(iState) {

        case SignalValue::AlreadyShot:
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>At this point has already been shot!</span><br>");

            // Allow shooting
            enemy_field->m_ShootOK = true;

            // Change cursor
            enemy_field->setShotCursor(true);
            break;
        case SignalValue::Hit:
            // When we just shot
            if(enemy_field->m_ShootOK == false) {

                ui->textBrowser_Chat->append("<span style='color:#04B404;'>You hit!</span><br>");

                // Show green X
                emit ToEF_LastShotHit();

            }
            // When the enemy has just fired
            else ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy has hit you!</span><br>");

            break;
        case SignalValue::NotHit:
            // When we just shot
            if(enemy_field->m_ShootOK == false) {

                ui->textBrowser_Chat->append("<span style='color:#04B404;'>Unforunately you did not hit!</span><br>");

            }
            // When the enemy has just fired
            else {

                ui->textBrowser_Chat->append("<span style='color:#04B404;'>The enemy did not hit you!</span><br>");

            }
            break;
        case SignalValue::GameEnd:
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>The game is over!"
                                         "<br>You won!</span><br>");
            ui->pushButton_StartGame->setEnabled(false);

            // Deny shooting
            enemy_field->m_ShootOK = false;

            // Change cursor
            enemy_field->setShotCursor(false);
            break;
        case SignalValue::SunkGameEnd:
            ui->textBrowser_Chat->append("<span style='color:#04B404;'>The game is over!"
                                         "<br>You lost!</span><br>");
            ui->pushButton_StartGame->setEnabled(false);

            // Deny shooting
            enemy_field->m_ShootOK = false;

            // Change cursor
            enemy_field->setShotCursor(false);
            break;
        default:
            break;

    }

}

// Initialize the Game tab
void Game::startGameTab() {

    ui->m_bIsTabGame = true;

    ui->tab_Game->setEnabled(true);
    ui->tabWidget->setCurrentIndex(2);  // Go to tab "Game"
    ui->pushButton_StartGame->setText("Start Game");
    ui->pushButton_StartGame->setEnabled(false);

    // Set the number of fields in X and Y direction
    ui->m_iAspectRatioX = ui->m_arrGameParameter.at(0);
    ui->m_iAspectRatioY = ui->m_arrGameParameter.at(1);

    // Initialize the available ships
    std::array<int, 4> iShipNumbers;
    iShipNumbers.at(0) = ui->m_arrGameParameter.at(2);
    iShipNumbers.at(1) = ui->m_arrGameParameter.at(3);
    iShipNumbers.at(2) = ui->m_arrGameParameter.at(4);
    iShipNumbers.at(3) = ui->m_arrGameParameter.at(5);
    available_ships->SetNumbersOfShips(iShipNumbers);   // NumShip5, NumShip4, NumShip3, NumShip2

    // Initialize my_field
    my_field->InitializeGrid();

    // Initialize enemy_field
    enemy_field->InitializeGrid();

    // Deny shooting
    enemy_field->m_ShootOK = false;

    // Change cursor
    enemy_field->setShotCursor(false);

    // Anjust aspect ratio
    emit TransWindowResized();

    ui->textBrowser_Chat->append("<span style='color:#04B404;'>Please place the ships on your field by dragging them with "
                                 "the mouse to the corresponding position (ship can be turned with right mouse button on placed ship).</span><br>");

}

// When the size of the main window was changed
void Game::WindowResized() {

    // Change buttons for ScrollArea
    if(ui->scrollArea_AvailableShips->width() < available_ships->width()) {

        ui->label_ScrollLeft->setText("<");
        ui->label_ScrollRight->setText(">");

    }
    else {

        ui->label_ScrollLeft->setText("");
        ui->label_ScrollRight->setText("");

    }
    emit TransWindowResized();

}

// Activate the button "Start Game" (when all ships are placed)
void Game::FromAS_StartGame() {

    ui->pushButton_StartGame->setEnabled(true);
    ui->textBrowser_Chat->append("<span style='color:#04B404;'>All ships were placed. If no ships overlap, then the "
                                 "fight can start!</span><br>");

}

// Is called when the button "Start Game" is pressed
void Game::ButtonBeginGame() {

    if(ui->pushButton_StartGame->text() == "Stop\nGame") {

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>You have aborted the game!</span><br>");
        ui->pushButton_StartGame->setEnabled(false);

        emit sendStateToGE(SignalValue::GameEnd, {});
        emit sendStateToCH(SignalValue::GameEnd);

    }
    else {

        emit ToMF_StartGame();

    }

    // Deny shooting
    enemy_field->m_ShootOK = false;

    // Change cursor
    enemy_field->setShotCursor(false);

}

// Print messages from MyField to the Chat
void Game::FromMF_StatusText(QString String) {

    ui->textBrowser_Chat->append("<span style='color:#04B404;'>" + String + "</span><br>");

}

// Passes on the position of a ship from MyField to the GameEngine
void Game::FromMF_ShipPos(std::vector<int> iCoord) {

    emit sendShipPosToGE(iCoord);

}

// Passes on the info about the game start to the ConnectionHandler
void Game::FromMF_SendGameBeginToCH() {

    // When we first start shooting
    if(ui->pushButton_StartGame->text() == "Start Game") {

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>You started the game."
                                     "<br>Wait for OK of the enemy...</span><br>");

        emit sendStateToCH(SignalValue::StartGame);

    }
    // When the enemy first starts shooting
    else {

        ui->textBrowser_Chat->append("<span style='color:#04B404;'>Wait for shot of the enemy...</span><br>");

        emit sendStateToCH(SignalValue::OK);

    }
    // Deny shooting
    enemy_field->m_ShootOK = false;

    // Change cursor
    enemy_field->setShotCursor(false);

    ui->pushButton_StartGame->setText("Stop\nGame");
    ui->pushButton_StartGame->setEnabled(true);
    my_field->m_bGameStarted = true;
    enemy_field->m_bGameStarted = true;

}

// Passes on the position of a shot to the ConnectionHandler
void Game::FromEF_SendShot(std::vector<int> arrPos) {

   emit sendShotToGE(arrPos);

}
