#include "app.h"
#include "termbox_emu.h"
#include "termbox_view.h"
#include <termbox.h>

#define Uses_TWindow
#include <tvision/tv.h>

extern "C" int tb_init(void)
{
    return 0;
}

extern "C" void tb_shutdown(void)
{
    if (app->mleTerm->view) {
        TWindow *w = (TWindow *) app->mleTerm->view->owner;
        w->flags |= wfClose;
        message(w, evCommand, cmClose, 0);
    }
}

extern "C" int tb_width(void)
{
    return app->mleTerm->size.x;
}

extern "C" int tb_height(void)
{
    return app->mleTerm->size.y;
}

extern "C" void tb_clear(void)
{
    app->mleTerm->setText("");
}

extern "C" void tb_set_clear_attributes(uint16_t fg, uint16_t bg)
{
}

extern "C" void tb_present(void)
{
    if (app->mleTerm->view)
        app->mleTerm->view->drawView();
}

extern "C" void tb_set_cursor(int cx, int cy)
{
    if (cx == TB_HIDE_CURSOR && cy == TB_HIDE_CURSOR)
        app->mleTerm->hideCursor();
    else
        app->mleTerm->setCursor(cx, cy);
}

extern "C" void tb_put_cell(int x, int y, const struct tb_cell *cell)
{
    app->mleTerm->put(x, y, *cell);
}

extern "C" void tb_change_cell(int x, int y, uint32_t ch, uint16_t fg, uint16_t bg)
{
    app->mleTerm->put(x, y, {ch, fg, bg});
}

extern "C" struct tb_cell *tb_cell_buffer(void)
{
    return &app->mleTerm->at(0, 0);
}

extern "C" int tb_select_input_mode(int mode)
{
    return TB_INPUT_ESC;
}

extern "C" int tb_select_output_mode(int mode)
{
    return TB_OUTPUT_NORMAL;
}

extern "C" int tb_peek_event(struct tb_event *event, int timeout)
{
    if (!app->mleTerm->eventq.empty()) {
        return app->mleTerm->popEvent(*event);
    }
    return 0;
}

extern "C" int tb_poll_event(struct tb_event *event)
{
    while (app->mleTerm->eventq.empty()) {
        TEvent e = {};
        app->getEvent(e);
        app->handleEvent(e);
    }
    return app->mleTerm->popEvent(*event);
}
