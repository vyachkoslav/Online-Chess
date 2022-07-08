#pragma once

namespace BoardGame {
	class UltralightUserInterface;
	class UltralightIOFactory;
}

#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>

#include <iostream>
#include <sstream>

#include "pch.h"
#include "Chess.h"

namespace BoardGame {
	using namespace ultralight;

	class UltralightUserInterface : public UserInterface {
	public:
		static UserInterface* Instance();
		virtual void Start();
		virtual void updatePosition(size_t, char);
		virtual void setMovePosition(size_t);
		virtual void showPromotion();
		virtual void show();
		virtual std::vector<std::string> getInput();
	protected:
		UltralightUserInterface() = default;
	private:
		static UltralightUserInterface* _instance;
		std::vector<std::string> inputBuffer;
		void addInput(std::string str) { inputBuffer.push_back(str); }

		class EventHandler : public LoadListener {
			RefPtr<Overlay> overlay_;
		public:
			EventHandler(Ref<Window> win);
			virtual ~EventHandler() {};

			String RunCommand(const String&);
		private:
			void OnDOMReady(View* caller,
				uint64_t frame_id,
				bool is_main_frame,
				const String& url);

			static std::string JSStringToString(JSContextRef ctx, JSValueRef str);
			static JSValueRef OnTileClick(
				JSContextRef ctx, 
				JSObjectRef function, 
				JSObjectRef thisObject, 
				size_t argumentCount, 
				const JSValueRef arguments[], 
				JSValueRef* exception);

		};
		EventHandler* eventHandler{ nullptr };
	};

	class UltralightIOFactory : public IOFactory {
	public:
		virtual UserInterface* createUserInterface() override { return UltralightUserInterface::Instance(); };
	};
}