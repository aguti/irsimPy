import sys
import math

INIT_X     = 0.0        # initial x position (metres)
INIT_Y     = 0.0        # initial y position (metres)
INIT_THETA = math.pi/4  # initial orientation (radians)


def step(step_number, time, step_interval, battery):
    """
    Parameters
    ----------
    battery : list of 1 float  -- battery level [0..1], 0=empty, 1=full

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[time {time}] Battery: {battery[0]:.4f}", file=sys.stderr)

    # Drive forward when charged, reverse to charger when low
    if battery[0] < 0.3:
        left_speed, right_speed = -500, -500
    else:
        left_speed, right_speed = 200, 200
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 3:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    battery       = [float(v) for v in parts[3:]]
    left, right   = step(step_number, time, step_interval, battery)
    print(f"{left} {right}", flush=True)
