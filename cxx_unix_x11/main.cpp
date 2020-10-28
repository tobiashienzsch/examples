#include <X11/Xlib.h>
#include <iostream>

#include <cstdlib>
#include <cstring>

int main(void)
{
    auto* display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Cannot open display\n";
        exit(1);
    }

    auto screen = XDefaultScreen(display);
    auto window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 1280, 720, 1,
                                      BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    while (true)
    {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose)
        {
            XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 10, 10);
            const char* msg = "Hello, World!";
            XDrawString(display, window, DefaultGC(display, screen), 10, 50, msg, strlen(msg));
        }

        if (event.type == KeyPress)
        {
            break;
        }
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}