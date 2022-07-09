#include "pch.h"

#include "BoardGame.h"
#include "ChessLogic.h"
#include "Ultralight.h"

namespace Chess {

	using namespace BoardGame;

	class ChessApp {
	public:
		static void Start();

	private:
		ChessApp();

		static void StartUI() { UI->Start(); }
		static void onLoad();
		static void onSelect(size_t);
		static void onUpdate();


		static void ShowPromotingMoves(const std::vector<Move>&);
		static void RefreshBoard();
		static bool isOnMovingSide(char);
		static const std::vector<Figure*>& getPositions();

		static const std::string defaultPositions;

		static size_t selectedPos;
		static UserInterface* UI;
		static GameLogic* logic;
		static std::vector<Move> promotingMoves;
		static bool IsPromoting() {
			return promotingMoves.size() > 0;
		}
		static void MakePromotingMove(size_t&);

		static void initPositions();
	};
}