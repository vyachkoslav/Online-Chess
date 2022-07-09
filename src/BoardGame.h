#pragma once

namespace BoardGame {
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
#include "Event.tpp"

namespace BoardGame {

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
		virtual const Board* getBoard() = 0;

		virtual std::vector<Move> availableMovesForFigure(const Figure&) const = 0;
		virtual GameState CheckBoardState() const = 0;
	};

	class Connection {
	public:
		void setConnectionSettings(std::string, unsigned int);
		bool connect();
		bool isConnected() const;
		void closeConnection();
		std::string getMessage();
	};

	struct UIEvents{
		Event<size_t> onSelect;
		Event<> onLoad;
		Event<> onClose;
		Event<> onUpdate;
	};

	class UserInterface {
	public:
		virtual void Start() = 0;
		virtual void updatePosition(size_t, char) = 0;
		virtual void setMovePosition(size_t) = 0;
		virtual void showPromotion() = 0;
		virtual void show() = 0;
		virtual std::vector<std::string> getInput() = 0;
		virtual UIEvents* getEvents() = 0;
	};

	class IOFactory {
	public:
		virtual UserInterface* createUserInterface() = 0;
	};
}

