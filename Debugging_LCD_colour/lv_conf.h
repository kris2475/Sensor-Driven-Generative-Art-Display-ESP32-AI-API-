/**
 * @file lv_conf.h
 * Configuration file for v9.4.0 (Exhaustive Template)
 */

/*
 * Copy this file as `lv_conf.h`
 * 1. simply next to `lvgl` folder
 * 2. or to any other place and
 * - define `LV_CONF_INCLUDE_SIMPLE`;
 * - add the path as an include path.
 */

/* clang-format off */
#if 1 /* Set this to "1" to enable content */

#ifndef LV_CONF_H
#define LV_CONF_H

/* If you need to include anything here, do it inside the `__ASSEMBLY__` guard */
#if  0 && defined(__ASSEMBLY__)
#include "my_include.h"
#endif

/*====================
   COLOR SETTINGS
 *====================*/

/** Color depth: 1 (I1), 8 (L8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888) */
#define LV_COLOR_DEPTH 16

// *******************************************************************
// ********** CRITICAL FIX FOR DISPLAY NOISE/SCRAMBLE **************
// *******************************************************************
// This swaps the bytes of 16-bit colors (RGB565 -> BGR565) at the LVGL core layer.
// This is essential when the redundant tft.setSwapBytes(true) is removed from the .ino file.
#define LV_COLOR_16_SWAP 1 
// *******************************************************************

/** Swap the 2 bytes of RGB565 color (e.g. for big endian systems) */
#define LV_COLOR_RAW_BYTES_SWAP 0

/*=========================\
   STDLIB WRAPPER SETTINGS
 *=========================*/

/** Possible values
 * - LV_STDLIB_BUILTIN:     LVGL's built in implementation
 * - LV_STDLIB_CLIB:        Standard C functions, like malloc, strlen, etc
 * - LV_STDLIB_MICROPYTHON: MicroPython implementation
 * - LV_STDLIB_RTTHREAD:    RT-Thread implementation
 * - LV_STDLIB_CUSTOM:      Implement the functions externally
 */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN

/** Possible values
 * - LV_STDLIB_BUILTIN:     LVGL's built in implementation
 * - LV_STDLIB_CLIB:        Standard C functions, like malloc, strlen, etc
 * - LV_STDLIB_MICROPYTHON: MicroPython implementation
 * - LV_STDLIB_RTTHREAD:    RT-Thread implementation
 * - LV_STDLIB_CUSTOM:      Implement the functions externally
 */
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN

/** Possible values
 * - LV_STDLIB_BUILTIN:     LVGL's built in implementation
 * - LV_STDLIB_CLIB:        Standard C functions, like malloc, strlen, etc
 * - LV_STDLIB_MICROPYTHON: MicroPython implementation
 * - LV_STDLIB_RTTHREAD:    RT-Thread implementation
 * - LV_STDLIB_CUSTOM:      Implement the functions externally
 */
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_CLIB

#define LV_USE_STDLIB_QSORT     LV_STDLIB_BUILTIN

#define LV_STDINT_INCLUDE       <stdint.h>
#define LV_STDDEF_INCLUDE       <stddef.h>
#define LV_STDBOOL_INCLUDE      <stdbool.h>
#define LV_INTTYPES_INCLUDE     <inttypes.h>
#define LV_LIMITS_INCLUDE       <limits.h>
#define LV_STDARG_INCLUDE       <stdarg.h>

#if LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    /** Size of memory available for `lv_malloc()` in bytes (>= 2kB) */
    #define LV_MEM_SIZE (64 * 1024U)          /**< [bytes] */

    /** Size of the memory expand for `lv_malloc()` in bytes */
    #define LV_MEM_POOL_EXPAND_SIZE 0

    /** Set an address for the memory pool instead of allocating it as a normal array. Can be in external SRAM too. */
    #define LV_MEM_ADR 0     /**< 0: unused*/
    /* Instead of an address give a memory allocator that will be called to get a memory pool for LVGL. E.g. my_malloc */
    #if LV_MEM_ADR == 0
        #undef LV_MEM_POOL_INCLUDE
        #undef LV_MEM_POOL_ALLOC
    #endif
#endif  /*LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN*/

/*====================
   HAL SETTINGS
 *====================*/

/** Default display refresh, input device read and animation step period. */
#define LV_DEF_REFR_PERIOD  33      /**< [ms] */

/** Default Dots Per Inch. Used to initialize default sizes such as widgets sized, style paddings.
 * (Not so important, you can adjust it to modify default sizes and spaces.) */
#define LV_DPI_DEF 130              /**< [px/inch] */

/*=================
 * OPERATING SYSTEM
 *=================*/
/** Select operating system to use. Possible options:
 * - LV_OS_NONE
 * - LV_OS_PTHREAD
 * - LV_OS_FREERTOS
 * - LV_OS_CMSIS_RTOS2
 * - LV_OS_RTTHREAD
 * - LV_OS_WINDOWS
 * - LV_OS_MQX
 * - LV_OS_SDL2
 * - LV_OS_CUSTOM */
#define LV_USE_OS   LV_OS_NONE

#if LV_USE_OS == LV_OS_CUSTOM
    #define LV_OS_CUSTOM_INCLUDE <stdint.h>
#endif
#if LV_USE_OS == LV_OS_FREERTOS
    #define LV_USE_FREERTOS_TASK_NOTIFY 1
#endif

/*====================
   LV_TICK_CUSTOM
 *====================*/

/** Use a custom tick source that tells the elapsed time to LVGL. */
#define LV_TICK_CUSTOM 0
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE  <Arduino.h>
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR  (millis())
#endif


/*========================
 * RENDERING CONFIGURATION
 *========================*/

/** Align stride of all layers and images to this bytes */
#define LV_DRAW_BUF_STRIDE_ALIGN                1

/** Align start address of draw_buf addresses to this bytes*/
#define LV_DRAW_BUF_ALIGN                       4

/** Using matrix for transformations.
 * Requirements:
 * - `LV_USE_MATRIX = 1`.
 * - Rendering engine needs to support 3x3 matrix transformations. */
#define LV_DRAW_TRANSFORM_USE_MATRIX            0

/** The target buffer size for simple layer chunks. */
#define LV_DRAW_LAYER_SIMPLE_BUF_SIZE    (24 * 1024)    /**< [bytes]*/

/* Limit the max allocated memory for simple and transformed layers.
 * It should be at least `LV_DRAW_LAYER_SIMPLE_BUF_SIZE` sized but if transformed layers are also used
 * it should be enough to store the largest widget too (width x height x 4 area).
 * Set it to 0 to have no limit. */
#define LV_DRAW_LAYER_MAX_MEMORY 0  /**< No limit by default [bytes]*/

/** Stack size of drawing thread.
 * NOTE: If FreeType or ThorVG is enabled, it is recommended to set it to 32KB or more.
 */
#define LV_DRAW_THREAD_STACK_SIZE    (8 * 1024)         /**< [bytes]*/

/** Thread priority of the drawing task.
 * Higher values mean higher priority.
 * Can use values from lv_thread_prio_t enum in lv_os.h: LV_THREAD_PRIO_LOWEST,
 * LV_THREAD_PRIO_LOW, LV_THREAD_PRIO_MID, LV_THREAD_PRIO_HIGH, LV_THREAD_PRIO_HIGHEST
 * Make sure the priority value aligns with the OS-specific priority levels.
 * On systems with limited priority levels (e.g., FreeRTOS), a higher value can improve
 * rendering performance but might cause other tasks to starve. */
#define LV_DRAW_THREAD_PRIO LV_THREAD_PRIO_HIGH

#define LV_USE_DRAW_SW 1
#if LV_USE_DRAW_SW == 1
    /*
     * Selectively disable color format support in order to reduce code size.
     */
    #define LV_DRAW_SW_SUPPORT_RGB565       1
    #define LV_DRAW_SW_SUPPORT_RGB565_SWAPPED       1
    #define LV_DRAW_SW_SUPPORT_RGB565A8     1
    #define LV_DRAW_SW_SUPPORT_RGB888       1
    #define LV_DRAW_SW_SUPPORT_XRGB8888     1
    #define LV_DRAW_SW_SUPPORT_ARGB8888     1
    #define LV_DRAW_SW_SUPPORT_ARGB8888_PREMULTIPLIED 1
    #define LV_DRAW_SW_SUPPORT_L8           1
    #define LV_DRAW_SW_SUPPORT_AL88         1
    #define LV_DRAW_SW_SUPPORT_A8           1
    #define LV_DRAW_SW_SUPPORT_I1           1

    /* The threshold of the luminance to consider a pixel as
     * active in indexed color format */
    #define LV_DRAW_SW_I1_LUM_THRESHOLD 127

    /** Set number of draw units.
     * - > 1 requires operating system to be enabled in `LV_USE_OS`.
     * - > 1 means multiple threads will render the screen in parallel. */
    #define LV_DRAW_SW_DRAW_UNIT_CNT    1

    /** Use Arm-2D to accelerate software (sw) rendering. */
    #define LV_USE_DRAW_ARM2D_SYNC      0

    /** Enable native helium assembly to be compiled. */
    #define LV_USE_NATIVE_HELIUM_ASM    0

    /**
     * - 0: Use a simple renderer capable of drawing only simple rectangles with gradient, images, text, and straight lines only.
     * - 1: Use a complex renderer capable of drawing rounded corners, shadow, skew lines, and arcs too. */
    #define LV_DRAW_SW_COMPLEX          1

    #if LV_DRAW_SW_COMPLEX == 1
        /** Allow buffering some shadow calculation. */
        #define LV_DRAW_SW_SHADOW_CACHE_SIZE 0

        /** Set number of maximally-cached circle data. */
        #define LV_DRAW_SW_CIRCLE_CACHE_SIZE 4
    #endif

    #define  LV_USE_DRAW_SW_ASM     LV_DRAW_SW_ASM_NONE

    #if LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_CUSTOM
        #define  LV_DRAW_SW_ASM_CUSTOM_INCLUDE ""
    #endif

    /** Enable drawing complex gradients in software: linear at an angle, radial or conical */
    #define LV_USE_DRAW_SW_COMPLEX_GRADIENTS    0

#endif

/*Use TSi's aka (Think Silicon) NemaGFX */
#define LV_USE_NEMA_GFX 0

/** Use NXP's PXP on iMX RTxxx platforms. */
#define LV_USE_PXP 0

/** Use NXP's G2D on MPU platforms. */
#define LV_USE_G2D 0

/** Use Renesas Dave2D on RA  platforms. */
#define LV_USE_DRAW_DAVE2D 0

/** Draw using cached SDL textures*/
#define LV_USE_DRAW_SDL 0

/** Use VG-Lite GPU. */
#define LV_USE_DRAW_VG_LITE 0

/** Accelerate blends, fills, etc. with STM32 DMA2D */
#define LV_USE_DRAW_DMA2D 0

/** Draw using cached OpenGLES textures. Requires LV_USE_OPENGLES */
#define LV_USE_DRAW_OPENGLES 0

/** Draw using espressif PPA accelerator */
#define LV_USE_PPA  0

/* Use EVE FT81X GPU. */
#define LV_USE_DRAW_EVE 0

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

/*-------------
 * Logging
 *-----------*/

/** Enable log module */
#define LV_USE_LOG 1
#if LV_USE_LOG
    /** Set value to one of the following levels of logging detail: */
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

    /** - 1: Print log with 'printf';
     * - 0: User needs to register a callback with `lv_log_register_print_cb()`. */
    #define LV_LOG_PRINTF 0

    /** - 1: Enable printing timestamp;
     * - 0: Disable printing timestamp. */
    #define LV_LOG_USE_TIMESTAMP 1

    /** - 1: Print file and line number of the log;
     * - 0: Do not print file and line number of the log. */
    #define LV_LOG_USE_FILE_LINE 1

    /* Enable/disable LV_LOG_TRACE in modules that produces a huge number of logs. */
    #define LV_LOG_TRACE_MEM        1   /**< Enable/disable trace logs in memory operations. */
    #define LV_LOG_TRACE_TIMER      1   /**< Enable/disable trace logs in timer operations. */
    #define LV_LOG_TRACE_INDEV      1   /**< Enable/disable trace logs in input device operations. */
    #define LV_LOG_TRACE_DISP_REFR  1   /**< Enable/disable trace logs in display re-draw operations. */
    #define LV_LOG_TRACE_EVENT      1   /**< Enable/disable trace logs in event dispatch logic. */
    #define LV_LOG_TRACE_OBJ_CREATE 1   /**< Enable/disable trace logs in object creation (core `obj` creation plus every widget). */
    #define LV_LOG_TRACE_LAYOUT     1   /**< Enable/disable trace logs in flex- and grid-layout operations. */
    #define LV_LOG_TRACE_ANIM       1   /**< Enable/disable trace logs in animation logic. */
    #define LV_LOG_TRACE_CACHE      1   /**< Enable/disable trace logs in cache operations. */
#endif  /*LV_USE_LOG*/

/*-------------
 * Asserts
 *-----------*/

/* Enable assertion failures if an operation fails or invalid data is found. */
#define LV_USE_ASSERT_NULL          1   /**< Check if the parameter is NULL. (Very fast, recommended) */
#define LV_USE_ASSERT_MALLOC        1   /**< Checks is the memory is successfully allocated or no. (Very fast, recommended) */
#define LV_USE_ASSERT_STYLE         0   /**< Check if the styles are properly initialized. (Very fast, recommended) */
#define LV_USE_ASSERT_MEM_INTEGRITY 0   /**< Check the integrity of `lv_mem` after critical operations. (Slow) */
#define LV_USE_ASSERT_OBJ           0   /**< Check the object's type and existence (e.g. not deleted). (Slow) */

/** Add a custom handler when assert happens e.g. to restart MCU. */
#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER while(1);     /**< Halt by default */

/*-------------
 * Debug
 *-----------*/

/** 1: Draw random colored rectangles over the redrawn areas. */
#define LV_USE_REFR_DEBUG 0

/** 1: Draw a red overlay for ARGB layers and a green overlay for RGB layers*/
#define LV_USE_LAYER_DEBUG 0

/** 1: Adds the following behaviors for debugging: */
#define LV_USE_PARALLEL_DRAW_DEBUG 0

/*-------------
 * Others
 *-----------*/

#define LV_ENABLE_GLOBAL_CUSTOM 0

/** Default cache size in bytes. */
#define LV_CACHE_DEF_SIZE       0

/** Default number of image header cache entries. The cache is used to store the headers of images */
#define LV_IMAGE_HEADER_CACHE_DEF_CNT 0

/** Number of stops allowed per gradient. Increase this to allow more stops. */
#define LV_GRADIENT_MAX_STOPS   2

/** Adjust color mix functions rounding. */
#define LV_COLOR_MIX_ROUND_OFS  0

/** Add 2 x 32-bit variables to each `lv_obj_t` to speed up getting style properties */
#define LV_OBJ_STYLE_CACHE      0

/** Add `id` field to `lv_obj_t` */
#define LV_USE_OBJ_ID           0

/** Enable support widget names*/
#define LV_USE_OBJ_NAME         0

/** Automatically assign an ID when obj is created */
#define LV_OBJ_ID_AUTO_ASSIGN   LV_USE_OBJ_ID

/** Use builtin obj ID handler functions: */
#define LV_USE_OBJ_ID_BUILTIN   1

/** Use obj property set/get API. */
#define LV_USE_OBJ_PROPERTY 0

/** Enable property name support. */
#define LV_USE_OBJ_PROPERTY_NAME 1

/* Enable the multi-touch gesture recognition feature */
/* Gesture recognition requires the use of floats */
#define LV_USE_GESTURE_RECOGNITION 0

/*=====================
 * COMPILER SETTINGS
 *====================*/

/** For big endian systems set to 1 */
#define LV_BIG_ENDIAN_SYSTEM 0

/** Define a custom attribute for `lv_tick_inc` function */
#define LV_ATTRIBUTE_TICK_INC

/** Define a custom attribute for `lv_timer_handler` function */
#define LV_ATTRIBUTE_TIMER_HANDLER

/** Define a custom attribute for `lv_display_flush_ready` function */
#define LV_ATTRIBUTE_FLUSH_READY

/** Align VG_LITE buffers on this number of bytes. */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1

/** Will be added where memory needs to be aligned (with -Os data might not be aligned to boundary by default). */
#define LV_ATTRIBUTE_MEM_ALIGN

/** Attribute to mark large constant arrays, for example for font bitmaps */
#define LV_ATTRIBUTE_LARGE_CONST

/** Compiler prefix for a large array declaration in RAM */
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/** Place performance critical functions into a faster memory (e.g RAM) */
#define LV_ATTRIBUTE_FAST_MEM

/** Export integer constant to binding. */
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning  /**< The default value just prevents GCC warning */

/** Prefix all global extern data with this */
#define LV_ATTRIBUTE_EXTERN_DATA

/** Use `float` as `lv_value_precise_t` */
#define LV_USE_FLOAT            0

/** Enable matrix support
 * - Requires `LV_USE_FLOAT = 1` */
#define LV_USE_MATRIX           0

/** Include `lvgl_private.h` in `lvgl.h` to access internal data and functions by default */
#ifndef LV_USE_PRIVATE_API
    #define LV_USE_PRIVATE_API  0
#endif

/*==================
 * FONT USAGE
 *===================*/

/* Montserrat fonts with ASCII range and some symbols using bpp = 4
 * https://fonts.google.com/specimen/Montserrat */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/*
 * More font settings
 */

/** Enable built-in symbols such as LV_SYMBOL_NEW_LINE */
#define LV_FONT_USE_LTTR                1

/** Set custom font */
#define LV_FONT_CUSTOM_DECLARE

/** The maximum number of hor. res. ticks per character in an unscrrolled label. */
#define LV_TXT_ENC_PAGES 0

/** Use FreeType to render fonts */
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
    #define LV_FREETYPE_CACHE_SIZE          524288   /**< [bytes] */
    #define LV_FREETYPE_CACHE_MEASURE       0
    #define LV_FREETYPE_FREE_ON_CLOSE       0
    #define LV_FREETYPE_GCM_ENGINE          0
    #define LV_FREETYPE_SBIT_ENGINE         0
#endif

/*===================
 * TEXT SETTINGS
 *==================*/

/** Max length of a row to cache on a line break. */
#define LV_TXT_LINE_BREAK_LONG_LEN 0

/** Minimum number of characters in a long word to put on a line before a break. */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3

/** Minimum number of characters in a long word to put on a line after a break. */
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/** Support bidirectional text. Allows mixing Left-to-Right and Right-to-Left text. */
#define LV_USE_BIDI 0
#if LV_USE_BIDI
/*Set the default direction. Supported values:
 *`LV_BASE_DIR_LTR` Left-to-Right
 *`LV_BASE_DIR_RTL` Right-to-Left
 *`LV_BASE_DIR_AUTO` detect text base direction*/
#define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif

/** Enable Arabic/Persian processing
 * In these languages characters should be replaced with another form based on their position in the text */
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*The control character to use for signaling text recoloring*/
#define LV_TXT_COLOR_CMD "#"

/*==================
 * WIDGETS
 *================*/

/** 1: Causes these widgets to be given default values at creation time. */
#define LV_WIDGETS_INIT_DEFAULT 0

#define LV_USE_ARC 1
#define LV_USE_ANIMIMG 1
#define LV_USE_BAR 1
#define LV_USE_BUTTON 1
#define LV_USE_BUTTONMATRIX 1
#define LV_USE_CALENDAR 1
#define LV_USE_CANVAS 1
#define LV_USE_CHECKBOX 1
#define LV_USE_COLORWHEEL 1
#define LV_USE_CHART 1
#define LV_USE_DROPDOWN 1
#define LV_USE_GRIDNAV 0
#define LV_USE_IMAGECENTER 1
#define LV_USE_IMAGE 1 /**< Requires: lv_label */
#define LV_USE_IMAGEBUTTON 1
#define LV_USE_KEYBOARD 1
#define LV_USE_LABEL 1
#if LV_USE_LABEL
    #define LV_LABEL_TEXT_SELECTION 1    /**< Enable selecting text of the label */
    #define LV_LABEL_LONG_TXT_HINT 1     /**< Store some extra info in labels to speed up drawing of very long text */
    #define LV_LABEL_WAIT_CHAR_COUNT 3   /**< The count of wait chart */
#endif
#define LV_USE_LED 1
#define LV_USE_LINE 1
#define LV_USE_LIST 1
#define LV_USE_LOTTIE 0 /**< Requires: lv_canvas, thorvg */
#define LV_USE_MENU 1
#define LV_USE_MSGBOX 1
#define LV_USE_ROLLER 1 /**< Requires: lv_label */
#define LV_USE_SCALE 1
#define LV_USE_SLIDER 1 /**< Requires: lv_bar */
#define LV_USE_SPAN 1
#if LV_USE_SPAN
    /** A line of text can contain this maximum number of span descriptors. */
    #define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif
#define LV_USE_SPINBOX 1
#define LV_USE_SPINNER 1
#define LV_USE_SWITCH 1
#define LV_USE_TABLE 1
#define LV_USE_TABVIEW 1
#define LV_USE_TEXTAREA 1 /**< Requires: lv_label */
#if LV_USE_TEXTAREA != 0
    #define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500 /**< [ms] */
#endif
#define LV_USE_TILEVIEW 1
#define LV_USE_WIN 1

/*==================
 * EXTRAS
 *==================*/

/* Extend the LVGL features with these extensions */
#define LV_USE_THEME_DEFAULT 1 /**< Enable the default theme */
#define LV_USE_THEME_MONO 1    /**< Enable the monochrome theme */
#define LV_USE_FLEX 1          /**< Enable flex layout*/
#define LV_USE_GRID 1          /**< Enable grid layout*/

/*===================
 * NON-DISPLAYS
 *==================*/

/* Input devices */
#define LV_USE_INDEV_DRIVER 1  /**< Enable the general indev driver interface */
#define LV_USE_DRAG_AND_DROP 1 /**< Enable drag and drop of objects and the `LV_EVENT_DROP_BEGIN/END` events */
#define LV_USE_POINTER 1       /**< Enable pointer handling with `lv_indev_get_point` */

/* Others */

/** Enable the file system API*/
#define LV_USE_FS_API 1

/** API for standard C file functions, like fopen, fread, etc. */
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
    #define LV_FS_STDIO_LETTER '\0' /**< Set an upper-case driver-identifier letter for this driver (e.g. 'A'). */
    #define LV_FS_STDIO_PATH ""     /**< Set the working directory. File/directory paths will be appended to it. */
    #define LV_FS_STDIO_CACHE_SIZE 0 /**< >0 to cache this number of bytes in lv_fs_read() */
#endif

/** API for open, read, etc. */
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
    #define LV_FS_POSIX_LETTER '\0' /**< Set an upper-case driver-identifier letter for this driver (e.g. 'A'). */
    #define LV_FS_POSIX_PATH ""     /**< Set the working directory. File/directory paths will be appended to it. */
    #define LV_FS_POSIX_CACHE_SIZE 0 /**< >0 to cache this number of bytes in lv_fs_read() */
#endif

/** API for CreateFile, ReadFile, etc. */
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
    #define LV_FS_WIN32_LETTER '\0' /**< Set an upper-case driver-identifier letter for this driver (e.g. 'A'). */
    #define LV_FS_WIN32_PATH ""     /**< Set the working directory. File/directory paths will be appended to it. */
    #define LV_FS_WIN32_CACHE_SIZE 0 /**< >0 to cache this number of bytes in lv_fs_read() */
#endif

/** API for FATFS (needs to be added separately). Supports only 8.3 file names. */
#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
    #define LV_FS_FATFS_LETTER '\0' /**< Set an upper-case driver-identifier letter for this driver (e.g. 'A'). */
    #define LV_FS_FATFS_CACHE_SIZE 0 /**< >0 to cache this number of bytes in lv_fs_read() */
#endif

/** API for LittleFS. */
#define LV_USE_FS_LITTLEFS 0
#if LV_USE_FS_LITTLEFS
    #define LV_FS_LITTLEFS_LETTER '\0' /**< Set an upper-case driver-identifier letter for this driver (e.g. 'A'). */
    #define LV_FS_LITTLEFS_CACHE_SIZE 0 /**< >0 to cache this number of bytes in lv_fs_read() */
#endif

/** API for SDCARD via Arduino SD.h */
#define LV_USE_FS_ARDUINO_SD 0
#if LV_USE_FS_ARDUINO_SD
    #define LV_FS_ARDUINO_SD_LETTER '\0' /**< Set an upper-case driver-identifier letter for this driver (e.g. 'A'). */
    #define LV_FS_ARDUINO_SD_PATH "" /**< Set the working directory. File/directory paths will be appended to it. */
#endif

/** API for UEFI */
#define LV_USE_FS_UEFI 0
#if LV_USE_FS_UEFI
    #define LV_FS_UEFI_LETTER '\0' /**< Set an upper-case driver-identifier letter for this driver (e.g. 'A'). */
#endif

#define LV_USE_FS_FROGFS 0
#if LV_USE_FS_FROGFS
    #define LV_FS_FROGFS_LETTER '\0'
#endif

/** LODEPNG decoder library */
#define LV_USE_LODEPNG 0

/** PNG decoder(libpng) library */
#define LV_USE_LIBPNG 0

/** BMP decoder library */
#define LV_USE_BMP 0

/** JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#define LV_USE_TJPGD 0

/** libjpeg-turbo decoder library.
 * - Supports complete JPEG specifications and high-performance JPEG decoding. */
#define LV_USE_LIBJPEG_TURBO 0

/** GIF decoder library */
#define LV_USE_GIF 0
#if LV_USE_GIF
    /** GIF decoder accelerate */
    #define LV_GIF_CACHE_DECODE_DATA 0
#endif

/** GStreamer library */
#define LV_USE_GSTREAMER 0

/** Decode bin images to RAM */
#define LV_BIN_DECODER_RAM_LOAD 0

/** RLE decompress library */
#define LV_USE_RLE 0

/** QR code library */
#define LV_USE_QRCODE 0

/** Barcode code library */
#define LV_USE_BARCODE 0

/** FreeType library */
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
    #define LV_FREETYPE_CACHE_SIZE 524288   /**< [bytes] */
    #define LV_FREETYPE_CACHE_MEASURE 0
    #define LV_FREETYPE_FREE_ON_CLOSE 0
    #define LV_FREETYPE_GCM_ENGINE 0
    #define LV_FREETYPE_SBIT_ENGINE 0
#endif

/** Tiny render library for vectors.
 * - Requires: `LV_USE_FLOAT = 1` */
#define LV_USE_THORVG 0

/** Tiny vector graphics renderer
 * - Requires: `LV_USE_FLOAT = 1` */
#define LV_USE_SVG 0
#define LV_USE_SVG_ANIMATION 0
#define LV_USE_SVG_DEBUG 0

/** FFmpeg library for image decoding and playing videos.
 * Supports all major image formats so do not enable other image decoder with it. */
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
    /** Dump input information to stderr */
    #define LV_FFMPEG_DUMP_FORMAT 0
    /** Use lvgl file path in FFmpeg Player widget
     * You won't be able to open URLs after enabling this feature.
     * Note that FFmpeg image decoder will always use lvgl file system. */
    #define LV_FFMPEG_PLAYER_USE_LV_FS 0
#endif

/*==================
 * OTHERS
 *==================*/

/** 1: Enable API to take snapshot for object */
#define LV_USE_SNAPSHOT 0

/** 1: Enable system monitor component */
#define LV_USE_SYSMON 0
#if LV_USE_SYSMON
    /** Get the idle percentage. E.g. uint32_t my_get_idle(void); */
    #define LV_SYSMON_GET_IDLE lv_os_get_idle_percent
    /** 1: Enable usage of lv_os_get_proc_idle_percent.*/
    #define LV_SYSMON_PROC_IDLE_AVAILABLE 0
    #if LV_SYSMON_PROC_IDLE_AVAILABLE
        /** Get the applications idle percentage.
         * - Requires `LV_USE_OS == LV_OS_PTHREAD` */
        #define LV_SYSMON_GET_PROC_IDLE lv_os_get_proc_idle_percent
    #endif
    /** 1: Show CPU usage and FPS count.
     * - Requires `LV_USE_PERF_MONITOR` */
    #define LV_SYSMON_USE_COMPLEX 0
#endif

/** 1: Enable the performance monitor.
 * - Requires `LV_USE_LOG` */
#define LV_USE_PERF_MONITOR 0
#if LV_USE_PERF_MONITOR
    #define LV_PERF_MONITOR_POS LV_DIR_BOTTOM_RIGHT
    #define LV_PERF_MONITOR_MAX_CPU 100
    #define LV_PERF_MONITOR_MAX_FPS 60
    #define LV_PERF_MONITOR_LOG_LEVEL LV_LOG_LEVEL_WARN
#endif

/** 1: Enable the memory monitor */
#define LV_USE_MEM_MONITOR 0
#if LV_USE_MEM_MONITOR
    #define LV_MEM_MONITOR_POS LV_DIR_BOTTOM_LEFT
#endif

/** 1: Enable the color picker widget.
 * - Requires: lv_slider, lv_roller, lv_canvas, lv_label, lv_btn */
#define LV_USE_COLOR_PICKER 0

/** 1: Enable the benchmark widget
 * - Requires: lv_list, lv_label, lv_checkbox, lv_textarea, lv_slider, lv_canvas, lv_switch, lv_chart, lv_bar, lv_table, lv_tabview */
#define LV_USE_BENCHMARK 0

/** 1: Enable the file explorer widget
 * - Requires: lv_list, lv_dropdown, lv_label, lv_textarea */
#define LV_USE_FILE_EXPLORER 0
#if LV_USE_FILE_EXPLORER
    /** Maximum length of path of file explorer */
    #define LV_FILE_EXPLORER_PATH_MAX_LEN 128
    /** The refresh time of the file explorer in milliseconds */
    #define LV_FILE_EXPLORER_REFRESH_TIME 1000
#endif

/** 1: Enable the IM (Input Method) widget.
 * - Requires: lv_keyboard, lv_textarea */
#define LV_USE_IME 0
#if LV_USE_IME
    /** 1: Use a basic method for im_text_create to get text. */
    #define LV_IME_USE_DEFAULT_IM_READY_CB 0
    /** Max length of the candidate string including NULL */
    #define LV_IME_MAX_TEXT_LEN 32
    /** Max candidate numbers in one page */
    #define LV_IME_CAND_TEXT_NUM 6
    /** Use the default feedback when clicking a candidate */
    #define LV_IME_USE_DEFAULT_CAND_CLICK_CB 0
#endif

/** 1: Enable the meter widget.
 * - Requires: lv_arc, lv_img, lv_label, lv_line */
#define LV_USE_METER 0

/** 1: Enable the odometer widget */
#define LV_USE_ODOMETER 0

/** 1: Enable the spinner widget */
#define LV_USE_SPINNER_WIDGET 0

/** 1: Enable the span widget (a lightweight label)
 * - Requires: lv_label */
#define LV_USE_SPAN_LABEL 0

/** 1: Enable the calendar widget.
 * - Requires: lv_calendar */
#define LV_USE_CALENDAR_WIDGET 0

/** 1: Enable the color temperature widget.
 * - Requires: lv_slider, lv_label, lv_canvas */
#define LV_USE_COLOR_TEMP 0

/** 1: Enable the gradient widget.
 * - Requires: lv_canvas, lv_slider, lv_label */
#define LV_USE_GRADIENT_WIDGET 0

/** 1: Enable the image widget.
 * - Requires: lv_img, lv_label */
#define LV_USE_IMAGE_WIDGET 0

/** 1: Enable the keyboard widget.
 * - Requires: lv_keyboard, lv_textarea */
#define LV_USE_KEYBOARD_WIDGET 0

/** 1: Enable the message box widget.
 * - Requires: lv_msgbox */
#define LV_USE_MSGBOX_WIDGET 0

/** 1: Enable the notification widget.
 * - Requires: lv_label, lv_img, lv_button */
#define LV_USE_NOTIFICATION 0

/** 1: Enable the settings widget.
 * - Requires: lv_list, lv_label, lv_switch, lv_roller, lv_slider, lv_dropdown, lv_textarea, lv_msgbox */
#define LV_USE_SETTINGS 0
#if LV_USE_SETTINGS
    #define LV_SETTINGS_MAX_TITLE_LENGTH 32
    #define LV_SETTINGS_MAX_DETAILS_LENGTH 128
    #define LV_SETTINGS_MIN_WIDTH 200
#endif

/** 1: Enable the spinbox widget.
 * - Requires: lv_spinbox */
#define LV_USE_SPINBOX_WIDGET 0

/** 1: Enable the style editor widget.
 * - Requires: lv_list, lv_label, lv_textarea, lv_slider, lv_roller, lv_dropdown, lv_switch */
#define LV_USE_STYLE_EDITOR 0

/** 1: Enable the tabview widget.
 * - Requires: lv_tabview */
#define LV_USE_TABVIEW_WIDGET 0

/** 1: Enable the template widget.
 * - Requires: lv_label, lv_img */
#define LV_USE_TEMPLATE_WIDGET 0

/** 1: Enable the tileview widget.
 * - Requires: lv_tileview */
#define LV_USE_TILEVIEW_WIDGET 0

/** 1: Enable the button widget.
 * - Requires: lv_button, lv_label */
#define LV_USE_BUTTON_WIDGET 0

/** 1: Enable the chart widget.
 * - Requires: lv_chart */
#define LV_USE_CHART_WIDGET 0

/** 1: Enable the gauge widget.
 * - Requires: lv_meter */
#define LV_USE_GAUGE 0

/** 1: Enable the roller widget.
 * - Requires: lv_roller */
#define LV_USE_ROLLER_WIDGET 0

/** 1: Enable the slider widget.
 * - Requires: lv_slider */
#define LV_USE_SLIDER_WIDGET 0

/** 1: Enable the switch widget.
 * - Requires: lv_switch */
#define LV_USE_SWITCH_WIDGET 0

/** 1: Enable the line meter widget.
 * - Requires: lv_line, lv_label */
#define LV_USE_LINEMETER 0

/** 1: Enable the menu widget.
 * - Requires: lv_menu */
#define LV_USE_MENU_WIDGET 0

/** 1: Enable the progress bar widget.
 * - Requires: lv_bar */
#define LV_USE_PROGRESSBAR 0

/** 1: Enable the radio button widget.
 * - Requires: lv_checkbox, lv_label */
#define LV_USE_RADIOBUTTON 0

/** 1: Enable the segmented control widget.
 * - Requires: lv_buttonmatrix */
#define LV_USE_SEGMENTED_CONTROL 0

/** 1: Enable the table widget.
 * - Requires: lv_table */
#define LV_USE_TABLE_WIDGET 0

/** 1: Enable the image button widget.
 * - Requires: lv_imagebutton */
#define LV_USE_IMAGEBUTTON_WIDGET 0

/** 1: Enable the button matrix widget.
 * - Requires: lv_buttonmatrix */
#define LV_USE_BUTTONMATRIX_WIDGET 0

/** 1: Enable the checkbox widget.
 * - Requires: lv_checkbox */
#define LV_USE_CHECKBOX_WIDGET 0

/** 1: Enable the dropdown widget.
 * - Requires: lv_dropdown */
#define LV_USE_DROPDOWN_WIDGET 0

/** 1: Enable the arc widget.
 * - Requires: lv_arc */
#define LV_USE_ARC_WIDGET 0

/** 1: Enable the line widget.
 * - Requires: lv_line */
#define LV_USE_LINE_WIDGET 0

/** 1: Enable the list widget.
 * - Requires: lv_list */
#define LV_USE_LIST_WIDGET 0

/** 1: Enable the scale widget.
 * - Requires: lv_scale */
#define LV_USE_SCALE_WIDGET 0

/** 1: Enable the color wheel widget.
 * - Requires: lv_colorwheel */
#define LV_USE_COLORWHEEL_WIDGET 0

/** 1: Enable the data structure widget (a generic container with a label)
 * - Requires: lv_label, lv_obj */
#define LV_USE_DATA_STRUCTURE 0

/** 1: Enable the profiler to analyze time cost of functions.
 * - Requires: `LV_USE_LOG`
 * - Requires: `LV_USE_TICK_INC` to be called */
#define LV_USE_PROFILER 0
#if LV_USE_PROFILER
    #define LV_PROFILER_INCLUDE "stdio.h"
    #define LV_PROFILER_BEGIN         { uint32_t t_start = LV_TICK_GET();
    #define LV_PROFILER_END(name)     { LV_LOG(LV_LOG_LEVEL_INFO, "%s takes %d ms", name, (int)(LV_TICK_GET() - t_start)); }
    #define LV_PROFILER_END_TRACE(name)     { LV_LOG(LV_LOG_LEVEL_TRACE, "%s takes %d ms", name, (int)(LV_TICK_GET() - t_start)); }
    #define LV_PROFILER_CUSTOM_BEGIN(name)      { uint32_t t_start_##name = LV_TICK_GET();
    #define LV_PROFILER_CUSTOM_END(name)        { LV_LOG(LV_LOG_LEVEL_INFO, "%s takes %d ms", #name, (int)(LV_TICK_GET() - t_start_##name)); }
    #define LV_PROFILER_CUSTOM_END_TRACE(name)  { LV_LOG(LV_LOG_LEVEL_TRACE, "%s takes %d ms", #name, (int)(LV_TICK_GET() - t_start_##name)); }

    /*Enable refr profiler*/
    #define LV_PROFILER_REFR 1
    /*Enable draw profiler*/
    #define LV_PROFILER_DRAW 1
    /*Enable indev profiler*/
    #define LV_PROFILER_INDEV 1
    /*Enable decoder profiler*/
    #define LV_PROFILER_DECODER 1
    /*Enable font profiler*/
    #define LV_PROFILER_FONT 1
    /*Enable fs profiler*/
    #define LV_PROFILER_FS 1
    /*Enable style profiler*/
    #define LV_PROFILER_STYLE 0
    /*Enable timer profiler*/
    #define LV_PROFILER_TIMER 1
    /*Enable cache profiler*/
    #define LV_PROFILER_CACHE 1
    /*Enable event profiler*/
    #define LV_PROFILER_EVENT 1
#endif

/** 1: Enable Monkey test */
#define LV_USE_MONKEY 0

/** 1: Enable grid navigation */
#define LV_USE_GRIDNAV 0

/** 1: Enable `lv_obj` fragment logic */
#define LV_USE_FRAGMENT 0

/** 1: Support using images as font in label or span widgets */
#define LV_USE_IMGFONT 0

/** 1: Enable an observer pattern implementation */
#define LV_USE_OBSERVER 1

/** 1: Enable Pinyin input method
 * - Requires: lv_keyboard */
#define LV_USE_IME_PINYIN 0
#if LV_USE_IME_PINYIN
    /** 1: Use default thesaurus.
     * @note If you do not use the default thesaurus, be sure to use `lv_ime_pinyin` after setting the thesaurus. */
    #define LV_IME_PINYIN_USE_DEFAULT_DICT 1
    /** Set maximum number of candidate panels that can be displayed.
     * @note This needs to be adjusted according to size of screen. */
    #define LV_IME_PINYIN_CAND_TEXT_NUM 6
    /** Use 9-key input (keypad) */
    #define LV_IME_PINYIN_USE_9_KEY 0
#endif

/*==================
 * DEMO USAGE
 *==================*/

#if LV_BUILD_DEMOS

/** Show some widgets. It might be required to increase `LV_MEM_SIZE`. */
#define LV_USE_DEMO_WIDGETS 1

/** Demonstrate the usage of the most common widgets in a two-panel arrangement.
 * - Requires: `LV_USE_DEMO_WIDGETS = 1` */
#define LV_USE_DEMO_WIDGETS_SLIDESHOW 0

/** Show a music player demo.
 * - Requires: lv_tabview, lv_button, lv_label, lv_slider, lv_list, lv_chart, lv_roller, lv_arc, lv_img, lv_imgbtn, lv_checkbox, lv_switch, lv_dropdown, lv_textarea, lv_msgbox */
#define LV_USE_DEMO_MUSIC 0
#if LV_USE_DEMO_MUSIC
    #define LV_DEMO_MUSIC_SQUARE    0
    #define LV_DEMO_MUSIC_LANDSCAPE 0
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif

/** Vector graphic demo */
#define LV_USE_DEMO_VECTOR_GRAPHIC  0

/** GLTF demo */
#define LV_USE_DEMO_GLTF            0

/*---------------------------
 * Demos from lvgl/lv_demos
  ---------------------------*/

/** Flex layout demo */
#define LV_USE_DEMO_FLEX_LAYOUT     0

/** Smart-phone like multi-language demo */
#define LV_USE_DEMO_MULTILANG       0

/** Widget transformation demo */
#define LV_USE_DEMO_TRANSFORM       0

/** Demonstrate scroll settings */
#define LV_USE_DEMO_SCROLL          0

/*E-bike demo with Lottie animations (if LV_USE_LOTTIE is enabled)*/
#define LV_USE_DEMO_EBIKE           0
#if LV_USE_DEMO_EBIKE
    #define LV_DEMO_EBIKE_LARGE 0 /**< For 800x480 resolution */
#endif

#endif /* LV_BUILD_DEMOS */

/*=================
 * CUSTOM DRIVERS
 *=================*/

#define LV_USE_CUSTOM_DRIVER 0
#if LV_USE_CUSTOM_DRIVER
    #define LV_CUSTOM_DRIVER_INCLUDE <stdint.h>
#endif

/*=================
 * EXAMPLES
 *=================*/

#if LV_BUILD_EXAMPLES

/** Example for `lv_obj` */
#define LV_EX_OBJ 1

/** Example for `lv_style` */
#define LV_EX_STYLE 1

/** Example for `lv_font` */
#define LV_EX_FONT 1

/** Example for `lv_timer` */
#define LV_EX_TIMER 1

/** Example for `lv_anim` */
#define LV_EX_ANIM 1

/** Example for `lv_theme` */
#define LV_EX_THEME 1

/** Example for `lv_disp` */
#define LV_EX_DISP 1

/** Example for `lv_indev` */
#define LV_EX_INDEV 1

/** Example for `lv_group` */
#define LV_EX_GROUP 1

/** Example for `lv_keyboard` */
#define LV_EX_KEYBOARD 1

/** Example for `lv_image` */
#define LV_EX_IMAGE 1

/** Example for `lv_qrcode` */
#define LV_EX_QRCODE 1

/** Example for `lv_tabview` */
#define LV_EX_TABVIEW 1

/** Example for `lv_list` */
#define LV_EX_LIST 1

/** Example for `lv_arc` */
#define LV_EX_ARC 1

/** Example for `lv_bar` */
#define LV_EX_BAR 1

/** Example for `lv_line` */
#define LV_EX_LINE 1

/** Example for `lv_chart` */
#define LV_EX_CHART 1

/** Example for `lv_table` */
#define LV_EX_TABLE 1

/** Example for `lv_text_area` */
#define LV_EX_TEXTAREA 1

/** Example for `lv_roller` */
#define LV_EX_ROLLER 1

/** Example for `lv_meter` */
#define LV_EX_METER 1

/** Example for `lv_canvas` */
#define LV_EX_CANVAS 1

/** Example for `lv_layer` */
#define LV_EX_LAYER 1

/** Example for `lv_draw` */
#define LV_EX_DRAW 1

/** Example for `lv_sdl` */
#define LV_EX_SDL 1

/** Example for `lv_x11` */
#define LV_EX_X11 1

/** Example for `lv_wayland` */
#define LV_EX_WAYLAND 1

/** Example for `lv_nuttx` */
#define LV_EX_NUTTX 1

/** Example for `lv_linux_drm` */
#define LV_EX_LINUX_DRM 1

/** Example for `lv_win32` */
#define LV_EX_WIN32 1

/** Example for `lv_fs` */
#define LV_EX_FS 1

/** Example for `lv_event` */
#define LV_EX_EVENT 1

/** Example for `lv_log` */
#define LV_EX_LOG 1

/** Example for `lv_sysmon` */
#define LV_EX_SYSMON 1

/** Example for `lv_profiler` */
#define LV_EX_PROFILER 1

/** Example for `lv_observer` */
#define LV_EX_OBSERVER 1

/** Example for `lv_imgfont` */
#define LV_EX_IMGFONT 1

#endif /* LV_BUILD_EXAMPLES */

/*==================
 * DEVICE DRIVERS
 *==================*/

/** Enable generic display driver for simple displays */
#define LV_USE_DISP_MONO 0

/** Enable generic display driver for displays with a dedicated color buffer */
#define LV_USE_DISP_TFT 0

/** Enable SDL2 display and input drivers */
#define LV_USE_SDL 0
#if LV_USE_SDL
    #define LV_SDL_INCLUDE <SDL2/SDL.h>
    #define LV_SDL_RENDER_MODE LV_SDL_RENDER_MODE_PARTIAL /**< LV_SDL_RENDER_MODE_PARTIAL, LV_SDL_RENDER_MODE_FULL or LV_SDL_RENDER_MODE_DIRECT */
    #define LV_SDL_BUF_COUNT 1 /**< 1 or 2 */
    #define LV_SDL_ACCELERATED 1 /**< 1: Use hardware acceleration*/
    #define LV_SDL_FULLSCREEN 0 /**< 1: Make the window full screen by default */
    #define LV_SDL_DIRECT_EXIT 1 /**< 1: Exit the application when all SDL windows are closed */
    #define LV_SDL_MOUSEWHEEL_MODE LV_SDL_MOUSEWHEEL_MODE_ENCODER /*LV_SDL_MOUSEWHEEL_MODE_ENCODER/CROWN*/
#endif

/** Use X11 to open window on Linux desktop and handle mouse and keyboard */
#define LV_USE_X11 0
#if LV_USE_X11
    #define LV_X11_DIRECT_EXIT 1 /**< Exit application when all X11 windows have been closed */
    #define LV_X11_DOUBLE_BUFFER 1 /**< Use double buffers for rendering */
    /* Select only 1 of the following render modes (LV_X11_RENDER_MODE_PARTIAL preferred!). */
    #define LV_X11_RENDER_MODE_PARTIAL 1 /**< Partial render mode (preferred) */
    #define LV_X11_RENDER_MODE_DIRECT 0 /**< Direct render mode */
    #define LV_X11_RENDER_MODE_FULL 0 /**< Full render mode */
#endif

/** Use Wayland to open a window and handle input on Linux or BSD desktops */
#define LV_USE_WAYLAND 0
#if LV_USE_WAYLAND
    #define LV_WAYLAND_BUF_COUNT 1 /**< Use 1 for single buffer with partial render mode or 2 for double buffer with full render mode*/
    #define LV_WAYLAND_USE_DMABUF 0 /**< Use DMA buffers for frame buffers. Requires LV_LINUX_DRM_GBM_BUFFERS to be enabled */
#endif

/** Use Windows GDI/DXGI to open window on Windows desktop and handle mouse and keyboard */
#define LV_USE_WIN32 0
#if LV_USE_WIN32
    #define LV_WIN32_DIRECT_EXIT 1 /**< Exit application when all Win32 windows have been closed */
#endif

/** Use NuttX display and input drivers */
#define LV_USE_NUTTX 0
#if LV_USE_NUTTX
    /* Optional: To enable trace log for NuttX display */
    #define LV_USE_NUTTX_TRACE_DISPLAY 0
    /* Optional: To enable trace log for NuttX input */
    #define LV_USE_NUTTX_TRACE_INPUT 0
    /* Optional: To save trace log in a file and its path*/
    #define LV_USE_NUTTX_TRACE_FILE 0
    #if LV_USE_NUTTX_TRACE_FILE
        #define LV_NUTTX_TRACE_FILE_PATH "/data/lvgl-trace.log"
    #endif
#endif

/** Driver for /dev/dri/card */
#define LV_USE_LINUX_DRM 0
#if LV_USE_LINUX_DRM
    /* Use the MESA GBM library to allocate DMA buffers that can be
     * shared across sub-systems and libraries using the Linux DMA-BUF API.
     * The GBM library aims to provide a platform independent memory management system
     * it supports the major GPU vendors - This option requires linking with libgbm */
    #define LV_USE_LINUX_DRM_GBM_BUFFERS 0
    #define LV_LINUX_DRM_USE_EGL 0
#endif

/** Interface for TFT_eSPI */
#define LV_USE_TFT_ESPI 0

/** Interface for Lovyan_GFX */
#define LV_USE_LOVYAN_GFX 0
#if LV_USE_LOVYAN_GFX
    #define LV_LGFX_USER_INCLUDE "lv_lgfx_user.hpp"
#endif /*LV_USE_LOVYAN_GFX*/

/** Driver for evdev input devices */
#define LV_USE_EVDEV 0

/** Driver for libinput input devices */
#define LV_USE_LIBINPUT 0
#if LV_USE_LIBINPUT
    #define LV_LIBINPUT_BSD 0 /** Full keyboard support on BSD, where `keyboard` groups include many keys outside of the standard Latin/Cyrillic/etc. keyboard set. */
#endif

/** QNX Screen display and input drivers */
#define LV_USE_QNX 0
#if LV_USE_QNX
    #define LV_QNX_BUF_COUNT 1 /**< 1 or 2 */
#endif

/*=====================
 * BUILD OPTIONS
 *======================*/

/** Enable examples to be built with the library. */
#define LV_BUILD_EXAMPLES 1

/** Build the demos */
#define LV_BUILD_DEMOS 1

/*==================
 * END OF SETTINGS
 *==================*/


#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
