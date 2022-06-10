#pragma once

class Chess::ConsoleInputManager;
class Chess::ConsoleUserInterface;
class Chess::ConsoleIOFactory;

#include "pch.h"
#include "Chess.h"

namespace Chess {
	class ConsoleInputManager : public InputManager {
	public:
		virtual std::string getInput();
	};

	class ConsoleUserInterface : public UserInterface {
	public:
		virtual void updatePosition(size_t);
		virtual void setMovePosition(size_t);
		virtual void showPromotion();
		virtual void refresh();
	};

	class ConsoleIOFactory {
	public:
		virtual ConsoleInputManager createInputManager() { return ConsoleInputManager(); };
		virtual ConsoleUserInterface createUserInterface() { return ConsoleUserInterface(); };
	};
}