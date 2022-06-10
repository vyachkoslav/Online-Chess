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

		const Board& getBoard() const { return board; }

		virtual std::vector<Move> availableMovesForFigure(const Figure&) const = 0;
		virtual GameState CheckBoardState() const = 0;
	protected:
		Board& board;
	};
	class ChessLogic : public GameLogic {
	public:
		virtual std::vector<Move> availableMovesForFigure(const Figure&) const;
		virtual GameState CheckBoardState() const;
	};

	class Board {
	public:
		Board(size_t, std::vector<Figure*>);

		const std::vector<Figure*>& getPositions() const { return positions; };
		bool makeMove(Move);
		bool undoMove();
		size_t getWidth() const { return width; };
		size_t getHeight() const { return height; };
		GameLogic::Side getMovingSide() const { return movingSide; };

	private:
		GameLogic::Side movingSide = GameLogic::Side::White;
		size_t width, height;
		std::vector<Figure*> positions;
	};

	class Connection {
	public:
		void setConnectionSettings(std::string, unsigned int);
		bool connect();
		bool isConnected() const;
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

