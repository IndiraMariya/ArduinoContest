import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

PORT = '/dev/cu.usbmodem112401'  # Update to your actual port
BAUD = 115200
ser = serial.Serial(PORT, BAUD)

fig, ax = plt.subplots()
x = np.arange(1, 64)
bars = ax.bar(x, np.zeros_like(x), width=1.0)

# === Customize appearance ===
ax.set_ylim(0, 80)       # Increased to give room for boosted bars
ax.set_xlim(0, 65)
ax.set_title('Arduino Spectrum Analyzer')
ax.set_xlabel('Frequency Bin')
ax.set_ylabel('Magnitude')
plt.grid(True)

# === Smoothing and scaling ===
last_vals = np.zeros(63)
smoothing = 0.6
scale_factor = 2.5  # ← This boosts bar heights. Try 2.0, 3.0, etc.

def update(frame):
    global last_vals
    try:
        line = ser.readline().decode().strip()
        values = np.array(list(map(int, line.split(','))))
        if len(values) == 63:
            last_vals = smoothing * last_vals + (1 - smoothing) * values
            scaled_vals = last_vals * scale_factor
            for bar, val in zip(bars, scaled_vals):
                bar.set_height(val)
    except Exception as e:
        print("Error reading data:", e)

ani = animation.FuncAnimation(fig, update, interval=30)
plt.show()
