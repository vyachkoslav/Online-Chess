#pragma once

namespace Chess {
	class UltralightUserInterface;
	class UltralightIOFactory;
}


#include "pch.h"
#include "Chess.h"

namespace Chess {
	
	class UltralightUserInterface : public UserInterface {
	public:
		virtual void updatePosition(size_t);
		virtual void setMovePosition(size_t);
		virtual void showPromotion();
		virtual void refresh();
		virtual std::string getInput();
	};

	class UltralightIOFactory {
	public:
		virtual UltralightUserInterface createUserInterface() { return UltralightUserInterface(); };
	};
}