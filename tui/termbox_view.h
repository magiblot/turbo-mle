#ifndef TUI_TERMBOX_VIEW_H
#define TUI_TERMBOX_VIEW_H

#define Uses_TView
#include <tvision/tv.h>

#include <memory>
#include <vector>
#include "termbox_emu.h"

struct TermboxView: public TView {

    std::shared_ptr<TermboxEmulator> term;
    std::vector<ushort> drawbuf;

    TermboxView(const TRect &bounds, std::shared_ptr<TermboxEmulator> term);
    ~TermboxView();

    void resetSize();

    void changeBounds(const TRect& bounds) override;
    void draw() override;

};

inline TermboxView::TermboxView(const TRect &bounds, std::shared_ptr<TermboxEmulator> term) :
    TView(bounds),
    term(std::move(term))
{
    growMode = gfGrowHiX | gfGrowHiY;
    this->term->view = this;
    resetSize();
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

#endif
