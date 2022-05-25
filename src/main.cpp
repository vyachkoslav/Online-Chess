#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>

#include <iostream>
#include <sstream>
#include "Game.h"
#include "Logic.h"

using namespace ultralight;

namespace match_info {
    RefPtr<Overlay> overlay_;
    Board board = Board();
    std::vector<Figure*> const& positions = *board.getPositions();
    Logic logic = Logic(&positions);
    std::pair<int, int> selectedPos;
    Player thisPlayer = Player(Side::white);
    Player enemyPlayer = Player(Side::black);
}

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

public:
    ChessApp(Ref<Window> win) {
        match_info::overlay_ = Overlay::Create(win, win->width(), win->height(), 0, 0);

        match_info::overlay_->view()->set_load_listener(this);

        match_info::overlay_->view()->LoadURL("file:///board.html");
    }

    virtual ~ChessApp() {}

    static JSValueRef OnTileClick(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {

        std::string cpp_string = JSStringToString(ctx, arguments[0]);
        std::cout << cpp_string << std::endl;
        if(cpp_string.length() > 2)
            return JSValueMakeNull(ctx);
        std::pair<int, int> newPos;
        newPos.first = cpp_string[0] - '0';
        newPos.second = cpp_string[1] - '0';
        int selectedIndex = match_info::selectedPos.first + match_info::selectedPos.second * 8;
        int newIndex = newPos.first + newPos.second * 8;
        Figure* selectedFigure = match_info::positions[selectedIndex];
        Figure* newFig = match_info::positions[newIndex];

        if (selectedFigure != newFig) {
            ClearBoard();

            auto moves = match_info::logic.getAvailableMovesForFigure(match_info::selectedPos);
            bool hasMove = false;
            for (const auto &pos : moves) {
                std::cout << pos.first << " " << pos.second << std::endl;
                if (pos == newPos) {
                    match_info::board.makeMove(match_info::selectedPos, pos, match_info::board.getMovingSide());
                    UpdatePosition(match_info::selectedPos.first, match_info::selectedPos.second, ' ');
                    UpdatePosition(pos.first, pos.second, selectedFigure->name);
                    hasMove = true;
                    break;
                }
            }
            if (!hasMove) {
                match_info::selectedPos = newPos;
                for (const auto &pos : match_info::logic.getAvailableMovesForFigure(match_info::selectedPos)) {
                    UpdatePosition(pos.first, pos.second, 'm');
                }
            }
        }

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

        match_info::overlay_->view()->EvaluateScript("init();");
        for (int i = 0; i < 64; ++i) {
            if (match_info::positions[i]) {
                UpdatePosition(i % 8, i / 8, match_info::positions[i]->name);
            }
        }
    }
    static void ClearBoard() {
        for (int i = 0; i < 64; ++i) {
            char name = ' ';
            if (match_info::positions[i]) {
                name = match_info::positions[i]->name;
            }
            UpdatePosition(i % 8, i / 8, name);
        }
    }
    static void UpdatePosition(int x, int y, char name) {
        std::ostringstream oss;
        oss << "SetPosition('" << x << "', '" << y << "', '" << name << "');";
        const ultralight::String command = oss.str().c_str();
        match_info::overlay_->view()->EvaluateScript(command);
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