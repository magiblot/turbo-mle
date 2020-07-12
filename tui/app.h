#ifndef TUI_APP_H
#define TUI_APP_H

#define Uses_TApplication
#include <tvision/tv.h>

#include <memory>

class TermboxEmulator;

struct TMleApp: public TApplication {

    // Currently we only support one instance of mle.
    std::shared_ptr<TermboxEmulator> mleTerm;
    bool mleRunning;

    void newEditorWindow();
    void invokeMle();

    TMleApp();
    static TMenuBar* initMenuBar(TRect r);
    static TStatusLine* initStatusLine(TRect r);
    void handleEvent(TEvent& event) override;

};

extern TMleApp *app;

#endif
