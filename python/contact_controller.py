import sys


def step(step_number, time, step_interval, contact):
    """
    Parameters
    ----------
    contact : list of float  -- contact sensor readings (0=no contact, 1=contact)

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[step {step_number}] Contact: {[f'{v:.2f}' for v in contact]}", file=sys.stderr)

    left_speed  = 750
    right_speed = 750
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 3:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    contact       = [float(v) for v in parts[3:]]
    left, right   = step(step_number, time, step_interval, contact)
    print(f"{left} {right}", flush=True)
