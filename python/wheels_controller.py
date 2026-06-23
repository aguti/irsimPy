import sys


def step(step_number, time, step_interval):
    """
    Called every simulation step.

    Parameters
    ----------
    step_number   : int   -- current simulation step
    time          : float -- elapsed simulation time (seconds)
    step_interval : float -- duration of one step (seconds)

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    left_speed  = -1000
    right_speed = -1000
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) != 3:
        continue
    left, right = step(int(parts[0]), float(parts[1]), float(parts[2]))
    print(f"{left} {right}", flush=True)
