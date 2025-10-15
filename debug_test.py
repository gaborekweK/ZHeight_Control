import serial
import time

# Test individual devices
def test_esp():
    print("=== Testing ESP8266 on COM5 ===")
    try:
        esp = serial.Serial("COM5", 115200, timeout=2)
        time.sleep(2)  # Give ESP time to boot
        
        print("Reading any startup messages...")
        for i in range(10):  # Read multiple lines
            line = esp.readline().decode(errors="ignore").strip()
            if line:
                print(f"ESP: {line}")
            time.sleep(0.1)
                
        print("Sending RST command...")
        esp.write(b"RST\n")
        time.sleep(0.5)
        
        print("Reading response...")
        for i in range(5):
            line = esp.readline().decode(errors="ignore").strip()
            if line:
                print(f"ESP: {line}")
            time.sleep(0.1)
                
        esp.close()
    except Exception as e:
        print(f"ESP Error: {e}")

def test_uno():
    print("\n=== Testing Arduino Uno on COM10 ===")
    try:
        uno = serial.Serial("COM10", 115200, timeout=2)
        time.sleep(2)  # Give Arduino time to boot
        
        print("Reading any startup messages...")
        for i in range(10):
            line = uno.readline().decode(errors="ignore").strip()
            if line:
                print(f"UNO: {line}")
            time.sleep(0.1)
                
        print("Sending GO command...")
        uno.write(b"GO\n")
        time.sleep(0.5)
        
        print("Reading response...")
        for i in range(5):
            line = uno.readline().decode(errors="ignore").strip()
            if line:
                print(f"UNO: {line}")
            time.sleep(0.1)
                
        print("Sending STOP command...")
        uno.write(b"STOP\n")
        time.sleep(0.5)
        
        print("Reading response...")
        for i in range(5):
            line = uno.readline().decode(errors="ignore").strip()
            if line:
                print(f"UNO: {line}")
            time.sleep(0.1)
                
        uno.close()
    except Exception as e:
        print(f"UNO Error: {e}")

if __name__ == "__main__":
    test_esp()
    test_uno()