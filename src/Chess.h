#pragma once

class Chess::GameLogic;
class Chess::Figure;
class Chess::Action;
class Chess::Board;
class Chess::Connection;
class Chess::InputManager;
class Chess::UserInterface;
class Chess::IOFactory;

#include "pch.h"

namespace Chess {

	using Move = std::vector<Action>;

	struct Figure {
		size_t posOnBoard, moveCount;
		char name;
	};

	struct Action {
		Figure figureStart, figureAfter;
	};

	class GameLogic {
	public:

		enum class GameState {
			Unfinished,
			BlackWin,
			WhiteWin,
			Draw
		};
		enum class Side {
			White,
			Black
		};

		virtual Move availableMovesForFigure(const Figure&) = 0;
		virtual GameState CheckBoardState() = 0;
	};
	class ChessLogic : public GameLogic {
	public:
		virtual Move availableMovesForFigure(const Figure&);
		virtual GameState CheckBoardState();
	};

	class Board {
	public:
		Board(size_t, std::vector<Figure*>);

		std::vector<Figure*> getBoard();
		bool makeMove(Move);
		bool undoMove();
		size_t getWidth();
		size_t getHeight();
		GameLogic::Side getMovingSide();

	private:
		GameLogic::Side movingSide;
		size_t width, height;
		std::vector<Figure*> positions;
	};

	class Connection {
	public:
		void setConnectionSettings(std::string, unsigned int);
		bool connect();
		bool isConnected();
		void closeConnection();
		std::string getMessage();
	};

	class InputManager {
	public:
		virtual std::string getInput() = 0;
	};

	class UserInterface {
	public:
		virtual void updatePosition(size_t) = 0;
		virtual void setMovePosition(size_t) = 0;
		virtual void showPromotion() = 0;
		virtual void refresh() = 0;
	};

	class IOFactory {
	public:
		virtual InputManager createInputManager() = 0;
		virtual UserInterface createUserInterface() = 0;
	};
}

