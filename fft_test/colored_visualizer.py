import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

PORT = '/dev/cu.usbmodem112401'
BAUD = 115200
ser = serial.Serial(PORT, BAUD)

# Category definitions
categories = {
    "subbass": (1, 2),
    "bass": (3, 4),
    "low-mid": (5, 8),
    "mid": (9, 32),
    "high-mid": (33, 64),
}
colors = {
    "subbass": 'blue',
    "bass": 'green',
    "low-mid": 'orange',
    "mid": 'red',
    "high-mid": 'purple',
}

fig, ax = plt.subplots()

# X positions and widths for each category bar
category_names = list(categories.keys())
positions = np.arange(len(category_names))
bar_width = 1.0
bars = ax.bar(positions, [0]*len(categories), width=bar_width, color=[colors[c] for c in category_names])

# Graph style
ax.set_ylim(0, 100)
ax.set_xlim(-0.5, len(categories)-0.5)
ax.set_xticks(positions)
ax.set_xticklabels(category_names)
ax.set_title('Arduino Frequency Spectrum by Category')
ax.set_ylabel('Magnitude')
plt.grid(True)

# Smoothing
last_vals = np.zeros(63)
smoothing = 0.6
scale_factor = 2.5

def update(frame):
    global last_vals
    try:
        line = ser.readline().decode().strip()
        values = np.array(list(map(int, line.split(','))))
        if len(values) == 63:
            last_vals = smoothing * last_vals + (1 - smoothing) * values
            scaled_vals = last_vals * scale_factor

            # Category averaging
            bin_avgs = []
            for (start, end) in categories.values():
                avg = np.mean(scaled_vals[start-1:end])
                bin_avgs.append(avg)

            for bar, val in zip(bars, bin_avgs):
                bar.set_height(val)
    except Exception as e:
        print("Error reading data:", e)

ani = animation.FuncAnimation(fig, update, interval=30)
plt.show()
