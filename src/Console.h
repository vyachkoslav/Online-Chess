#pragma once
namespace BoardGame {
	class ConsoleUserInterface;
	class ConsoleIOFactory;
}

#include "pch.h"
#include "Chess.h"

namespace BoardGame {

	class ConsoleUserInterface : public UserInterface {
	public:
		virtual void updatePosition(size_t);
		virtual void setMovePosition(size_t);
		virtual void showPromotion();
		virtual void show();
		virtual std::string getInput();
	};

	class ConsoleIOFactory {
	public:
		virtual ConsoleUserInterface createUserInterface() { return ConsoleUserInterface(); };
	};
}