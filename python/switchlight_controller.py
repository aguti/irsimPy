import sys

INIT_X     = 0.0  # initial x position (metres)
INIT_Y     = 0.0  # initial y position (metres)
INIT_THETA = 0.0  # initial orientation (radians)


def step(step_number, time, step_interval, light):
    """
    Parameters
    ----------
    light : list of 8 floats  -- light sensor readings [0..1]

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    switch_cmd : int or None
        0 = switch nearest light OFF
        1 = switch nearest light ON
        None = do nothing (omit from return)
    """
    print(f"[time {time}] Light: {[f'{v:.4f}' for v in light]}", file=sys.stderr)

    total_light = light[0] + light[7]

    switch_cmd = None
    if total_light >= 0.9:
        switch_cmd = 0  # switch off nearest light

    if light[0] * light[7] == 0.0:
        light_left  = sum(light[0:4])
        light_right = sum(light[4:8])
        if light_left > light_right:
            left_speed, right_speed = -500, 500
        else:
            left_speed, right_speed = 500, -500
    else:
        left_speed, right_speed = 500, 500

    return left_speed, right_speed, switch_cmd


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 3:
        continue
    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    light         = [float(v) for v in parts[3:]]
    result        = step(step_number, time, step_interval, light)
    left, right   = result[0], result[1]
    switch_cmd    = result[2] if len(result) > 2 else None
    if switch_cmd is not None:
        print(f"{left} {right} {switch_cmd}", flush=True)
    else:
        print(f"{left} {right}", flush=True)
