#include "Ultralight.h"

namespace BoardGame {
	
	using ULUI = UltralightUserInterface;
	ULUI* ULUI::_instance = nullptr;

	UserInterface* ULUI::Instance(){
		if (_instance == nullptr) {
			_instance = new ULUI();
		}

		return _instance;
	}

	void ULUI::Start() {
		if (eventHandler)
			return;

		auto app = App::Create();
		auto window = Window::Create(app->main_monitor(), 450, 450, false, kWindowFlags_Titled);
		window->SetTitle("Online Chess");
		app->set_window(window);

		auto handler = EventHandler(window);
		eventHandler = &handler;

		app->Run();
	}
	void ULUI::updatePosition(size_t index, char name) {
		int x = index % 8;
		int y = index / 8;

		std::ostringstream oss;
		oss << "SetPosition('" << x << "', '" << y << "', '" << name << "');"; // todo only index as parameter
		const ultralight::String command = oss.str().c_str();

		eventHandler->RunCommand(command);
	}
	void ULUI::setMovePosition(size_t index) {
		int x = index % 8; 
		int y = index / 8;

		std::ostringstream oss;
		oss << "SetMove('" << x << "', '" << y << "');"; // todo only index as parameter
		const ultralight::String command = oss.str().c_str();

		eventHandler->RunCommand(command);
	}
	void ULUI::showPromotion() {
		
	}
	void ULUI::show() {
		// not used
	}
	std::vector<std::string> ULUI::getInput(){
		std::vector<std::string> tmp{ ULUI::inputBuffer };
		ULUI::inputBuffer.clear();
		return tmp;
	}

	ULUI::EventHandler::EventHandler(Ref<Window> win) {
		overlay_ = Overlay::Create(win, win->width(), win->height(), 0, 0);
		overlay_->view()->set_load_listener(this);
		overlay_->view()->LoadURL("file:///board.html");
	}
	void ULUI::EventHandler::OnDOMReady(
		View* caller, 
		uint64_t frame_id, 
		bool is_main_frame, 
		const String& url)
	{
		Ref<JSContext> context = caller->LockJSContext();
		JSContextRef ctx = context.get();
		{
			JSStringRef name = JSStringCreateWithUTF8CString("OnTileClick");
			JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
				&OnTileClick);
			JSObjectRef globalObj = JSContextGetGlobalObject(ctx);
			JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);
			JSStringRelease(name);
		}

		RunCommand("init();");
	}
	std::string ULUI::EventHandler::JSStringToString(JSContextRef ctx, JSValueRef str) {
		JSValueRef exc = nullptr;
		JSStringRef string_ref = JSValueToStringCopy(ctx, str, &exc);
		if (exc) {
			return "";
		}

		size_t buffer_size = JSStringGetMaximumUTF8CStringSize(string_ref);

		if (buffer_size < 1) {
			return "";
		}
		char* buffer = new char[buffer_size];

		size_t real_size = JSStringGetUTF8CString(string_ref, buffer, buffer_size);
		JSStringRelease(string_ref);

		std::string ret(buffer, real_size - 1);

		delete[] buffer;
		return ret;
	}
	JSValueRef ULUI::EventHandler::OnTileClick(
		JSContextRef ctx, 
		JSObjectRef function, 
		JSObjectRef thisObject, 
		size_t argumentCount, 
		const JSValueRef arguments[], 
		JSValueRef* exception)
	{
		std::string cpp_string = JSStringToString(ctx, arguments[0]);
		ULUI::_instance->addInput(cpp_string);
		std::cout << cpp_string << std::endl;

		return JSValueMakeNull(ctx);
	}
	String ULUI::EventHandler::RunCommand(const String& command) {
		return overlay_->view()->EvaluateScript(command);
	}
}