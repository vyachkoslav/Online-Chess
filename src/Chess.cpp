#include "Chess.h"
#include <thread>

namespace Chess {

    UserInterface* ChessApp::UI = nullptr;
    GameLogic* ChessApp::logic = nullptr;
    size_t ChessApp::selectedPos;
    std::vector<Move> ChessApp::promotingMoves;
    Team ChessApp::teams[2];
    Team* ChessApp::movingTeam = &teams[0];

    void ChessApp::onLoad() {
        for (auto figure : logic->getBoard()->getPositions()) {
            if (figure) {
                UI->updatePosition(figure->posOnBoard, figure->name);
            }
        }
            
    }
    void ChessApp::onSelect(size_t pos) {
        size_t& selectedIndex = selectedPos;
        size_t newIndex = pos;
        Figure* selectedFigure = getPositions()[selectedIndex];
        Figure* newFig = getPositions()[newIndex];

        RefreshBoard();
        if (IsPromoting()) {
            MakePromotingMove(newIndex);
        }

        bool hasMove = false;
        if (selectedFigure != newFig && selectedFigure && isOnMovingSide(selectedFigure->name)) {
            auto moves = logic->availableMovesForFigure(selectedIndex);
            std::vector<Move> possibleMoves;
            for (const auto& move : moves) {
                for (const auto& action : move) {
                    if (action.figureAtStart.posOnBoard == selectedPos && action.figureAtDest.posOnBoard == newIndex) {
                        possibleMoves.push_back(move);
                        break;
                    }
                }
            }
            if (possibleMoves.size() == 1) {
                if (logic->getBoard()->makeMove(possibleMoves[0])) {
                    movingTeam = getOppositeTeam(movingTeam);
                    for (const auto& action : possibleMoves[0]) {
                        UI->updatePosition(action.figureAtStart.posOnBoard, ' ');
                        UI->updatePosition(action.figureAtDest.posOnBoard, action.nameAfter);
                        hasMove = true;
                    }
                }
            }
            else if (possibleMoves.size() > 1) {
                ShowPromotingMoves(possibleMoves);
            }
        }

        if (!hasMove) {
            selectedPos = newIndex;
            if (newFig && isOnMovingSide(newFig->name)) {
                for (const auto& move : logic->availableMovesForFigure(selectedPos)) {
                    const auto& action = move[0];
                    UI->setMovePosition(action.figureAtDest.posOnBoard);
                }
            }
        }
    }
    void ChessApp::onUpdate() {
        
    }

    void ChessApp::Start() {
        logic = new ChessLogic();
        IOFactory* UIFactory = new UltralightIOFactory();
        UI = UIFactory->createUserInterface();

        UI->getEvents()->onSelect.AddListener(onSelect);
        UI->getEvents()->onLoad.AddListener(onLoad);
        UI->getEvents()->onUpdate.AddListener(onUpdate);

        StartUI();

        delete UIFactory;
        delete UI;
        delete logic;
    }

    void ChessApp::RefreshBoard() {
        for (int i = 0; i < 64; ++i) {
            char name = ' ';
            if (getPositions()[i]) {
                name = getPositions()[i]->name;
            }

            UI->updatePosition(i, name);
        }
    }
    bool ChessApp::isOnMovingSide(char name) {
        Team* figSide = getFigureTeam(name);
        return figSide == movingTeam;
    }
    Team* ChessApp::getFigureTeam(char name) {
        return isupper(name) ? &teams[0] : &teams[1];
    }
    Team* ChessApp::getOppositeTeam(Team* team) {
        return team == &teams[0] ? &teams[1] : &teams[0];
    }

    void ChessApp::ShowPromotingMoves(const std::vector<Move>& possibleMoves) {
        for (int i = 0; i < 64; ++i) {
            UI->updatePosition(i, ' ');
        }
        for (int i = 0; i < possibleMoves.size(); ++i) {
            Action action = possibleMoves[i][0];
            UI->updatePosition(i, action.nameAfter);
            UI->setMovePosition(i);
        }

        promotingMoves = possibleMoves;
    }

   void ChessApp::MakePromotingMove(size_t& pos) {
       size_t x = pos % 8;
       size_t y = pos / 8;
       if (x < promotingMoves.size() && y == 0) {
           Action action = promotingMoves[x][0];
           if (logic->getBoard()->makeMove(std::vector<Action>{action})) {
               movingTeam = getOppositeTeam(movingTeam);

               UI->updatePosition(action.figureAtStart.posOnBoard, ' ');
               UI->updatePosition(action.figureAtDest.posOnBoard, action.nameAfter);
               pos = selectedPos = action.figureAtDest.posOnBoard;
           }
       }
       promotingMoves.clear();
    }

    const std::vector<Figure*>& ChessApp::getPositions() {
        return logic->getBoard()->getPositions();
    }
}