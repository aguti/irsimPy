import sys

# ============================================================
# ARENA CONFIGURATION (edit to change the environment)
# Lights, ground areas, robots and sensors come from the param file
# (e.g. paramFiles/braitenbergExamples/braitenbergParam.txt)
# ============================================================
ARENA_CELLS_X = 20      # grid columns (arena width discretisation)
ARENA_CELLS_Y = 20      # grid rows    (arena height discretisation)
ARENA_WIDTH   = 3.0     # arena width  in metres
ARENA_HEIGHT  = 3.0     # arena height in metres

# Map layout: list of ARENA_CELLS_Y strings, each ARENA_CELLS_X characters.
# % = wall (obstacle),  # = free space.  First string = top row of the arena.
ARENA_MAP = [
    "%%%%%%%%%%%%%%%%%%%%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%##################%",
    "%%%%%%%%%%%%%%%%%%%%",
]

# --- sensor sizes (fixed for the e-puck robot) ---
N_PROX        = 8
N_LIGHT       = 8
N_BLUELIGHT   = 8
N_REDLIGHT    = 8
N_CONTACT     = 8
N_GROUND      = 3
N_GROUNDMEM   = 1
N_BATTERY     = 1
N_BLUEBATTERY = 1
N_REDBATTERY  = 1


def step(step_number, time, step_interval,
         prox, light, bluelight, redlight,
         contact, ground, ground_memory,
         battery, blue_battery, red_battery,
         RobotSim_pos_x, RobotSim_pos_y, RobotSim_orientation):
    """
    Called every simulation step. Edit this function to program the vehicle.

    Parameters
    ----------
    prox          : list of 8 floats  -- proximity sensors [0=far .. 1=close]
    light         : list of 8 floats  -- light sensors [0=dark .. 1=bright]
    bluelight     : list of 8 floats  -- blue light sensors
    redlight      : list of 8 floats  -- red light sensors
    contact       : list of 8 floats  -- contact sensors [0=free, 1=contact]
    ground        : list of 3 floats  -- ground sensors [0=black .. 1=white]
    ground_memory : list of 1 float   -- ground memory sensor
    battery       : list of 1 float   -- battery level [0=empty .. 1=full]
    blue_battery  : list of 1 float   -- blue battery level
    red_battery   : list of 1 float   -- red battery level
    RobotSim_pos_x, RobotSim_pos_y : float  -- simulator position of the robot
    RobotSim_orientation           : float  -- simulator orientation (radians)

    Returns
    -------
    (left_output, right_output) : floats in [0, 1]
        0.5 = stop, 1 = full speed forward, 0 = full speed backward.
        Values outside [0, 1] are saturated by the simulator.
    """

    # Phase 1: sensors
    print("LIGHT: " + "".join(f"{v:1.3f} " for v in light), file=sys.stderr)

    # Write robot position (always, as in the C++ controller)
    with open("outputFiles/robotPosition", "a") as f:
        f.write(f" {time:2.4f} {RobotSim_pos_x:2.4f} {RobotSim_pos_y:2.4f} {RobotSim_orientation:2.4f}\n")

    # Phase 3: actuation (uncomment one test, comment the others)

    # TEST 1-8: Light (-0.7, 0.7), Robot (0.0, 0.0, 1.57)
    # Test 1: V2 - Contra Lateral + Light
    left_output  = 0.5 + light[7]
    right_output = 0.5 + light[0]

    # Test 2: V2 - Lateral + Light
    #left_output  = 0.5 + light[0]
    #right_output = 0.5 + light[7]

    # Test 3: V2 - Contra Lateral all + light
    #light_left  = light[0] + light[1] + light[2] + light[3]
    #light_right = light[4] + light[5] + light[6] + light[7]
    #left_output  = 0.5 + light_right
    #right_output = 0.5 + light_left

    # Test 4: V2 - Lateral all + light
    #light_left  = light[0] + light[1] + light[2] + light[3]
    #light_right = light[4] + light[5] + light[6] + light[7]
    #left_output  = 0.5 + light_left
    #right_output = 0.5 + light_right

    # Test 5: V2 - Lateral - Light
    # Stops with 0.7 and unstable with 0.75
    #left_output  = 0.5 + (0.5 - light[0])
    #right_output = 0.5 + (0.5 - light[7])

    # Test 6: V2 - Contra-Lateral - Light
    #left_output  = 0.5 + (0.5 - light[7])
    #right_output = 0.5 + (0.5 - light[0])

    # Test 7: V2 - Contra-Lateral all - Light
    #light_left  = light[0] + light[1] + light[2] + light[3]
    #light_right = light[4] + light[5] + light[6] + light[7]
    #left_output  = 0.5 + (0.5 - light_right)
    #right_output = 0.5 + (0.5 - light_left)

    # Test 8: V2 - Lateral all - Light
    #light_left  = light[0] + light[1] + light[2] + light[3]
    #light_right = light[4] + light[5] + light[6] + light[7]
    #left_output  = 0.5 + (0.95 - light_left)
    #right_output = 0.5 + (0.95 - light_right)

    # TEST 9: Light (-0.7, 0.7) & (0.7, -0.7), Robot (0.0, 0.0, 1.57)
    # Test 9: V4 - Back&Forth - Light
    #light_left  = light[0] + light[1]    # + light[2]
    #light_right = light[7] + light[6]    # + light[5]

    #if light_right > 0.5:
        #light_right = 1 - light_right

    #left_output  = 0.6 + light_right
    #right_output = 0.6 + light_left

    return left_output, right_output


# --- communication loop (do not edit below this line) ---
def _parse(parts, start, n):
    return [float(parts[start + i]) for i in range(n)], start + n


for line in sys.stdin:
    parts = line.strip().split()
    expected = 3 + N_PROX + N_LIGHT + N_BLUELIGHT + N_REDLIGHT + N_CONTACT \
               + N_GROUND + N_GROUNDMEM + N_BATTERY + N_BLUEBATTERY \
               + N_REDBATTERY + 3
    if len(parts) < expected:
        continue

    step_number   = int(parts[0])
    time          = float(parts[1])
    step_interval = float(parts[2])
    idx           = 3

    prox,         idx = _parse(parts, idx, N_PROX)
    light,        idx = _parse(parts, idx, N_LIGHT)
    bluelight,    idx = _parse(parts, idx, N_BLUELIGHT)
    redlight,     idx = _parse(parts, idx, N_REDLIGHT)
    contact,      idx = _parse(parts, idx, N_CONTACT)
    ground,       idx = _parse(parts, idx, N_GROUND)
    ground_memory,idx = _parse(parts, idx, N_GROUNDMEM)
    battery,      idx = _parse(parts, idx, N_BATTERY)
    blue_battery, idx = _parse(parts, idx, N_BLUEBATTERY)
    red_battery,  idx = _parse(parts, idx, N_REDBATTERY)
    RobotSim_pos_x       = float(parts[idx]);  idx += 1
    RobotSim_pos_y       = float(parts[idx]);  idx += 1
    RobotSim_orientation = float(parts[idx]);  idx += 1

    left, right = step(step_number, time, step_interval,
                       prox, light, bluelight, redlight,
                       contact, ground, ground_memory,
                       battery, blue_battery, red_battery,
                       RobotSim_pos_x, RobotSim_pos_y, RobotSim_orientation)
    print(f"{left!r} {right!r}", flush=True)
