#include "termbox_view.h"
#include <unordered_map>

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

const std::unordered_map<ushort, ushort> TermboxKeyCodes = {
    { kbF1,             TB_KEY_F1,          },
    { kbF2,             TB_KEY_F2,          },
    { kbF3,             TB_KEY_F3,          },
    { kbF4,             TB_KEY_F4,          },
    { kbF5,             TB_KEY_F5,          },
    { kbF6,             TB_KEY_F6,          },
    { kbF7,             TB_KEY_F7,          },
    { kbF8,             TB_KEY_F8,          },
    { kbF9,             TB_KEY_F9,          },
    { kbF10,            TB_KEY_F10,         },
    { kbF11,            TB_KEY_F11,         },
    { kbF12,            TB_KEY_F12,         },
    { kbIns,            TB_KEY_INSERT,      },
    { kbDel,            TB_KEY_DELETE       },
    { kbHome,           TB_KEY_HOME         },
    { kbEnd,            TB_KEY_END          },
    { kbPgUp,           TB_KEY_PGUP         },
    { kbPgDn,           TB_KEY_PGDN         },
    { kbUp,             TB_KEY_ARROW_UP     },
    { kbDown,           TB_KEY_ARROW_DOWN   },
    { kbLeft,           TB_KEY_ARROW_LEFT   },
    { kbRight,          TB_KEY_ARROW_RIGHT  },
    { 0x00,             TB_KEY_CTRL_2       },
    { kbCtrlA,          TB_KEY_CTRL_A       },
    { kbCtrlB,          TB_KEY_CTRL_B       },
    { kbCtrlC,          TB_KEY_CTRL_C       },
    { kbCtrlD,          TB_KEY_CTRL_D       },
    { kbCtrlE,          TB_KEY_CTRL_E       },
    { kbCtrlF,          TB_KEY_CTRL_F       },
    { kbCtrlG,          TB_KEY_CTRL_G       },
    { kbBack,           TB_KEY_CTRL_H       },
    { kbTab,            TB_KEY_CTRL_I       },
    { kbCtrlJ,          TB_KEY_CTRL_J       },
    { kbCtrlK,          TB_KEY_CTRL_K       },
    { kbCtrlL,          TB_KEY_CTRL_L       },
    { kbEnter,          TB_KEY_CTRL_M       },
    { kbCtrlN,          TB_KEY_CTRL_N       },
    { kbCtrlO,          TB_KEY_CTRL_O       },
    { kbCtrlP,          TB_KEY_CTRL_P       },
    { kbCtrlQ,          TB_KEY_CTRL_Q       },
    { kbCtrlR,          TB_KEY_CTRL_R       },
    { kbCtrlS,          TB_KEY_CTRL_S       },
    { kbCtrlT,          TB_KEY_CTRL_T       },
    { kbCtrlU,          TB_KEY_CTRL_U       },
    { kbCtrlV,          TB_KEY_CTRL_V       },
    { kbCtrlW,          TB_KEY_CTRL_W       },
    { kbCtrlX,          TB_KEY_CTRL_X       },
    { kbCtrlY,          TB_KEY_CTRL_Y       },
    { kbCtrlZ,          TB_KEY_CTRL_Z       },
    { 0x1C,             TB_KEY_CTRL_4       },
    { 0x1D,             TB_KEY_CTRL_5       },
    { 0x1E,             TB_KEY_CTRL_6       },
    { 0x1F,             TB_KEY_CTRL_7       },
    { kbEsc,            TB_KEY_ESC          },
    { ' ',              TB_KEY_SPACE        }
};

void TermboxView::handleEvent(TEvent &ev)
{
    TView::handleEvent(ev);
    tb_event tev {};
    switch (ev.what) {
        case evKeyDown:
            if ( ev.keyDown.charScan.charCode == 9 ||
                 (ev.keyDown.charScan.charCode >= 32 && ev.keyDown.charScan.charCode < 255) )
            {
                tev.type = TB_EVENT_KEY;
                tev.ch = uchar(ev.keyDown.charScan.charCode);
                if (ev.keyDown.controlKeyState & kbAltShift)
                    tev.mod |= TB_MOD_ALT;
                clearEvent(ev);
            }
            else {
                auto it = TermboxKeyCodes.find(ev.keyDown.keyCode);
                if (it != TermboxKeyCodes.end()) {
                    tev.type = TB_EVENT_KEY;
                    tev.key = it->second;
                    clearEvent(ev);
                }
            }
    }
    if (tev.type)
        term->eventq.push(tev);
}
