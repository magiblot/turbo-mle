#include "app.h"
#include "termbox_emu.h"
#include "termbox_view.h"
#include <termbox.h>

extern "C" int tb_init(void)
{
    return 0;
}

extern "C" void tb_shutdown(void)
{
    app->mleTerm->setText("tb_shutdown() was invoked.");
    if (app->mleTerm->view)
        app->mleTerm->view->drawView();
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
    TView *view = app->mleTerm->view;
    if (view) {
        if (cx == TB_HIDE_CURSOR && cy == TB_HIDE_CURSOR)
                view->hideCursor();
        else {
            view->setCursor(cx, cy);
            if (!view->getState(sfCursorVis))
                view->showCursor();
        }
    }
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
    return 0;
}

extern "C" int tb_select_output_mode(int mode)
{
    return TB_OUTPUT_NORMAL;
}

extern "C" int tb_peek_event(struct tb_event *event, int timeout)
{
    return -1;
}

extern "C" int tb_poll_event(struct tb_event *event)
{
    TEvent e;
    do {
        e = {};
        app->getEvent(e);
        app->handleEvent(e);
    } while (e.what == evNothing);
    return -1;
}
