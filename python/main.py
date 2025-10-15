import serial, time, threading

ESP_PORT  = "COM5"  
UNO_PORT  = "COM10"   
BAUD      = 115200

def open_port(name):
    return serial.Serial(name, BAUD, timeout=0.05)

def main():
    try:
        esp = open_port(ESP_PORT)
        uno = open_port(UNO_PORT)
        print(f"[PC] Connected to ESP on {ESP_PORT} and UNO on {UNO_PORT}")
    except Exception as e:
        print(f"[PC] ERROR: Failed to open serial ports - {e}")
        return

    # Wait for devices to boot up and send ready messages
    print("[PC] Waiting for devices to initialize...")
    time.sleep(2.0)  # Give devices time to send startup messages
    
    # Read and display startup messages
    for i in range(10):
        esp_msg = esp.readline().decode(errors="ignore").strip()
        uno_msg = uno.readline().decode(errors="ignore").strip()
        if esp_msg:
            print(f"[PC] ESP: {esp_msg}")
        if uno_msg:
            print(f"[PC] UNO: {uno_msg}")
        if not esp_msg and not uno_msg:
            break
        time.sleep(0.1)
    
    # clear buffers after reading messages
    esp.reset_input_buffer(); uno.reset_input_buffer()
    
    # re-arm devices
    print("[PC] Sending RST to both devices...")
    esp.write(b"RST\n")
    uno.write(b"RST\n")
    time.sleep(0.5)  # Wait longer for RST response
    
    # Read RST responses
    for i in range(5):
        esp_msg = esp.readline().decode(errors="ignore").strip()
        uno_msg = uno.readline().decode(errors="ignore").strip()
        if esp_msg:
            print(f"[PC] ESP RST: {esp_msg}")
        if uno_msg:
            print(f"[PC] UNO RST: {uno_msg}")
        time.sleep(0.1)
    
    esp.reset_input_buffer(); uno.reset_input_buffer()

    # start motion
    uno.write(b"GO\n")
    t_go = time.perf_counter()
    print("[PC] GO sent to UNO")

    # wait for TOUCH (green light) from ESP, then STOP UNO
    t_touch = None
    print("[PC] Waiting for TOUCH signal from ESP...")
    
    timeout_start = time.perf_counter()
    while True:
        # Add timeout to prevent infinite waiting
        if time.perf_counter() - timeout_start > 30.0:  # 30 second timeout
            print("[PC] ERROR: Timeout waiting for TOUCH signal")
            break
            
        line = esp.readline().decode(errors="ignore").strip()
        if line:
            print(f"[PC] ESP: {line}")  # Debug: show all ESP messages
            if line == "TOUCH":
                t_touch = time.perf_counter()
                print("[PC] TOUCH received - stopping UNO")
                break

    # immediately stop UNO
    if t_touch:  # Only send STOP if we actually received TOUCH
        uno.write(b"STOP\n")
        t_stop_cmd = time.perf_counter()
        print("[PC] STOP command sent to UNO")
    else:
        print("[PC] No TOUCH received - sending emergency STOP anyway")
        uno.write(b"STOP\n")
        t_stop_cmd = time.perf_counter()

    # read UNO ack
    t_stop_ack = None
    t0 = time.perf_counter()
    while time.perf_counter() - t0 < 1.0:
        ul = uno.readline().decode(errors="ignore").strip()
        if ul:
            print(f"[PC] UNO: {ul}")
            if ul.startswith("OK:STOP"):
                t_stop_ack = time.perf_counter()
                break

    # timings (PC perspective)
    if t_touch:
        dt_touch_to_stop_cmd_ms = (t_stop_cmd - t_touch)*1000.0
        print(f"[PC] Touch→StopCmd latency: {dt_touch_to_stop_cmd_ms:.3f} ms")

        if t_stop_ack:
            print(f"[PC] StopCmd→UNO-ACK: {(t_stop_ack - t_stop_cmd)*1000.0:.3f} ms")
    else:
        print("[PC] Could not calculate timing - no TOUCH signal received")

    # done
    print("[PC] Closing connections...")
    uno.close()
    esp.close()
    print("[PC] Test complete")

if __name__ == "__main__":
    main()
