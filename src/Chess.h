#pragma once

namespace Chess {
	class GameLogic;
	struct Figure;
	struct Action;
	class Board;
	class Connection;
	class UserInterface;
	class IOFactory;
	enum class GameState;
	enum class Side;
}

#include "pch.h"

namespace Chess {

	using Move = std::vector<Action>;
	using Pos2D = std::pair<size_t, size_t>;

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

	struct Figure {
		size_t posOnBoard, moveCount;
		char name;
	};

	struct Action {
		Action(Figure start, Figure dest, char after) : 
			figureAtStart(start), figureAtDest(dest), nameAfter(after) {};
		Figure figureAtStart, figureAtDest;
		char nameAfter;
	};

	class Board {
	public:
		Board(size_t, const std::vector<Figure*>&);

		const std::vector<Figure*>& getPositions() const { return positions; };

		bool makeMove(const Move&, Side side);
		bool undoMove();
		bool redoMove();
		bool canUndo() const { return currentMove != moves.begin(); };
		bool canRedo() const { return currentMove != moves.end(); };

		size_t getWidth() const { return width; };
		size_t getHeight() const { return height; };
		Side getMovingSide() const { return movingSide; };

		const std::vector<Move>& getMoves() const { return moves; };
		std::vector<Move>::iterator getCurrentMove() const { return currentMove; };

	private:
		
		Side movingSide{ Side::White };
		size_t width, height;
		std::vector<Figure*> positions;

		std::vector<Move> moves;
		std::vector<Move>::iterator currentMove{ moves.end() };
	};

	class GameLogic {
	public:
		const Board& getBoard() const { return board; }

		virtual std::vector<Move> availableMovesForFigure(const Figure&) const = 0;
		virtual GameState CheckBoardState() const = 0;

	protected:
		GameLogic(size_t _width, const std::vector<Figure*>& startPos) : 
			board(Board(_width, startPos)){}
		Board board;
	};

	class Connection {
	public:
		void setConnectionSettings(std::string, unsigned int);
		bool connect();
		bool isConnected() const;
		void closeConnection();
		std::string getMessage();
	};

	class UserInterface {
	public:
		virtual void updatePosition(size_t) = 0;
		virtual void setMovePosition(size_t) = 0;
		virtual void showPromotion() = 0;
		virtual void refresh() = 0;
		virtual std::string getInput() = 0;
	};

	class IOFactory {
	public:
		virtual InputManager createInputManager() = 0;
		virtual UserInterface createUserInterface() = 0;
	};
}

