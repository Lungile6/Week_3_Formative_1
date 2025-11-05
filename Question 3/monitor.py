import time
import matplotlib.pyplot as plt
import sensormodule  # Import our compiled C module

# --- Setup Real-time Plot ---
plt.ion() # Turn on interactive mode
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)
fig.suptitle('Simulated IoT Sensor Dashboard')

# --- Data Lists ---
timestamps = []
temps = []
hums = []
bats = []

start_time = time.time()
loop_duration = 30 # Total time to run (in seconds)
interval = 2       # Interval between readings (in seconds)

print(f"Monitoring for {loop_duration} seconds...")

try:
    while (time.time() - start_time) < loop_duration:
        # 1. Fetch data from C extension
        temp, humidity, battery = sensormodule.get_readings()
        
        current_time = time.time() - start_time
        
        # 2. Append data to lists
        timestamps.append(current_time)
        temps.append(temp)
        hums.append(humidity)
        bats.append(battery)
        
        # 3. Update Plots
        
        # Temperature
        ax1.clear()
        ax1.plot(timestamps, temps, 'r-')
        ax1.set_ylabel('Temp (°C)')
        ax1.grid(True)
        
        # Humidity
        ax2.clear()
        ax2.plot(timestamps, hums, 'b-')
        ax2.set_ylabel('Humidity (%)')
        ax2.grid(True)

        # Battery
        ax3.clear()
        ax3.plot(timestamps, bats, 'g-')
        ax3.set_ylabel('Battery (%)')
        ax3.set_xlabel('Time (s)')
        ax3.set_ylim(65, 95) # Fix y-axis for battery
        ax3.grid(True)
        
        plt.tight_layout(rect=[0, 0.03, 1, 0.95]) # Adjust for suptitle
        plt.pause(0.1) # Redraw the canvas
        
        print(f"[Time: {current_time:.1f}s] Temp: {temp:.1f}C, Hum: {humidity:.1f}%, Bat: {battery:.1f}%")

        # 4. Wait for the next interval
        time.sleep(interval)

except KeyboardInterrupt:
    print("Monitoring stopped by user.")
finally:
    plt.ioff() # Turn off interactive mode
    print("Monitoring complete. Final plot is now static.")
    plt.show() # Keep the final plot window open
