#include "Ultralight.h"

namespace BoardGame {

	using ULUI = UltralightUserInterface;
	ULUI* ULUI::instance_ = nullptr;

	UserInterface* ULUI::Instance() {
		if (instance_ == nullptr) {
			instance_ = new ULUI();
		}

		return instance_;
	}
	void ULUI::Start() {
		auto app = App::Create();
		auto window = Window::Create(app->main_monitor(), 450, 450, false, kWindowFlags_Titled);
		window->SetTitle("Online Chess");
		app->set_window(window);

		auto handler = EventHandler(window);
		eventHandler = &handler;

		app->set_listener(eventHandler);
		app->Run();
	}
	void ULUI::updatePosition(size_t index, char name) {
		std::ostringstream oss;
		oss << "SetPosition('" << index << "', '" << name << "');";
		const ultralight::String command = oss.str().c_str();

		eventHandler->RunCommand(command);
	}
	void ULUI::setMovePosition(size_t index) {
		std::ostringstream oss;
		oss << "SetMove('" << index  << "');";
		const ultralight::String command = oss.str().c_str();

		eventHandler->RunCommand(command);
	}
	void ULUI::printText(std::string line) {
		std::ostringstream oss;
		oss << "Print('" << line << "');";
		const ultralight::String command = oss.str().c_str();
		eventHandler->RunCommand(command);
	}
	std::string ULUI::getInput(std::string message = "") {
		std::ostringstream oss;
		oss << "prompt('" << message << "');";
		const ultralight::String command = oss.str().c_str();
		eventHandler->RunCommand(command);
		return "";
	}

	ULUI::EventHandler::EventHandler(Ref<Window> win) {
		overlay_ = Overlay::Create(win, win->width(), win->height(), 0, 0);
		overlay_->view()->set_load_listener(this);
		overlay_->view()->LoadURL("file:///board.html");
	}
	void ULUI::EventHandler::OnUpdate() {
		ULUI::Instance()->getEvents()->onUpdate.Invoke();
		Sleep(1000 / 30); //30fps
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
		ULUI::Instance()->getEvents()->onLoad.Invoke();
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
		ULUI::instance_->addInput(cpp_string);
		ULUI::Instance()->getEvents()->onSelect.Invoke(std::stoi(cpp_string));

		return JSValueMakeNull(ctx);
	}
	String ULUI::EventHandler::RunCommand(const String& command) {
		return overlay_->view()->EvaluateScript(command);
	}
}