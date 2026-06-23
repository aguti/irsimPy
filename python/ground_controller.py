import sys


def step(step_number, time, step_interval, ground, ground_memory):
    """
    Parameters
    ----------
    ground        : list of 3 floats  -- ground sensor readings [0..1]
    ground_memory : list of 1 float   -- ground memory sensor reading [0..1]

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[step {step_number}] Ground: {[f'{v:.2f}' for v in ground]}  Memory: {[f'{v:.2f}' for v in ground_memory]}", file=sys.stderr)

    left_speed  = 200
    right_speed = 200
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
GROUND_N = 3
GMEM_N   = 1

for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 3 + GROUND_N + GMEM_N:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    ground        = [float(v) for v in parts[3:3 + GROUND_N]]
    ground_memory = [float(v) for v in parts[3 + GROUND_N:3 + GROUND_N + GMEM_N]]
    left, right   = step(step_number, time, step_interval, ground, ground_memory)
    print(f"{left} {right}", flush=True)
