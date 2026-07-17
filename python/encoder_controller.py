import sys

INIT_X     = 0.0  # initial x position (metres)
INIT_Y     = 0.0  # initial y position (metres)
INIT_THETA = 0.0  # initial orientation (radians)


def step(step_number, time, step_interval, encoder, RobotSim_pos_x, RobotSim_pos_y):
    """
    Parameters
    ----------
    encoder        : list of 2 floats  -- left and right wheel encoder readings
    RobotSim_pos_x : float             -- simulator X position of the robot
    RobotSim_pos_y : float             -- simulator Y position of the robot

    Returns
    -------
    (left_speed, right_speed) : floats in [-1000, 1000]
    """
    print(f"[time {time}] Encoder: {[f'{v:.4f}' for v in encoder]}  Pos: ({RobotSim_pos_x:.4f}, {RobotSim_pos_y:.4f})", file=sys.stderr)

    left_speed  = 300
    right_speed = 300
    return left_speed, right_speed


# --- communication loop (do not edit below this line) ---
for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) < 7:
        continue
    step_number    = int(parts[0])
    time           = float(parts[1])
    step_interval  = float(parts[2])
    encoder        = [float(parts[3]), float(parts[4])]
    RobotSim_pos_x = float(parts[5])
    RobotSim_pos_y = float(parts[6])
    left, right    = step(step_number, time, step_interval, encoder, RobotSim_pos_x, RobotSim_pos_y)
    print(f"{left} {right}", flush=True)
