import sys

INIT_X     = 0.5  # initial x position (metres)
INIT_Y     = 0.5  # initial y position (metres)
INIT_THETA = 0.0  # initial orientation (radians)


def step(step_number, time, step_interval, light):
    """
    Parameters
    ----------
    light : list of 8 floats  -- light sensor readings [0..1], 0=dark, 1=bright

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[time {time}] Light: {[f'{v:.4f}' for v in light]}", file=sys.stderr)

    left_speed  = 400
    right_speed = -400
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 3:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    light         = [float(v) for v in parts[3:]]
    left, right   = step(step_number, time, step_interval, light)
    print(f"{left} {right}", flush=True)
