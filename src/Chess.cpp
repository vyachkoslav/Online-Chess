#include "Chess.h"
#include <thread>

UserInterface* ChessApp::UI = nullptr;

void ChessApp::Start() {
    IOFactory* UIFactory = new UltralightIOFactory();
    UI = UIFactory->createUserInterface();
    GameLogic* logic = new ChessLogic();

    std::thread UIThread(StartUI);

    UIThread.join();

    delete UIFactory;
    delete logic;
    delete UI;
}