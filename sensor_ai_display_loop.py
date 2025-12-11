import socket
import time
import numpy as np
import sys
import io
from PIL import Image
import requests
import base64

# --- API Configuration ---
STABILITY_API_KEY = "sk-kdeBvH0q4bae23YA1LpSnSVrFJIypg1pZlJLEK7MoEdaOsnl" 
STABILITY_API_URL = "https://api.stability.ai/v2beta/stable-image/generate/core"
STABILITY_MODEL = "stable-diffusion-xl" 

# --- NETWORK CONFIGURATION (ESP32 is the SERVER) ---
# ESP32 IP is 192.168.0.17 (Confirmed)
ESP32_IP_ADDRESS = '192.168.0.17' 
ESP32_IMAGE_PORT = 8080        # Connect to stream image
ESP32_SENSOR_PORT = 8082       # Connect to request sensor data

# --- LCD Image Dimensions ---
IMAGE_WIDTH = 320  
IMAGE_HEIGHT = 170  
EXPECTED_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 2 
POLLING_INTERVAL = 30 # seconds

# -----------------------------------------------------------------------------
# *** UTILITY FUNCTIONS (Generation, Conversion, Send Image - UNCHANGED) ***
# -----------------------------------------------------------------------------

# ( ... Include the functions: generate_image_from_prompt, convert_to_rgb565_raw, send_image_tcp from the previous step ... )
# Due to length constraints, I'm omitting the bodies of the standard functions.
# They remain exactly as in the sensor_ai_display_loop.py script, just the name changes.

def generate_image_from_prompt(prompt, api_key):
    """Generates an image using the Stability AI API."""
    print(f"\n[GENERATING IMAGE] Prompt: '{prompt}'...")
    try:
        headers = {"Accept": "image/*", "Authorization": f"Bearer {api_key}"}
        data = {"prompt": (None, prompt), "output_format": (None, "jpeg"), "aspect_ratio": (None, "16:9"), "model": (None, STABILITY_MODEL)}
        response = requests.post(STABILITY_API_URL, headers=headers, files=data, timeout=45)
        if not response.ok:
            print(f"--- ERROR: Stability AI Failed --- Status: {response.status_code}. Response: {response.text}")
            return None
        pil_image = Image.open(io.BytesIO(response.content))
        print("[GENERATING IMAGE] Image successfully generated.")
        return pil_image
    except Exception as e:
        print(f"--- ERROR: Image Generation Failed --- Details: {e}")
        return None

def convert_to_rgb565_raw(pil_image):
    """Resizes and converts the PIL Image to raw 16-bit RGB565 binary data."""
    if pil_image.size != (IMAGE_WIDTH, IMAGE_HEIGHT):
        img_resized = pil_image.resize((IMAGE_WIDTH, IMAGE_HEIGHT), Image.Resampling.LANCZOS)
    else:
        img_resized = pil_image

    img_rgb = img_resized.convert("RGB")
    np_array = np.array(img_rgb, dtype=np.uint8)
    
    R = np_array[:, :, 0]
    G = np_array[:, :, 1]
    B = np_array[:, :, 2]

    rgb565 = (R.astype(np.uint16) >> 3) << 11 | \
             (G.astype(np.uint16) >> 2) << 5 | \
             (B.astype(np.uint16) >> 3)
             
    raw_data = rgb565.astype('<u2').tobytes() 
    
    if len(raw_data) != EXPECTED_SIZE:
        raise ValueError(f"Conversion failed: Expected {EXPECTED_SIZE} bytes, got {len(raw_data)}.")

    print(f"[CONVERSION] Success. Converted {len(raw_data)} bytes to RGB565.")
    return raw_data


def send_image_tcp(raw_data):
    """Connects to the ESP32 Image Server (8080) and sends the raw binary data."""
    print(f"[CLIENT] Connecting to ESP32 Image Server at {ESP32_IP_ADDRESS}:{ESP32_IMAGE_PORT}...")
    
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5) 
        s.connect((ESP32_IP_ADDRESS, ESP32_IMAGE_PORT))
        print("[CLIENT] Connection successful! Streaming image data...")

        s.sendall(raw_data)
        
        s.shutdown(socket.SHUT_RDWR)
        s.close()
        print("[CLIENT] Image stream complete. Socket closed.")
        
    except socket.timeout:
        print("--- NETWORK ERROR --- Connection timed out.")
    except ConnectionRefusedError:
        print("--- NETWORK ERROR --- Connection refused. Is ESP32 server running?")
    except Exception as e:
        print(f"An unexpected error occurred during image stream: {e}")


# -----------------------------------------------------------------------------
# *** NEW SENSOR POLLING LOGIC ***
# -----------------------------------------------------------------------------
def poll_sensor_data():
    """Connects to the ESP32 Sensor Server (8082) and requests data."""
    print(f"\n[POLLING] Connecting to ESP32 Sensor Server at {ESP32_IP_ADDRESS}:{ESP32_SENSOR_PORT}...")
    
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5) 
        s.connect((ESP32_IP_ADDRESS, ESP32_SENSOR_PORT))
        print("[POLLING] Connection successful. Requesting data...")

        # Send a simple request command to the ESP32
        s.sendall("GET_TEMP\n".encode('utf-8'))

        # Receive the response (sensor data)
        data = s.recv(1024).decode('utf-8').strip()
        
        s.close()
        return data
        
    except socket.timeout:
        print("--- POLLING ERROR --- Connection timed out. ESP32 unresponsive.")
    except ConnectionRefusedError:
        print("--- POLLING ERROR --- Connection refused. ESP32 server not listening on 8082.")
    except Exception as e:
        print(f"An unexpected error occurred during polling: {e}")
    
    return None

def process_and_send_image(data):
    """Parses sensor data, generates prompt, and initiates image stream."""
    print(f"[PROCESSOR] Received sensor data: {data}")
    
    try:
        # Simple Parsing (Example: data = "temp=35.50C")
        temp_string = data.split('=')[1].replace('C', '')
        temp_value = float(temp_string)
    except Exception:
        print("[PROCESSOR] Could not parse temperature data. Using default temp 25.0C.")
        temp_value = 25.0

    # 1. GENERATE DYNAMIC PROMPT
    if temp_value > 30:
        prompt = f"A dramatic, hot, desert landscape under a blazing sun with red and yellow tones. Current temperature is {temp_value:.2f}C."
    elif temp_value < 20:
        prompt = f"A serene, cold winter wonderland with thick snow and deep blue tones. Current temperature is {temp_value:.2f}C."
    else:
        decimal_part = temp_value - int(temp_value)
        if decimal_part < 0.33:
             mood = "A mild, contemplative forest scene at dawn with soft muted colors."
        elif decimal_part < 0.66:
             mood = "A pleasant, sunny meadow scene in the afternoon with cheerful green and yellow."
        else:
             mood = "A slightly overcast, calm lake scene at dusk with atmospheric moody blue."
             
        prompt = f"{mood} Temperature is {temp_value:.2f}C, causing subtle changes in the environment."

    print(f"[PROCESSOR] Generated AI Prompt: {prompt}")

    # 2. GENERATE IMAGE
    pil_image = generate_image_from_prompt(prompt, STABILITY_API_KEY)
    
    if pil_image:
        # 3. CONVERT & SEND IMAGE BACK TO ESP32
        try:
            raw_data_bytes = convert_to_rgb565_raw(pil_image)
            send_image_tcp(raw_data_bytes)
        except ValueError as e:
            print(f"FATAL ERROR during conversion: {e}. Aborting send.")
    
    print("-" * 50)


if __name__ == "__main__":
    print(f"*** Starting Firewall-Bypass Polling Client ***")
    print(f"Polling ESP32 at {ESP32_IP_ADDRESS} every {POLLING_INTERVAL} seconds.")
    
    while True:
        sensor_data = poll_sensor_data()
        
        if sensor_data:
            process_and_send_image(sensor_data)
        else:
            print("Failed to retrieve sensor data. Skipping image generation.")
            
        print(f"Waiting {POLLING_INTERVAL} seconds...")
        time.sleep(POLLING_INTERVAL)