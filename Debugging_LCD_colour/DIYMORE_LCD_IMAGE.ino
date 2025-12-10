#include "TFT_eSPI.h" 
#include "lvgl.h"

// 1. Declare TFT_eSPI object
TFT_eSPI tft = TFT_eSPI(); 

// 2. Define Display and Buffer Parameters
#define LVGL_HOR_RES 320
#define LVGL_VER_RES 240
#define DRAW_BUF_SIZE_PIXELS (LVGL_HOR_RES * 10) 

// 3. Declare Static Buffers
static lv_color_t buf[DRAW_BUF_SIZE_PIXELS]; 
static lv_draw_buf_t draw_buf; 

// 4. LVGL Display Flush Callback (TFT_eSPI compatible)
void lv_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    // LVGL is now handling the byte swap, so we just push the image.
    tft.pushImage(area->x1, area->y1, lv_area_get_width(area), lv_area_get_height(area), (uint16_t*)px_map);

    // Tell LVGL that the flushing is finished
    lv_display_flush_ready(disp);
}

// 5. External declaration for the image data
extern const lv_image_dsc_t Gemini_Generated_Image_320x240;


void setup() {
    Serial.begin(115200);
    delay(100);

    // --- TFT_eSPI Initialization ---
    tft.init();
    tft.setRotation(3); // Set to landscape (320x240)
    
    // CRITICAL: We DO NOT call tft.setSwapBytes(true) here!
    // This is now handled by LV_COLOR_16_SWAP 1 in lv_conf.h.

    // Enable backlight
#ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
#endif

    // --- LVGL Initialization ---
    lv_init();

    // 1. Initialize the draw buffer structure's geometry
    lv_draw_buf_init(&draw_buf, 
                      LVGL_HOR_RES,
                      10, // 10 lines height
                      LV_COLOR_FORMAT_RGB565,
                      LVGL_HOR_RES,
                      buf,
                      DRAW_BUF_SIZE_PIXELS * sizeof(lv_color_t) 
                      );

    // 2. Create and register the display driver
    lv_display_t *disp = lv_display_create(LVGL_HOR_RES, LVGL_VER_RES); 
    lv_display_set_default(disp);
    lv_display_set_flush_cb(disp, lv_disp_flush);
    
    // 3. Link the initialized draw buffer to the display
    lv_display_set_draw_buffers(disp, &draw_buf, NULL);

    // --- LVGL Content: Display the Image ---
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_scr_load(scr);

    lv_obj_t *img = lv_image_create(scr);
    
    // Set the image source 
    lv_image_set_src(img, &Gemini_Generated_Image_320x240); 
    
    // Center the image on the screen
    lv_obj_center(img);
    
    Serial.println("Setup complete. Starting LVGL loop.");
}

void loop() {
    // Must be called periodically to process LVGL tasks, timers, and animations
    lv_timer_handler(); 
    delay(5);
}