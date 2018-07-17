QT += core gui
QT += network widgets

@
QMAKE_CXXFLAGS += -std=c++11
@

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Battleship
TEMPLATE = app

win32: RC_ICONS = Ressources/Icon.ico

SOURCES += \
    main.cpp \
    UserInterface/UserInterface.cpp \
    UserInterface/Chat.cpp \
    UserInterface/ConnSettings.cpp \
    UserInterface/Game.cpp \
    UserInterface/GameSettings.cpp \
    UserInterface/AvailableShips.cpp \
    UserInterface/Field.cpp \
    UserInterface/MyField.cpp \
    UserInterface/EnemyField.cpp \
    UserInterface/MyGraphicsScene.cpp \
    GameEngine.cpp \
    ConstParam.cpp \
    ConnectionHandler.cpp

HEADERS  += \
    UserInterface/UserInterface.hpp \
    UserInterface/Chat.hpp \
    UserInterface/ConnSettings.hpp \
    UserInterface/Game.hpp \
    UserInterface/GameSettings.hpp \
    UserInterface/AvailableShips.hpp \
    UserInterface/Field.hpp \
    UserInterface/MyField.hpp \
    UserInterface/EnemyField.hpp \
    UserInterface/MyGraphicsScene.hpp \
    GameEngine.hpp \
    ConstParam.hpp \
    SignalValue.hpp \
    ConnectionHandler.hpp

FORMS    += \
    UserInterface/UserInterface.ui

RESOURCES += \
    Ressources/ressources.qrc

DISTFILES +=
