#include "pch.h"

#include "BoardGame.h"
#include "ChessLogic.h"
#include "Ultralight.h"

using namespace BoardGame;

class ChessApp {
public:
	static void Start();

private:
	ChessApp();

	static void StartUI() { UI->Start(); }

	static const std::string defaultPositions;

	static UserInterface* UI;

	static void initPositions();
};