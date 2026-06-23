import sys


def step(step_number, time, step_interval, encoder, pos_x, pos_y):
    """
    Parameters
    ----------
    encoder : list of 2 floats  -- left and right wheel encoder readings
    pos_x   : float             -- robot real X position (for reference)
    pos_y   : float             -- robot real Y position (for reference)

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[step {step_number}] Encoder: {[f'{v:.2f}' for v in encoder]}  Pos: ({pos_x:.3f}, {pos_y:.3f})", file=sys.stderr)

    left_speed  = -500
    right_speed = -500
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 7:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    encoder       = [float(parts[3]), float(parts[4])]
    pos_x         = float(parts[5])
    pos_y         = float(parts[6])
    left, right   = step(step_number, time, step_interval, encoder, pos_x, pos_y)
    print(f"{left} {right}", flush=True)
