#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>

#include <iostream>

using namespace ultralight;

std::string JSStringToString(JSContextRef ctx, JSValueRef str) {
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

class ChessApp : public LoadListener {
    RefPtr<Overlay> overlay_;
public:
    ChessApp(Ref<Window> win) {
        overlay_ = Overlay::Create(win, win->width(), win->height(), 0, 0);

        overlay_->view()->set_load_listener(this);

        overlay_->view()->LoadURL("file:///board.html");
    }

    virtual ~ChessApp() {}

    static JSValueRef OnTileClick(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
        
        std::string cpp_string = JSStringToString(ctx, arguments[0]);
        
        std::cout << cpp_string << std::endl;

        return JSValueMakeNull(ctx);
    }

    void OnDOMReady(View* caller,
        uint64_t frame_id,
        bool is_main_frame,
        const String& url) {

        Ref<JSContext> context = caller->LockJSContext();

        JSContextRef ctx = context.get();

        JSStringRef name = JSStringCreateWithUTF8CString("OnTileClick");

        JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
            &OnTileClick);

        JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

        JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);
        JSStringRelease(name);
    }
};

int main() {
    auto app = App::Create();
    auto window = Window::Create(app->main_monitor(), 500, 500, false, kWindowFlags_Titled);

    window->SetTitle("Online Chess");

    app->set_window(window);

    ChessApp my_app(window);
    app->Run();

    return 0;
}