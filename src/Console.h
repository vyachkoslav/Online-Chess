#pragma once

class Chess::ConsoleInputManager;
class Chess::ConsoleUserInterface;
class Chess::ConsoleIOFactory;

#include "pch.h"
#include "Chess.h"

namespace Chess {

	class ConsoleUserInterface : public UserInterface {
	public:
		virtual void updatePosition(size_t);
		virtual void setMovePosition(size_t);
		virtual void showPromotion();
		virtual void refresh();
		virtual std::string getInput();
	};

	class ConsoleIOFactory {
	public:
		virtual ConsoleUserInterface createUserInterface() { return ConsoleUserInterface(); };
	};
}