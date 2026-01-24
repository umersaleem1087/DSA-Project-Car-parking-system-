#include <QApplication>
#include "MainWindow.h"
#include <windows.h>

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);
        app.addLibraryPath("./plugins");
        
        MainWindow window;
        window.show();
        return app.exec();
    } catch (const std::exception& e) {
        return -1;
    } catch (...) {
        return -2;
    }
}

// Windows entry point wrapper - forward to the CRT-provided argc/argv
#ifdef _WIN32
extern "C" {
    extern int __argc;
    extern char **__argv;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
    return main(__argc, __argv);
}
#endif
