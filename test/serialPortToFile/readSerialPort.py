import serial
import time

# --- CONFIGURATION ---
SERIAL_PORT = 'COM3'     # Change to '/dev/ttyUSB0' or '/dev/ttyACM0' on Linux/Mac
BAUD_RATE = 9600         # Match this to your device's baud rate
OUTPUT_FILE = 'serial_output.txt'
# ---------------------

print(f"Connecting to {SERIAL_PORT} at {BAUD_RATE} baud...")

try:
    # Open the serial port
    # timeout=1 ensures the read doesn't block indefinitely if no data arrives
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser, \
         open(OUTPUT_FILE, 'a', encoding='utf-8') as file:
         
        print(f"Connected! Logging data to '{OUTPUT_FILE}'. Press Ctrl+C to stop.")
        
        while True:
            # Read a line of data terminated by '\n'
            line = ser.readline()
            
            if line:
                # Decode the bytes to a string and strip whitespace (like \r\n)
                decoded_line = line.decode('utf-8', errors='ignore').strip()
                
                if decoded_line:
                    # Optional: Add a timestamp to each logged line
                    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                    log_entry = f"[{timestamp}] {decoded_line}\n"
                    
                    # Print to console and write to file
                    print(log_entry, end="")
                    file.write(log_entry)
                    file.flush()  # Force writing to disk immediately
                    
except serial.SerialException as e:
    print(f"\nSerial Error: Could not open port {SERIAL_PORT}. Is it plugged in or used by another app?")
except KeyboardInterrupt:
    print("\nLogging stopped by user. Exiting cleanly.")