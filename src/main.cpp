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
    std::vector<Move> promotingMoves;
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
    RefPtr<Overlay> overlay_;
public:
    ChessApp(Ref<Window> win) {
        overlay_ = Overlay::Create(win, win->width(), win->height(), 0, 0);

        overlay_->view()->set_load_listener(this);

        overlay_->view()->LoadURL("file:///board.html");
        match_info::overlay_ = overlay_;
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
        int selectedIndex = match_info::logic.pairToInd(match_info::selectedPos);
        int newIndex = match_info::logic.pairToInd(newPos);
        Figure* selectedFigure = match_info::positions[selectedIndex];
        Figure* newFig = match_info::positions[newIndex];
        

        RefreshBoard();
        if (match_info::promotingMoves.size() > 0) {
            RefreshBoard();
            Action action = match_info::promotingMoves[newPos.first][0];
            if (match_info::board.makeMove(action, match_info::board.getMovingSide())) {
                UpdatePosition(action.pos.first, action.pos.second, ' ');
                UpdatePosition(action.dest.first, action.dest.second, action.name);
                match_info::selectedPos = action.dest;
                newPos = action.dest;
            }
            match_info::promotingMoves.clear();
        }

        bool hasMove = false;
        if (selectedFigure != newFig && selectedFigure && isOnMovingSide(selectedFigure->name)) {
            auto moves = match_info::logic.getAvailableMovesForFigure(match_info::selectedPos);
            std::vector<Move> possibleMoves;
            for (const auto &move : moves) {
                for (const auto& action : move) {
                    if (action.pos == match_info::selectedPos && action.dest == newPos) {
                        possibleMoves.push_back(move);
                        break;
                    }
                }
            }
            if (possibleMoves.size() == 1) {
                for (const auto& action : possibleMoves[0]) {
                    if (match_info::board.makeMove(action, match_info::board.getMovingSide())) {
                        UpdatePosition(action.pos.first, action.pos.second, ' ');
                        UpdatePosition(action.dest.first, action.dest.second, action.name);
                        hasMove = true;
                    }
                }
            }
            else if(possibleMoves.size() > 1) {
                for (int i = 0; i < 64; ++i) {
                    auto pos = match_info::logic.indToPair(i);
                    UpdatePosition(pos.first, pos.second, ' ');
                }
                for (int i = 0; i < possibleMoves.size(); ++i) {
                    Action action = possibleMoves[i][0];
                    UpdatePosition(i, 0, action.name);
                    SetMove(i, 0);
                }
                hasMove = true;
                match_info::promotingMoves = possibleMoves;
            }
        }
        
        if (!hasMove) {
            match_info::selectedPos = newPos;
            if (newFig && isOnMovingSide(newFig->name)) {
                for (const auto& move : match_info::logic.getAvailableMovesForFigure(match_info::selectedPos)) {
                    for (const auto& action : move) {
                        SetMove(action.dest.first, action.dest.second);
                    }
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
        {
            JSStringRef name = JSStringCreateWithUTF8CString("OnTileClick");
            JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name,
                &OnTileClick);
            JSObjectRef globalObj = JSContextGetGlobalObject(ctx);
            JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);
            JSStringRelease(name);
        }

        match_info::overlay_->view()->EvaluateScript("init();");
        for (int i = 0; i < 64; ++i) {
            if (match_info::positions[i]) {
                UpdatePosition(i % 8, i / 8, match_info::positions[i]->name);
            }
        }
    }
    static void RefreshBoard() {
        for (int i = 0; i < 64; ++i) {
            char name = ' ';
            if (match_info::positions[i]) {
                name = match_info::positions[i]->name;
            }
            UpdatePosition(i % 8, i / 8, name);
        }
    }
    static bool isOnMovingSide(char name) {
        Side figSide = isupper(name) ? Side::white : Side::black;
        return figSide == match_info::board.getMovingSide();
    }
    static void UpdatePosition(int x, int y, char name) {
        std::ostringstream oss;
        oss << "SetPosition('" << x << "', '" << y << "', '" << name << "');";
        const ultralight::String command = oss.str().c_str();
        match_info::overlay_->view()->EvaluateScript(command);
    }
    static void SetMove(int x, int y) {
        std::ostringstream oss;
        oss << "SetMove('" << x << "', '" << y << "');";
        const ultralight::String command = oss.str().c_str();
        match_info::overlay_->view()->EvaluateScript(command);
    }
};

int main() {
    auto app = App::Create();
    auto window = Window::Create(app->main_monitor(), 450, 450, false, kWindowFlags_Titled);

    window->SetTitle("Online Chess");

    app->set_window(window);

    ChessApp my_app(window);
    app->Run();

    return 0;
}