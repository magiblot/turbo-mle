#ifndef TUI_TERMBOX_EMU_H
#define TUI_TERMBOX_EMU_H

#define Uses_TPoint
#include <tvision/tv.h>

#include <vector>
#include <cstdio>
#include <queue>
#include <termbox.h>

class TermboxView;

struct TermboxEmulator {

    TPoint size {0, 0};
    TermboxView *view {0};
    std::vector<tb_cell> cellbuf;
    std::queue<tb_event> eventq;
    TPoint cursor {};
    bool cursorVisible {false};

    uint16_t defaultFg {0x07};
    uint16_t defaultBg {0x00};

    template<class ...Args>
    void setText(Args &&...);
    void resize(int x, int y);
    void setCursor(int x, int y);
    void hideCursor();

    tb_cell& at(int x, int y);
    void put(int x, int y, const tb_cell &cell);
    int popEvent(tb_event &event);

};

#include "termbox_view.h"

template<class ...Args>
inline void TermboxEmulator::setText(Args &&...args)
{
    char s[256];
    int written = snprintf(s, sizeof(s), args...);
    size_t i = 0;
    for (; int(i) < written; ++i)
        cellbuf[i] = {uchar(s[i]), defaultFg, defaultBg};
    for (; i < cellbuf.size(); ++i)
        cellbuf[i] = {'\0', defaultFg, defaultBg};
}

inline void TermboxEmulator::resize(int x, int y)
{
    cellbuf.resize(x*y);
    size = {x, y};
}

inline void TermboxEmulator::setCursor(int x, int y)
{
    cursor = {x, y};
    cursorVisible = true;
    if (view) {
        view->setCursor(x, y);
        view->showCursor();
    }
}

inline void TermboxEmulator::hideCursor()
{
    cursorVisible = false;
    if (view)
        view->hideCursor();
}

inline tb_cell& TermboxEmulator::at(int x, int y)
{
    return cellbuf[y*size.x + x];
}

inline void TermboxEmulator::put(int x, int y, const tb_cell &cell)
{
    at(x, y) = cell;
}

inline int TermboxEmulator::popEvent(tb_event &event)
{
    tb_event tev = eventq.front();
    eventq.pop();
    event = tev;
    return tev.type;
}

#endif
