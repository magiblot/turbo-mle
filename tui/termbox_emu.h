#ifndef TUI_TERMBOX_EMU_H
#define TUI_TERMBOX_EMU_H

#define Uses_TPoint
#include <tvision/tv.h>

#include <vector>
#include <cstdio>
#include <termbox.h>

class TermboxView;

struct TermboxEmulator {

    TPoint size {0, 0};
    TermboxView *view {0};
    std::vector<tb_cell> cellbuf;

    template<class ...Args>
    void setText(Args &&...);
    void resize(int x, int y);

    tb_cell& at(int x, int y);
    void put(int x, int y, const tb_cell &cell);

};

template<class ...Args>
inline void TermboxEmulator::setText(Args &&...args)
{
    char s[256];
    int written = snprintf(s, sizeof(s), args...);
    size_t i = 0;
    for (; int(i) < written; ++i)
        cellbuf[i].ch = s[i];
    for (; i < cellbuf.size(); ++i)
        cellbuf[i].ch = '\0';
}

inline void TermboxEmulator::resize(int x, int y)
{
    cellbuf.resize(x*y);
    size = {x, y};
}

inline tb_cell& TermboxEmulator::at(int x, int y)
{
    return cellbuf[y*size.x + x];
}

inline void TermboxEmulator::put(int x, int y, const tb_cell &cell)
{
    at(x, y) = cell;
}

#endif
