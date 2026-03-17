#include "icons.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "pax_codecs.h"
#include "pax_gfx.h"

static char const TAG[] = "icons";

#define ICON_WIDTH        32
#define ICON_HEIGHT       32
#define ICON_BUFFER_SIZE  (ICON_WIDTH * ICON_HEIGHT * 4)
#define ICON_COLOR_FORMAT PAX_BUF_32_8888ARGB

// Embedded icon PNG data
extern const uint8_t icon_esc_start[]       asm("_binary_esc_png_start");
extern const uint8_t icon_esc_end[]         asm("_binary_esc_png_end");
extern const uint8_t icon_f1_start[]        asm("_binary_f1_png_start");
extern const uint8_t icon_f1_end[]          asm("_binary_f1_png_end");
extern const uint8_t icon_f2_start[]        asm("_binary_f2_png_start");
extern const uint8_t icon_f2_end[]          asm("_binary_f2_png_end");
extern const uint8_t icon_f3_start[]        asm("_binary_f3_png_start");
extern const uint8_t icon_f3_end[]          asm("_binary_f3_png_end");
extern const uint8_t icon_f4_start[]        asm("_binary_f4_png_start");
extern const uint8_t icon_f4_end[]          asm("_binary_f4_png_end");
extern const uint8_t icon_f5_start[]        asm("_binary_f5_png_start");
extern const uint8_t icon_f5_end[]          asm("_binary_f5_png_end");
extern const uint8_t icon_f6_start[]        asm("_binary_f6_png_start");
extern const uint8_t icon_f6_end[]          asm("_binary_f6_png_end");
extern const uint8_t icon_error_start[]     asm("_binary_error_png_start");
extern const uint8_t icon_error_end[]       asm("_binary_error_png_end");
extern const uint8_t icon_help_start[]      asm("_binary_help_png_start");
extern const uint8_t icon_help_end[]        asm("_binary_help_png_end");
extern const uint8_t icon_extension_start[] asm("_binary_extension_png_start");
extern const uint8_t icon_extension_end[]   asm("_binary_extension_png_end");

typedef struct {
    const uint8_t* start;
    const uint8_t* end;
} embedded_icon_t;

static const embedded_icon_t embedded_icons[ICON_LAST] = {
    [ICON_ESC]       = {icon_esc_start, icon_esc_end},
    [ICON_F1]        = {icon_f1_start, icon_f1_end},
    [ICON_F2]        = {icon_f2_start, icon_f2_end},
    [ICON_F3]        = {icon_f3_start, icon_f3_end},
    [ICON_F4]        = {icon_f4_start, icon_f4_end},
    [ICON_F5]        = {icon_f5_start, icon_f5_end},
    [ICON_F6]        = {icon_f6_start, icon_f6_end},
    [ICON_ERROR]     = {icon_error_start, icon_error_end},
    [ICON_HELP]      = {icon_help_start, icon_help_end},
    [ICON_EXTENSION] = {icon_extension_start, icon_extension_end},
};

static pax_buf_t icons[ICON_LAST] = {0};
static bool      icons_loaded     = false;

void load_icons(void) {
    if (icons_loaded) return;

    for (int i = 0; i < ICON_LAST; i++) {
        if (embedded_icons[i].start == NULL) continue;

        size_t size = embedded_icons[i].end - embedded_icons[i].start;
        void*  buffer = heap_caps_calloc(1, ICON_BUFFER_SIZE, MALLOC_CAP_SPIRAM);
        if (buffer == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for icon %d", i);
            continue;
        }
        pax_buf_init(&icons[i], buffer, ICON_WIDTH, ICON_HEIGHT, ICON_COLOR_FORMAT);
        if (!pax_insert_png_buf(&icons[i], embedded_icons[i].start, size, 0, 0, 0)) {
            pax_buf_destroy(&icons[i]);
            free(buffer);
            memset(&icons[i], 0, sizeof(pax_buf_t));
            ESP_LOGE(TAG, "Failed to decode embedded icon %d", i);
        }
    }
    icons_loaded = true;
}

void unload_icons(void) {
    for (int i = 0; i < ICON_LAST; i++) {
        if (pax_buf_get_width(&icons[i]) == 0) continue;
        uint8_t* buffer = pax_buf_get_pixels(&icons[i]);
        pax_buf_destroy(&icons[i]);
        free(buffer);
        memset(&icons[i], 0, sizeof(pax_buf_t));
    }
    icons_loaded = false;
}

pax_buf_t* get_icon(icon_t icon) {
    if (!icons_loaded) load_icons();
    if (icon < 0 || icon >= ICON_LAST) return NULL;
    if (pax_buf_get_width(&icons[icon]) == 0) return NULL;
    return &icons[icon];
}

bool get_icons_missing(void) {
    return false;
}

esp_err_t download_icons(bool delete_old_files) {
    (void)delete_old_files;
    return ESP_ERR_NOT_SUPPORTED;
}
