#include "termbox_view.h"

void TermboxView::draw()
{
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x)
            drawbuf[x] = convertCellAttr(term->at(x, y));
        writeBuf(0, y, size.x, 1, &drawbuf[0]);
    }
}

void TermboxView::changeBounds(const TRect &bounds)
{
    setBounds(bounds);
    resetSize();
    drawView();

    term->eventq.push({
        .type = TB_EVENT_RESIZE,
        .w = size.x,
        .h = size.y
    });
}
