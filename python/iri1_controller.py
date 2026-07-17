import sys

# --- LED color constants ---
LED_BLACK  = 0
LED_RED    = 1
LED_GREEN  = 2
LED_BLUE   = 3
LED_YELLOW = 4
LED_WHITE  = 5

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


def step(step_number, time, step_interval,
         prox, light, bluelight, redlight,
         contact, ground, ground_memory,
         battery, blue_battery, red_battery,
         encoder, compass,
         pos_x, pos_y):
    """
    Called every simulation step. Edit this function to program the robot.

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
    encoder       : list of 2 floats  -- wheel encoder readings (left, right)
    compass       : list of 1 float   -- compass heading (radians)
    pos_x, pos_y  : float             -- real robot position (for logging)

    Returns
    -------
    left_speed    : float in [-1000, 1000]
    right_speed   : float in [-1000, 1000]
    switch_light  : int or None  -- 0=switch nearest light OFF, 1=ON, None=no action
    switch_blue   : int or None  -- 0=switch nearest blue light OFF, 1=ON, None=no action
    switch_red    : int or None  -- 0=switch nearest red light OFF, 1=ON, None=no action
    leds          : list of 8 ints or None
                    Each entry: LED_BLACK/RED/GREEN/BLUE/YELLOW/WHITE, or -1 for no change.
                    None means leave all LEDs unchanged.
    """

    # Print all sensor readings to the terminal
    print(f"[time {time:.2f}]", file=sys.stderr)
    print(f"  PROX     : {[f'{v:.3f}' for v in prox]}", file=sys.stderr)
    print(f"  LIGHT    : {[f'{v:.3f}' for v in light]}", file=sys.stderr)
    print(f"  BLUELIGHT: {[f'{v:.3f}' for v in bluelight]}", file=sys.stderr)
    print(f"  REDLIGHT : {[f'{v:.3f}' for v in redlight]}", file=sys.stderr)
    print(f"  CONTACT  : {[f'{v:.3f}' for v in contact]}", file=sys.stderr)
    print(f"  GROUND   : {[f'{v:.3f}' for v in ground]}", file=sys.stderr)
    print(f"  GMEM     : {ground_memory[0]:.3f}", file=sys.stderr)
    print(f"  BATTERY  : {battery[0]:.3f}  BLUE: {blue_battery[0]:.3f}  RED: {red_battery[0]:.3f}", file=sys.stderr)
    print(f"  ENCODER  : {[f'{v:.3f}' for v in encoder]}", file=sys.stderr)
    print(f"  COMPASS  : {compass[0]:.3f}", file=sys.stderr)
    print(f"  POS      : ({pos_x:.3f}, {pos_y:.3f})", file=sys.stderr)

    # --- write your controller logic here ---
    left_speed   = 100
    right_speed  = 100
    switch_light = None
    switch_blue  = 0
    switch_red   = None
    leds         = [LED_BLACK]   # e.g. [LED_RED]*8  or  [-1,-1,-1,LED_GREEN,-1,-1,-1,-1]

    return left_speed, right_speed, switch_light, switch_blue, switch_red, leds


# --- communication loop (do not edit below this line) ---
def _parse(parts, start, n):
    return [float(parts[start + i]) for i in range(n)], start + n


for line in sys.stdin:
    parts = line.strip().split()
    expected = 3 + N_PROX + N_LIGHT + N_BLUELIGHT + N_REDLIGHT + N_CONTACT \
               + N_GROUND + N_GROUNDMEM + N_BATTERY + N_BLUEBATTERY \
               + N_REDBATTERY + N_ENCODER + N_COMPASS + 2
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
    pos_x  = float(parts[idx]);     idx += 1
    pos_y  = float(parts[idx]);     idx += 1

    result = step(step_number, time, step_interval,
                  prox, light, bluelight, redlight,
                  contact, ground, ground_memory,
                  battery, blue_battery, red_battery,
                  encoder, compass, pos_x, pos_y)

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
