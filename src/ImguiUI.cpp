#include "ImguiUI.hpp"


#if defined(DGL_USE_GLES2)
#   define IMGUI_IMPL_OPENGL_ES2
#elif defined(DGL_USE_GLES3)
#   define IMGUI_IMPL_OPENGL_ES3
#elif defined(DGL_USE_OPENGL3)
#   define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#if defined(DGL_USE_GLES2) || defined(DGL_USE_GLES3) || defined(DGL_USE_OPENGL3)
#   include <backends/imgui_impl_opengl3.h>
#else
#   include <backends/imgui_impl_opengl2.h>
#endif

#include <Application.hpp>
#include <Window.hpp>


static const char* GetClipboardTextFn(void* const userData) {
    size_t dataSize;
    return (const char*)((TopLevelWidget*)(userData))->getClipboard(dataSize);
}

static void SetClipboardTextFn(void* const userData, const char* const text) {
    TopLevelWidget* tlw = (TopLevelWidget*)userData;
    DISTRHO_SAFE_ASSERT_RETURN(tlw != nullptr,);
    DISTRHO_SAFE_ASSERT_RETURN(text != nullptr,);
    tlw->setClipboard(nullptr, text, std::strlen(text) + 1);
}

static ImGuiKey GetImguiKey(uint key) {
    switch (key) {
        case kKeyTab:
            return ImGuiKey_Tab;
        case kKeyLeft:
            return ImGuiKey_LeftArrow;
        case kKeyRight:
            return ImGuiKey_RightArrow;
        case kKeyUp:
            return ImGuiKey_UpArrow;
        case kKeyDown:
            return ImGuiKey_DownArrow;
        case kKeyPageUp:
            return ImGuiKey_PageUp;
        case kKeyPageDown:
            return ImGuiKey_PageDown;
        case kKeyHome:
            return ImGuiKey_Home;
        case kKeyEnd:
            return ImGuiKey_End;
        case kKeyInsert:
            return ImGuiKey_Insert;
        case kKeyDelete:
            return ImGuiKey_Delete;
        case kKeyBackspace:
            return ImGuiKey_Backspace;
        case kKeySpace:
            return ImGuiKey_Space;
        case kKeyEnter:
            return ImGuiKey_Enter;
        case kKeyEscape:
            return ImGuiKey_Escape;
        case kKeyControlL:
            return ImGuiKey_LeftCtrl;
        case kKeyShiftL:
            return ImGuiKey_LeftShift;
        case kKeyAltL:
            return ImGuiKey_LeftAlt;
        case kKeySuperL:
            return ImGuiKey_LeftSuper;
        case kKeyControlR:
            return ImGuiKey_RightCtrl;
        case kKeyShiftR:
            return ImGuiKey_RightShift;
        case kKeyAltR:
            return ImGuiKey_RightAlt;
        case kKeySuperR:
            return ImGuiKey_RightSuper;
        case kKeyMenu:
            return ImGuiKey_Menu;
        case 0x27:
            return ImGuiKey_Apostrophe;
        case 0x2C:
            return ImGuiKey_Comma;
        case 0x2D:
            return ImGuiKey_Minus;
        case 0x2E:
            return ImGuiKey_Period;
        case 0x2F:
            return ImGuiKey_Slash;
        case 0x3B:
            return ImGuiKey_Semicolon;;
        case 0x3D:
            return ImGuiKey_Equal;
        case 0x5B:
            return ImGuiKey_LeftBracket;
        case 0x5C:
            return ImGuiKey_Backslash;
        case 0x5D:
            return ImGuiKey_RightBracket;
        case 0x60:
            return ImGuiKey_GraveAccent;
        case kKeyCapsLock:
            return ImGuiKey_CapsLock;
        case kKeyScrollLock:
            return ImGuiKey_ScrollLock;
        case kKeyNumLock:
            return ImGuiKey_NumLock;
        case kKeyPrintScreen:
            return ImGuiKey_PrintScreen;
        case kKeyPause:
            return ImGuiKey_Pause;
        case kKeyPadDecimal:
            return ImGuiKey_KeypadDecimal;
        case kKeyPadDivide:
            return ImGuiKey_KeypadDivide;
        case kKeyPadMultiply:
            return ImGuiKey_KeypadMultiply;
        case kKeyPadSubtract:
            return ImGuiKey_KeypadSubtract;
        case kKeyPadAdd:
            return ImGuiKey_KeypadAdd;
        case kKeyPadEnter:
            return ImGuiKey_KeypadEnter;
        case kKeyPadEqual:
            return ImGuiKey_KeypadEqual;
        default:
            if (key >= 48 && key <= 57)
                return (ImGuiKey)(ImGuiKey_0 + key - 48);
            if (key >= 65 && key <= 90)
                return (ImGuiKey)(ImGuiKey_A + key - 65);
            if (key >= 97 && key <= 123)
                return (ImGuiKey)(ImGuiKey_A + key - 97);
            if (key >= kKeyF1 && key <= kKeyF12)
                return (ImGuiKey)(ImGuiKey_F1 + key - kKeyF1);
            if (key >= kKeyPad0 && key <= kKeyPad9)
                return (ImGuiKey)(ImGuiKey_Keypad0 + key - kKeyPad0);
            return ImGuiKey_None;
    }
}

ImguiUI::ImguiUI(uint width, uint height, bool automaticallyScaleAndSetAsMinimumSize) : 
    DISTRHO::UI(width, height, automaticallyScaleAndSetAsMinimumSize) {
    
    IMGUI_CHECKVERSION();

    context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.DisplaySize.x = getWidth();
    io.DisplaySize.y = getHeight();
    io.DisplayFramebufferScale = ImVec2{ (float)getScaleFactor(), (float)getScaleFactor() };
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    
    io.GetClipboardTextFn = GetClipboardTextFn;
    io.SetClipboardTextFn = SetClipboardTextFn;
    io.ClipboardUserData = getTopLevelWidget();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes((float)getScaleFactor());


    #if defined(DGL_USE_GLES2) || defined(DGL_USE_GLES3) || defined(DGL_USE_OPENGL3)
    ImGui_ImplOpenGL3_Init();
    #else
    ImGui_ImplOpenGL2_Init();
    #endif
}

ImguiUI::~ImguiUI() {
    ImGui::SetCurrentContext(context);
    #if defined(DGL_USE_GLES2) || defined(DGL_USE_GLES3) || defined(DGL_USE_OPENGL3)
    ImGui_ImplOpenGL3_Shutdown();
    #else
    ImGui_ImplOpenGL2_Shutdown();
    #endif
    ImGui::DestroyContext(context);
}


void ImguiUI::onDisplay() {
    const double time = getApp().getTime();
    const double delta = time - lastFrameTime;
    lastFrameTime = time;

    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = delta;

    #if defined(DGL_USE_GLES2) || defined(DGL_USE_GLES3) || defined(DGL_USE_OPENGL3)
    ImGui_ImplOpenGL3_NewFrame();
    #else
    ImGui_ImplOpenGL2_NewFrame();
    #endif

    ImGui::NewFrame();
    onImguiDisplay();
    ImGui::Render();

    if (ImDrawData* const data = ImGui::GetDrawData())
    {
        #if defined(DGL_USE_GLES2) || defined(DGL_USE_GLES3) || defined(DGL_USE_OPENGL3)
        ImGui_ImplOpenGL3_RenderDrawData(data);
        #else
        ImGui_ImplOpenGL2_RenderDrawData(data);
        #endif
    }
}

void ImguiUI::uiIdle() {
    repaint();
}

bool ImguiUI::onKeyboard(const KeyboardEvent& event) {
    if (UI::onKeyboard(event))
        return true;

    ImGuiIO& io = ImGui::GetIO();

    ImGuiKey key = GetImguiKey(event.key);
    io.AddKeyEvent(key, event.press);
    io.SetKeyEventNativeData(key, event.keycode, event.keycode);

    return io.WantCaptureKeyboard;
}

bool ImguiUI::onCharacterInput(const CharacterInputEvent& event) {
    if (UI::onCharacterInput(event))
        return true;

    ImGuiIO& io = ImGui::GetIO();

    io.AddInputCharacter(event.character);

    return io.WantCaptureKeyboard;
}

bool ImguiUI::onMouse(const MouseEvent& event) {
    if (UI::onMouse(event))
        return true;

    ImGuiIO& io = ImGui::GetIO();

    io.AddMouseButtonEvent(event.button - 1, event.press);

    return io.WantCaptureMouse;
}

bool ImguiUI::onMotion(const MotionEvent& event) {
    if (UI::onMotion(event))
        return true;

    ImGuiIO& io = ImGui::GetIO();

    io.AddMousePosEvent(event.pos.getX(), event.pos.getY());

    return false;
}

bool ImguiUI::onScroll(const ScrollEvent& event) {
    if (UI::onScroll(event))
        return true;

    ImGuiIO& io = ImGui::GetIO();

    //io.AddMouseWheelEvent(event.delta.getX(), event.delta.getY());

    io.MouseWheel += event.delta.getY();
    io.MouseWheelH += event.delta.getX();

    return io.WantCaptureMouse;
}

void ImguiUI::onResize(const ResizeEvent& event) {
    UI::onResize(event);

    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize.x = event.size.getWidth();
    io.DisplaySize.y = event.size.getHeight();
}