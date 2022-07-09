#include "Chess.h"
#include <thread>

namespace Chess {

    UserInterface* ChessApp::UI = nullptr;

    void ChessApp::onLoad() {
        std::cout << "load" << std::endl;
        UI->updatePosition(0, 'r');
    }
    void ChessApp::onSelect(size_t pos) {
        std::cout << "pos: " << pos << std::endl;
    }
    void ChessApp::onUpdate() {
        std::cout << "update" << std::endl;
    }

    void ChessApp::Start() {
        IOFactory* UIFactory = new UltralightIOFactory();
        UI = UIFactory->createUserInterface();
        GameLogic* logic = new ChessLogic();

        UI->getEvents()->onSelect.AddListener(onSelect);
        UI->getEvents()->onLoad.AddListener(onLoad);
        UI->getEvents()->onUpdate.AddListener(onUpdate);

        StartUI();

        delete UIFactory;
        delete logic;
        delete UI;
    }
}