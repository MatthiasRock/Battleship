#ifndef SIGNALVALUE_HPP
#define SIGNALVALUE_HPP

enum class SignalValue : char {
    OK = 0x00,
    RejectedNotReady = 0x01,
    NotSupported = 0x02,
    MessageIncomplete = 0x03,
    TimeOutOnConfirmation = 0x04,
    NotHit = 0x05,
    Hit = 0x06,
    Sunk = 0x07,
    SunkGameEnd = 0x08,
    StartGame,
    AlreadyShot,
    GameEnd,
    Disconnect,
    Connected,
    Disconnected,
    ConnectionImpossible,
    PlaceHolder,
    ErrorUnknown = 0x20
};

#endif // SIGNALVALUE_HPP
