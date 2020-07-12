#include "app.h"
#include "termbox_emu.h"
#include "termbox_view.h"

#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TKeys
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TSubMenu
#define Uses_TWindow
#include <tvision/tv.h>

TMleApp *app = 0;

TMleApp::TMleApp() :
    TProgInit( &TMleApp::initStatusLine,
               &TMleApp::initMenuBar,
               &TApplication::initDeskTop
             ),
    mleTerm(std::make_shared<TermboxEmulator>()),
    mleRunning(false)
{
}

TMenuBar *TMleApp::initMenuBar(TRect r)
{
    r.b.y = r.a.y+1;
    return new TMenuBar( r,
        *new TSubMenu( "~F~ile", kbAltF, hcNoContext ) +
            *new TMenuItem( "~N~ew", cmNew, kbCtrlN, hcNoContext, "Ctrl+N" ) +
            newLine() +
            *new TMenuItem( "E~x~it", cmQuit, kbNoKey, hcNoContext, "Alt-X" )
            );

}

TStatusLine *TMleApp::initStatusLine( TRect r )
{
    r.a.y = r.b.y-1;
    return new TStatusLine( r,
        *new TStatusDef( 0, 0xFFFF ) +
            *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmQuit ) +
            *new TStatusItem( 0, kbF10, cmMenu )
            );
}


void TMleApp::handleEvent(TEvent& event)
{
    TApplication::handleEvent(event);
    if (event.what == evCommand) {
        switch (event.message.command) {
            case cmNew:
                newEditorWindow();
                clearEvent(event);
                break;
            default:
                break;
        }
    }
}

void TMleApp::newEditorWindow()
{
    TWindow *w = new TWindow(deskTop->getExtent(), "Editor Window", wnNoNumber);
    w = (TWindow *) validView(w);
    if (w) {
        TRect r = w->getExtent();
        r.grow(-1, -1);
        TermboxView *v = new TermboxView(
            r,
            mleTerm.use_count() == 1 ? mleTerm
                                     : std::make_shared<TermboxEmulator>()
        );
        v = (TermboxView *) validView(v);
        if (v) {
            w->insert(v);
            deskTop->insert(w);
            if (!mleRunning) {
                mleRunning = true;
                invokeMle();
            } else {
                v->term->setText("Another instance of the application is running.");
                v->drawView();
            }
        }
    }
}

extern "C" int mle_main(int argc, char **argv);

void TMleApp::invokeMle()
{
    char name[] = "mle";
    char *argv[2] = {name, 0};
    mle_main(1, argv);
}
