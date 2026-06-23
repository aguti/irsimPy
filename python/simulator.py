import argparse
import os
import subprocess

EXPERIMENTS = {
    1:  "Test Wheels",
    2:  "Test Contact Sensor",
    3:  "Test Proximity Sensor",
    4:  "Test Light Sensor",
    5:  "Test Blue Light Sensor",
    6:  "Test Red Light Sensor",
    7:  "Test Switch Light",
    8:  "Test Ground Sensor",
    9:  "Test Battery Sensor",
    10: "Test Encoder Sensor",
    11: "Test Compass Sensor",
    20: "Braitenberg Vehicle 2",
    21: "Neuron Experiment",
    22: "Subsumption Light",
    23: "Subsumption Garbage",
    24: "CTRNN Experiment",
    30: "IRI1",
    31: "IRI2",
    32: "IRI3",
}

_SIM_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_BINARY  = os.path.join(_SIM_DIR, "irsim")


class Simulator:
    """Python wrapper around the irsim binary.

    Parameters
    ----------
    experiment : int
        Experiment ID (see EXPERIMENTS dict for valid values).
    seed : int, optional
        Random seed (-s flag).
    visual : bool
        Show graphical window (default True). Pass False for headless runs.
    evolutionary : bool
        Run in evolutionary mode (-e flag).
    param_file : str, optional
        Path to a parameter file (-p flag).
    chromosome_file : str, optional
        Path to a chromosome file (-c flag).
    produce_frames : bool
        Save frames to disk (-f flag).
    """

    def __init__(
        self,
        experiment,
        seed=None,
        visual=True,
        evolutionary=False,
        param_file=None,
        chromosome_file=None,
        produce_frames=False,
    ):
        if experiment not in EXPERIMENTS:
            raise ValueError(
                f"Unknown experiment ID {experiment!r}. "
                f"Valid IDs: {sorted(EXPERIMENTS)}"
            )
        self.experiment = experiment
        self.seed = seed
        self.visual = visual
        self.evolutionary = evolutionary
        self.param_file = param_file
        self.chromosome_file = chromosome_file
        self.produce_frames = produce_frames

    def _build_command(self):
        cmd = [_BINARY, "-E", str(self.experiment)]
        if self.seed is not None:
            cmd += ["-s", str(self.seed)]
        if not self.visual:
            cmd += ["-v"]
        if self.evolutionary:
            cmd += ["-e"]
        if self.produce_frames:
            cmd += ["-f"]
        if self.param_file:
            cmd += ["-p", self.param_file]
        if self.chromosome_file:
            cmd += ["-c", self.chromosome_file]
        return cmd

    def run(self):
        """Launch the simulator and block until it exits."""
        if not os.path.isfile(_BINARY):
            raise FileNotFoundError(
                f"Simulator binary not found at '{_BINARY}'. "
                "Compile first: ./bootstrap.sh && make"
            )
        subprocess.run(self._build_command(), cwd=_SIM_DIR)


def _print_experiments():
    print("\nEXPERIMENTS IDENTIFIERS\n")
    groups = [
        (range(1, 12),  None),
        (range(20, 25), None),
        (range(30, 33), None),
    ]
    for group_range, _ in groups:
        for eid in group_range:
            if eid in EXPERIMENTS:
                print(f"{eid:>3} - {EXPERIMENTS[eid].upper()}")
        print()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="irsim Python launcher")
    parser.add_argument("-E", dest="experiment", type=int, nargs="?",
                        const=-1, default=None,
                        help="Experiment ID (omit value to list all experiments)")
    parser.add_argument("-s", dest="seed", type=int, default=None,
                        help="Random seed")
    parser.add_argument("-v", dest="no_visual", action="store_true",
                        help="Disable graphical window (headless)")
    parser.add_argument("-e", dest="evolutionary", action="store_true",
                        help="Run in evolutionary mode")
    parser.add_argument("-f", dest="produce_frames", action="store_true",
                        help="Save frames to disk")
    parser.add_argument("-p", dest="param_file", default=None,
                        help="Parameter file path")
    parser.add_argument("-c", dest="chromosome_file", default=None,
                        help="Chromosome file path")
    args = parser.parse_args()

    if args.experiment is None or args.experiment == -1:
        _print_experiments()
    else:
        Simulator(
            experiment=args.experiment,
            seed=args.seed,
            visual=not args.no_visual,
            evolutionary=args.evolutionary,
            param_file=args.param_file,
            chromosome_file=args.chromosome_file,
            produce_frames=args.produce_frames,
        ).run()
