#pragma once

class Chess::UltralightInputManager;
class Chess::UltralightUserInterface;
class Chess::UltralightIOFactory;

#include "pch.h"
#include "Chess.h"

namespace Chess {
	class UltralightInputManager : public InputManager {
	public:
		virtual std::string getInput();
	};
	
	class UltralightUserInterface : public UserInterface {
	public:
		virtual void updatePosition(size_t);
		virtual void setMovePosition(size_t);
		virtual void showPromotion();
		virtual void refresh();
	};

	class UltralightIOFactory {
	public:
		virtual UltralightInputManager createInputManager() { return UltralightInputManager(); };
		virtual UltralightUserInterface createUserInterface() { return UltralightUserInterface(); };
	};
}