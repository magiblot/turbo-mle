#include "app.h"

int main()
{
    TMleApp app;
    ::app = &app;
    app.run();
    ::app = 0;
}
