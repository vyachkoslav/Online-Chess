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

		static const std::string defaultPositions;

		static UserInterface* UI;

		static void initPositions();
	};
}