#include "gui_style.h"
#include "pax_gfx.h"
#include "sdkconfig.h"
#include <string.h>

gui_theme_t theme = {0};

void theme_initialize(void) {
    gui_palette_t palette = {
        .color_foreground          = 0xFF340132,
        .color_background          = 0xFFEEEAEE,
        .color_active_foreground   = 0xFF340132,
        .color_active_background   = 0xFFFFFFFF,
        .color_highlight_primary   = 0xFF01BC99,
        .color_highlight_secondary = 0xFFFFCF53,
        .color_highlight_tertiary  = 0xFFFF017F,
    };

    memcpy((void*)&theme.palette, &palette, sizeof(gui_palette_t));
    memcpy((void*)&theme.footer.palette, &palette, sizeof(gui_palette_t));
    memcpy((void*)&theme.header.palette, &palette, sizeof(gui_palette_t));
    memcpy((void*)&theme.menu.palette, &palette, sizeof(gui_palette_t));
    memcpy((void*)&theme.progressbar.palette, &palette, sizeof(gui_palette_t));
    memcpy((void*)&theme.chat.palette, &palette, sizeof(gui_palette_t));

    theme.footer.height                  = 32;
    theme.footer.vertical_margin         = 7;
    theme.footer.horizontal_margin       = 20;
    theme.footer.text_height             = 16;
    theme.footer.vertical_padding        = 20;
    theme.footer.horizontal_padding      = 0;
    theme.footer.text_font               = pax_font_sky_mono;
    theme.header.height                  = 32;
    theme.header.vertical_margin         = 7;
    theme.header.horizontal_margin       = 20;
    theme.header.text_height             = 16;
    theme.header.vertical_padding        = 20;
    theme.header.horizontal_padding      = 0;
    theme.header.text_font               = pax_font_sky_mono;
    theme.menu.height                    = 480 - 64;
    theme.menu.vertical_margin           = 20;
    theme.menu.horizontal_margin         = 30;
    theme.menu.text_height               = 16;
    theme.menu.vertical_padding          = 6;
    theme.menu.horizontal_padding        = 6;
    theme.menu.text_font                 = pax_font_sky_mono;
    theme.menu.list_entry_height         = 32;
    theme.menu.grid_horizontal_count     = 4;
    theme.menu.grid_vertical_count       = 3;
    theme.progressbar.vertical_margin    = 5;
    theme.progressbar.horizontal_margin  = 5;
    theme.progressbar.vertical_padding   = 5;
    theme.progressbar.horizontal_padding = 5;
    theme.chat.height                    = 480 - 64;
    theme.chat.vertical_margin           = 20;
    theme.chat.horizontal_margin         = 30;
    theme.chat.text_height               = 24;
    theme.chat.vertical_padding          = 6;
    theme.chat.horizontal_padding        = 6;
    theme.chat.text_font                 = pax_font_sky_mono;
    theme.chat.list_entry_height         = 64;

    theme.show_clock = true;
}

gui_theme_t* get_theme(void) {
    return (gui_theme_t*)&theme;
}
