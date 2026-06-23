import sys


def step(step_number, time, step_interval, battery):
    """
    Parameters
    ----------
    battery : list of 1 float  -- battery level [0..1], 0=empty, 1=full

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[step {step_number}] Battery: {battery[0]:.3f}", file=sys.stderr)

    # Drive forward when charged, reverse to charger when low
    if battery[0] < 0.5:
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
