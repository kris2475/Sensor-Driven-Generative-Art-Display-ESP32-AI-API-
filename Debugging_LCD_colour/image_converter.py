import os
import sys
from PIL import Image

# --- CONFIGURATION ---
TARGET_WIDTH = 320
TARGET_HEIGHT = 240
# The name of the C array and struct
IMAGE_NAME = "Gemini_Generated_Image"
# Output file name
OUTPUT_FILENAME = f"{IMAGE_NAME}_{TARGET_WIDTH}x{TARGET_HEIGHT}.c"
# ---------------------

def rgb_to_bgr565(r, g, b):
    """
    Converts 8-bit R, G, B components to a 16-bit BGR565 value.
    (This function performs the R/B color swap and the necessary byte swap.)
    """
    # CRITICAL FIX: Swap R and B here to generate BGR data (BBBBBGGGGGGRRRRR)
    # B (5 bits) << 11 | G (6 bits) << 5 | R (5 bits)
    p16 = ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3)
    
    # Byte Swap (High Byte <-> Low Byte) for LVGL/ESP32 Endianness
    swapped_p16 = ((p16 & 0xFF) << 8) | ((p16 >> 8) & 0xFF)
    
    return swapped_p16

def convert_image_to_c_array(input_path, output_path):
    """Opens an image, converts it to a 16-bit BGR565 C array, and writes the output file."""
    try:
        img = Image.open(input_path)
    except FileNotFoundError:
        print(f"Error: Input file '{input_path}' not found.")
        return
    except Exception as e:
        print(f"Error opening image: {e}")
        return

    # Resize and convert to RGB
    img = img.resize((TARGET_WIDTH, TARGET_HEIGHT)).convert("RGB")
    pixels = list(img.getdata())

    print(f"Converting {os.path.basename(input_path)} to {OUTPUT_FILENAME} ({TARGET_WIDTH}x{TARGET_HEIGHT} pixels)...")

    # Start C file content
    c_code = (
        f'#include <stdint.h>\n'
        f'#include <lvgl.h>\n\n'
        f'// Generated from: {os.path.basename(input_path)} at {TARGET_WIDTH}x{TARGET_HEIGHT}\n'
        f'// Format: BGR565 (16-bit, manually byte-swapped and color-swapped)\n'
        f'// NOTE: Field ".header.always_zero" removed for LVGL v8 compatibility.\n\n'
        f'const uint8_t {IMAGE_NAME}_map[] = {{\n'
    )

    # Process all pixels and format as C array
    line_count = 0
    for r, g, b in pixels:
        p16_swapped = rgb_to_bgr565(r, g, b)
        
        # Write the 16-bit value as two 8-bit hex values (Low Byte, High Byte)
        byte_low = p16_swapped & 0xFF
        byte_high = (p16_swapped >> 8) & 0xFF
        
        c_code += f'0x{byte_low:02X}, 0x{byte_high:02X}, '
        line_count += 1
        
        if line_count % 16 == 0:
            c_code += '\n'

    c_code = c_code.rstrip(', \n') + '\n};\n\n'
    
    # Add the LVGL image descriptor struct
    c_code += (
        f'const lv_image_dsc_t {IMAGE_NAME}_320x240 = {{\n'
        f'  .header.cf = LV_COLOR_FORMAT_RGB565,\n'
        f'  .header.w = {TARGET_WIDTH},\n'
        f'  .header.h = {TARGET_HEIGHT},\n'
        f'  .data_size = {TARGET_WIDTH * TARGET_HEIGHT * 2},\n'
        f'  .data = {IMAGE_NAME}_map,\n'
        f'}};\n'
    )

    # Write to file
    with open(output_path, 'w') as f:
        f.write(c_code)

    print(f"✅ Success! Image data written to {output_path}")


if __name__ == "__main__":
    if len(sys.argv) == 1:
        input_file = input("Enter the input image file path (e.g., my_image.png): ")
    else:
        input_file = sys.argv[1]
        
    convert_image_to_c_array(input_file, OUTPUT_FILENAME)