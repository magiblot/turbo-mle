#ifndef TUI_TERMBOX_VIEW_H
#define TUI_TERMBOX_VIEW_H

#define Uses_TView
#define Uses_TKeys
#define Uses_TEvent
#include <tvision/tv.h>

#include <memory>
#include <vector>
#include <termbox.h>

class TermboxEmulator;

struct TermboxView: public TView {

    std::shared_ptr<TermboxEmulator> term;
    std::vector<ushort> drawbuf;

    TermboxView(const TRect &bounds, std::shared_ptr<TermboxEmulator> term);
    ~TermboxView();

    void resetSize();
    ushort convertCellAttr(const tb_cell &cell);
    uchar convertColor(uint16_t color, bool bg = false);

    void changeBounds(const TRect& bounds) override;
    void draw() override;
    void handleEvent(TEvent &ev) override;

};

#include "termbox_emu.h"

inline TermboxView::TermboxView(const TRect &bounds, std::shared_ptr<TermboxEmulator> _term) :
    TView(bounds),
    term(std::move(_term))
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofSelectable;
    eventMask = evMouseDown | evKeyDown | evCommand | evBroadcast;
    term->view = this;
    resetSize();
    if (term->cursorVisible) {
        auto [x, y] = term->cursor;
        setCursor(x, y);
        showCursor();
    }
}

inline TermboxView::~TermboxView()
{
    term->view = 0;
}

inline void TermboxView::resetSize()
{
    term->resize(size.x, size.y);
    drawbuf.resize(size.x);
}

inline ushort TermboxView::convertCellAttr(const tb_cell &cell)
{
    uchar ch = cell.ch; // TODO: Unicode to codepage.
    uchar fg = convertColor(cell.fg);
    uchar bg = convertColor(cell.bg, true);
    return bg << 12 | fg << 8 | ch;
}

inline uchar TermboxView::convertColor(uint16_t color, bool bg)
{
    uchar c;
    switch (color) {
        case TB_BLACK:  c = 0x00; break;
        case TB_RED:    c = 0x04; break;
        case TB_GREEN:  c = 0x02; break;
        case TB_YELLOW: c = 0x06; break;
        case TB_BLUE:   c = 0x01; break;
        case TB_MAGENTA:c = 0x05; break;
        case TB_CYAN:   c = 0x03; break;
        case TB_WHITE:  c = 0x07; break;
        default:        c = bg ? 0x00 : 0x07;
    }
    if (color & TB_BOLD)
        c |= 0x08;
    return c;
}

#endif
