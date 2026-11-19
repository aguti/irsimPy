import sys
import math
import struct

# --- LED color constants ---
LED_BLACK  = 0
LED_RED    = 1
LED_GREEN  = 2
LED_BLUE   = 3
LED_YELLOW = 4
LED_WHITE  = 5

# ============================================================
# ARENA CONFIGURATION (edit to change the environment)
# Lights, ground areas, robots and sensors come from the param file
# (e.g. paramFiles/subExamples/paramFileSubsumptionGarbage.txt)
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

# ============================================================
# SUBSUMPTION ARCHITECTURE
# ============================================================
# Levels of competence, ordered by priority (0 = highest)
AVOID_PRIORITY    = 0
RELOAD_PRIORITY   = 1
FORAGE_PRIORITY   = 2
NAVIGATE_PRIORITY = 3
BEHAVIORS         = 4

PROXIMITY_THRESHOLD      = 0.3   # threshold to avoid obstacles
BATTERY_THRESHOLD        = 0.5   # threshold to define the battery discharged
NAVIGATE_LIGHT_THRESHOLD = 0.9   # threshold to reduce the speed of the robot

SPEED = 500.0

# Proximity sensor directions (radians) of the e-puck
PROX_DIRECTIONS = [0.2967, 0.8727, 1.5708, 2.6180, 3.6652, 4.7124, 5.4105, 5.9865]

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
N_ENCODER     = 2
N_COMPASS     = 1

# Set every step from the param file (WRITE TO FILE)
write_to_file = 0

# [left_speed, right_speed, active] for each level of competence.
# Only "active" is reset every step (as in the C++ controller)
activation_table = [[0.0, 0.0, 0.0] for _ in range(BEHAVIORS)]

# Color of the robot LEDs, sent to the simulator at the end of each step
robot_leds = LED_BLACK


def f32(x):
    """Round to single precision, like a C++ float."""
    return struct.unpack("f", struct.pack("f", x))[0]


def set_all_leds(color):
    """Set all the LEDs of the robot to color (as SetAllColoredLeds in C++)."""
    global robot_leds
    robot_leds = color


def write_output(filename, text):
    """Append a line to outputFiles/<filename> (only if WRITE TO FILE = 1)."""
    if write_to_file:
        with open(f"outputFiles/{filename}", "a") as f:
            f.write(text + "\n")


def obstacle_avoidance(priority, activation_table, time, prox):
    """Turn away from the obstacles detected by the proximity sensors."""

    # Vector sum of the proximity readings and maximum reading
    repelent_x = sum(p * math.cos(d) for p, d in zip(prox, PROX_DIRECTIONS))
    repelent_y = sum(p * math.sin(d) for p, d in zip(prox, PROX_DIRECTIONS))
    max_prox   = max(max(prox), 0.0)

    # Repelent angle, normalised to [-pi, pi] (a float in the C++ controller)
    repelent = f32(math.atan2(repelent_y, repelent_x))
    repelent = f32(repelent - math.pi)
    while repelent > math.pi:
        repelent = f32(repelent - 2 * math.pi)
    while repelent < -math.pi:
        repelent = f32(repelent + 2 * math.pi)

    if max_prox > PROXIMITY_THRESHOLD:
        set_all_leds(LED_GREEN)

        c_linear = 1.0
        c1       = SPEED / math.pi
        v_linear  = SPEED * c_linear * f32(math.cos(repelent / 2))
        v_angular = repelent

        activation_table[priority][:] = [v_linear - c1 * v_angular,
                                         v_linear + c1 * v_angular,
                                         1.0]

    left, right, active = activation_table[priority]
    write_output("avoidOutput",
                 f"{time:2.4f} " + " ".join(f"{p:2.4f}" for p in prox)
                 + f" {max_prox:2.4f} {repelent:2.4f} {active:2.4f} {left:2.4f} {right:2.4f}")


def go_load(priority, activation_table, inhibitors, time, battery, light):
    """When the battery is low, turn towards the light (charger)."""

    if battery[0] < BATTERY_THRESHOLD:
        set_all_leds(LED_RED)

        # Inhibit forage while the battery is low
        inhibitors["batt_to_forage"] = 0.0

        # If not pointing to the light
        if light[0] * light[7] == 0.0:
            activation_table[priority][2] = 1.0

            light_left  = light[0] + light[1] + light[2] + light[3]
            light_right = light[4] + light[5] + light[6] + light[7]

            if light_left > light_right:
                # Turn left
                activation_table[priority][0] = -SPEED
                activation_table[priority][1] = SPEED
            else:
                # Turn right
                activation_table[priority][0] = SPEED
                activation_table[priority][1] = -SPEED

    left, right, active = activation_table[priority]
    write_output("batteryOutput",
                 f"{time:2.4f} {battery[0]:2.4f} " + " ".join(f"{l:2.4f}" for l in light)
                 + f" {active:2.4f} {left:2.4f} {right:2.4f}")


def forage(priority, activation_table, inhibitors, time, ground_memory, light):
    """When carrying a virtual puck, go away from the light (to the nest)."""

    # If with a virtual puck
    if ground_memory[0] * inhibitors["batt_to_forage"] == 1.0:
        set_all_leds(LED_BLUE)

        # Go opposite to the light
        if light[3] * light[4] == 0.0:
            activation_table[priority][2] = 1.0

            light_left  = light[0] + light[1] + light[2] + light[3]
            light_right = light[4] + light[5] + light[6] + light[7]

            if light_left > light_right:
                activation_table[priority][0] = SPEED
                activation_table[priority][1] = -SPEED
            else:
                activation_table[priority][0] = -SPEED
                activation_table[priority][1] = SPEED

    left, right, active = activation_table[priority]
    write_output("forageOutput",
                 f"{time:2.4f} {inhibitors['batt_to_forage']:2.4f} {ground_memory[0]:2.4f} "
                 + " ".join(f"{l:2.4f}" for l in light)
                 + f" {active:2.4f} {left:2.4f} {right:2.4f}")


def navigate(priority, activation_table, time, light):
    """Go straight, slowly when close to the light."""
    total_light = sum(light)

    if total_light >= NAVIGATE_LIGHT_THRESHOLD:
        activation_table[priority][:] = [SPEED / 4, SPEED / 4, 1.0]
    else:
        activation_table[priority][:] = [SPEED, SPEED, 1.0]

    left, right, active = activation_table[priority]
    write_output("navigateOutput", f"{time:2.4f} {total_light:2.4f} {active:2.4f} {left:2.4f} {right:2.4f}")


def coordinator(activation_table, time):
    """Select the active behavior with the highest priority."""
    for behavior in range(BEHAVIORS):
        if activation_table[behavior][2] == 1.0:
            break

    left_speed, right_speed = activation_table[behavior][0], activation_table[behavior][1]

    print(f"{behavior} {left_speed:2.4f} {right_speed:2.4f} ", file=sys.stderr)
    write_output("coordinatorOutput", f"{time:2.4f} {behavior} {left_speed:2.4f} {right_speed:2.4f} ")
    return left_speed, right_speed


def step(step_number, time, step_interval,
         prox, light, bluelight, redlight,
         contact, ground, ground_memory,
         battery, blue_battery, red_battery,
         encoder, compass,
         RobotSim_pos_x, RobotSim_pos_y, RobotSim_orientation):
    """
    Called every simulation step.

    Parameters: same sensors as iri1_controller.py, plus
    RobotSim_orientation : float -- simulator orientation of the robot (radians)

    Returns
    -------
    left_speed, right_speed : float in [-1000, 1000]
    switch_light, switch_blue, switch_red : None (not used)
    leds : list with one LED color for all LEDs (set by the behaviors)
    """
    for behavior in activation_table:
        behavior[2] = 0.0

    # Release inhibitors
    inhibitors = {"batt_to_forage": 1.0}

    # Set LEDs to BLACK (each behavior may change them)
    set_all_leds(LED_BLACK)

    # Execute the levels of competence
    obstacle_avoidance(AVOID_PRIORITY, activation_table, time, prox)
    go_load(RELOAD_PRIORITY, activation_table, inhibitors, time, battery, light)
    forage(FORAGE_PRIORITY, activation_table, inhibitors, time, ground_memory, light)
    navigate(NAVIGATE_PRIORITY, activation_table, time, light)

    # Execute coordinator
    left_speed, right_speed = coordinator(activation_table, time)

    write_output("robotPosition",
                 f"{time:2.4f} {RobotSim_pos_x:2.4f} {RobotSim_pos_y:2.4f} {RobotSim_orientation:2.4f}")
    write_output("robotWheels", f"{time:2.4f} {left_speed:2.4f} {right_speed:2.4f} ")

    return left_speed, right_speed, None, None, None, [robot_leds]


# --- communication loop (do not edit below this line) ---
def _parse(parts, start, n):
    return [float(parts[start + i]) for i in range(n)], start + n


for line in sys.stdin:
    parts = line.strip().split()
    expected = 3 + N_PROX + N_LIGHT + N_BLUELIGHT + N_REDLIGHT + N_CONTACT \
               + N_GROUND + N_GROUNDMEM + N_BATTERY + N_BLUEBATTERY \
               + N_REDBATTERY + N_ENCODER + N_COMPASS + 4
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
    encoder,      idx = _parse(parts, idx, N_ENCODER)
    compass,      idx = _parse(parts, idx, N_COMPASS)
    RobotSim_pos_x       = float(parts[idx]);  idx += 1
    RobotSim_pos_y       = float(parts[idx]);  idx += 1
    RobotSim_orientation = float(parts[idx]);  idx += 1
    write_to_file        = int(parts[idx]);    idx += 1

    result = step(step_number, time, step_interval,
                  prox, light, bluelight, redlight,
                  contact, ground, ground_memory,
                  battery, blue_battery, red_battery,
                  encoder, compass,
                  RobotSim_pos_x, RobotSim_pos_y, RobotSim_orientation)

    left, right    = result[0], result[1]
    switch_light   = result[2] if len(result) > 2 else None
    switch_blue    = result[3] if len(result) > 3 else None
    switch_red     = result[4] if len(result) > 4 else None
    leds           = result[5] if len(result) > 5 else None

    sw_l = switch_light if switch_light is not None else -1
    sw_b = switch_blue  if switch_blue  is not None else -1
    sw_r = switch_red   if switch_red   is not None else -1
    if leds is None:
        led_vals = [-1] * 8
    elif len(leds) == 1:
        led_vals = leds * 8          # broadcast single value to all 8 LEDs
    elif len(leds) != 8:
        raise ValueError(f"leds must be None, a single value, or a list of 8 — got {len(leds)}")
    else:
        led_vals = leds
    led_str = " ".join(str(v) for v in led_vals)
    print(f"{left} {right} {sw_l} {sw_b} {sw_r} {led_str}", flush=True)
