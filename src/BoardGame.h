#pragma once

namespace BoardGame {
	class GameLogic;
	class Team;
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
	using Pos2D = std::pair<long long, long long>;

	enum class GameState {
		Unfinished,
		Finished,
		Draw
	};
	enum class Side {
		White,
		Black
	};
	class Team {
	public:
		static void setAllies(Team* first, Team* second, bool shareAllies = false);
		static void setEnemies(Team* first, Team* second, bool withSharedAllies = false);
		bool isAlly(Team* team) const;
		const std::vector<Team*>& getAllies() const { return allies; }

	private:
		void addAlly(Team* enemy, bool shared = false);
		void removeAlly(Team* ally, bool shared = false);
		bool tryRemoveSharedAlly(Team* ally);

		std::vector<Team*> allies;
		std::vector<Team*> sharedAllies;
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
		Board(size_t width, const std::vector<Figure*>& positions);
		~Board();

		const std::vector<Figure*>& getPositions() const { return positions; };

		bool makeMove(const Move&);
		bool undoMove();
		bool redoMove();
		bool canUndo() const { return currentMove != moves.begin(); };
		bool canRedo() const { return currentMove != moves.end(); };

		size_t getWidth() const { return width; };
		size_t getHeight() const { return height; };

		const std::vector<Move*>& getMoves() const { return moves; };
		std::vector<Move*>::iterator getCurrentMove() const { return currentMove; };

	private:
		
		size_t width, height;
		std::vector<Figure*> positions;

		std::vector<Move*> moves;
		std::vector<Move*>::iterator currentMove{ moves.end() };
	};

	class GameLogic {
	public:
		virtual Board* getBoard() = 0;

		virtual std::vector<Move> availableMovesForFigure(const Figure&) const = 0;
		virtual std::vector<Move> availableMovesForFigure(size_t position) const = 0;
		virtual GameState ñheckBoardState() const = 0;
	};

	class Connection {
	public:
		void setConnectionSettings(std::string ip, unsigned int port);
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
		Event<> onUndo;
		Event<std::string> onInput;
	};

	class UserInterface {
	public:
		virtual void Start() = 0;
		virtual void updatePosition(size_t index, char name) = 0;
		virtual void setMovePosition(size_t index) = 0;
		virtual void printText(std::string message) = 0;
		virtual std::string getInput(std::string message) = 0;
		virtual UIEvents* getEvents() = 0;
	};

	class IOFactory {
	public:
		virtual UserInterface* createUserInterface() = 0;
	};
}