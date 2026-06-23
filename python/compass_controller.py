import sys


def step(step_number, time, step_interval, encoder, compass):
    """
    Parameters
    ----------
    encoder : list of 2 floats  -- left and right wheel encoder readings
    compass : list of floats    -- compass sensor readings (heading)

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[step {step_number}] Encoder: {[f'{v:.2f}' for v in encoder]}  Compass: {[f'{v:.2f}' for v in compass]}", file=sys.stderr)

    left_speed  = 200
    right_speed = -200
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
ENCODER_N = 2

for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 3 + ENCODER_N:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    encoder       = [float(v) for v in parts[3:3 + ENCODER_N]]
    compass       = [float(v) for v in parts[3 + ENCODER_N:]]
    left, right   = step(step_number, time, step_interval, encoder, compass)
    print(f"{left} {right}", flush=True)
