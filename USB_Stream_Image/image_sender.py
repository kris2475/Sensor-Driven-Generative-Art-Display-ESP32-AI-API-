import serial
import time
import os

# --- Configuration ---
COM_PORT = 'COM17' 
BAUD_RATE = 115200 
FILE_PATH = "myimage.raw"
START_COMMAND = "START_IMAGE_TRANSFER\n" # Command followed by newline

def send_image_serial():
    if not os.path.exists(FILE_PATH):
        print(f"Error: File not found at {FILE_PATH}. Make sure it is in the same directory as this script.")
        return

    try:
        with open(FILE_PATH, 'rb') as f:
            raw_data = f.read()
        
        expected_size = 320 * 170 * 2
        if len(raw_data) != expected_size:
            print(f"Error: File size mismatch. Expected {expected_size} bytes, got {len(raw_data)} bytes.")
            return

        print(f"Connecting to {COM_PORT} at {BAUD_RATE}...")
        
        ser = serial.Serial(COM_PORT, BAUD_RATE)
        ser.write_timeout = 1 
        time.sleep(2) 

        # --- STEP 1: SEND START COMMAND ---
        print(f"Sending start command: {START_COMMAND.strip()}")
        ser.write(START_COMMAND.encode())
        time.sleep(0.5) # Wait for the ESP32 to enter the read function

        # --- STEP 2: SEND RAW DATA ---
        print(f"Sending {len(raw_data)} bytes of image data over Serial...")
        
        bytes_sent = ser.write(raw_data)
        
        ser.close()

        if bytes_sent == expected_size:
            print("--- SUCCESS ---")
            print("Image transmission successful. The screen should now be displaying the image.")
        else:
            print(f"--- FAILURE --- (Only sent {bytes_sent} of {expected_size})")

    except serial.SerialException as e:
        print("--- SERIAL CONNECTION ERROR ---")
        print(f"Could not open or write to port {COM_PORT}. Error: {e}")
        print("Make sure the Serial Monitor in the Arduino IDE is closed.")
    
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    send_image_serial()
