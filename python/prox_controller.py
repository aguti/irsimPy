import sys

INIT_X     = 0.0   # initial x position (metres)
INIT_Y     = 1.2   # initial y position (metres)
INIT_THETA = 1.57  # initial orientation (radians)


def step(step_number, time, step_interval, prox):
    """
    Parameters
    ----------
    prox : list of 8 floats  -- proximity sensor readings [0..1], 0=far, 1=close

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[time {time}] Prox: {[f'{v:.4f}' for v in prox]}", file=sys.stderr)

    left_speed  = 100
    right_speed = -100
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 3:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    prox          = [float(v) for v in parts[3:]]
    left, right   = step(step_number, time, step_interval, prox)
    print(f"{left} {right}", flush=True)
