#include <QApplication>
#include "MainWindow.h"
#include <windows.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

// Windows entry point wrapper
extern "C" {
    // Define the variables that Qt's entry point expects
    int __argc;
    char **__argv;

    // Windows main function
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPSTR lpCmdLine, int nCmdShow) {
        return main(__argc, __argv);
    }
}
