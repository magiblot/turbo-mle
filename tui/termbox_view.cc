#include "termbox_view.h"

void TermboxView::draw()
{
    uchar color = 0x1C; // Blue background, light red foreground.

    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x)
            drawbuf[x] = color << 8 | (uchar) term->at(x, y).ch;
        writeBuf(0, y, size.x, 1, &drawbuf[0]);
    }
}

void TermboxView::changeBounds(const TRect &bounds)
{
    setBounds(bounds);
    resetSize();
    drawView();
}
