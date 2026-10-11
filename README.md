# ATmega328P Bare-Metal Embedded C

A progressive collection of **Embedded C exercises for the ATmega328P microcontroller**, moving from GPIO fundamentals to LCD interfacing, ADC, timers, interrupts, and an integrated temperature-control system.

The repository is intended as a practical reference for learning how microcontroller peripherals work at register level. Proteus project files and circuit screenshots can be paired with each exercise so that the code can be explored and simulated.

## Contents

| # | Exercise | Main concepts | Source |
|---:|---|---|---|
| 01 | GPIO LED Blink | GPIO output, timing | [`01-GPIO-LED-Blink.c`](01-GPIO-LED-Blink.c) |
| 02 | GPIO Running LEDs | Digital outputs, bit patterns | [`02-GPIO-Running-LEDs.c`](02-GPIO-Running-LEDs.c) |
| 03 | GPIO Input / Output | Digital input, output control | [`03-GPIO-Input-Output.c`](03-GPIO-Input-Output.c) |
| 04 | LCD 8-Bit Initialization | LCD setup, 8-bit interface | [`04-LCD-8Bit-Initialization.c`](04-LCD-8Bit-Initialization.c) |
| 05 | LCD 4-Bit State Control | LCD 4-bit interface, control signals | [`05-LCD-4Bit-State-Control.c`](05-LCD-4Bit-State-Control.c) |
| 06 | LCD 8-Bit Parallel Interface | Parallel communication with an LCD | [`06-LCD-8Bit-Parallel-Interface.c`](06-LCD-8Bit-Parallel-Interface.c) |
| 07 | ADC Polling | Analog-to-digital conversion, polling | [`07-ADC-Polling.c`](07-ADC-Polling.c) |
| 08 | Sensor Input + LCD Counter | Sensor input, ADC, LCD output | [`08-Sensor-Input-LCD-Counter.c`](08-Sensor-Input-LCD-Counter.c) |
| 09 | Timer Square-Wave Generator | Timer configuration, waveform generation | [`09-Timer-Square-Wave-Generator.c`](09-Timer-Square-Wave-Generator.c) |
| 10 | LCD Telemetry Display | Displaying measured or system values | [`10-LCD-Telemetry-Display.c`](10-LCD-Telemetry-Display.c) |
| 11 | ADC Interrupt-Driven Monitor | ADC, interrupts, event-driven monitoring | [`11-ADC-Interrupt-Driven-Monitor.c`](11-ADC-Interrupt-Driven-Monitor.c) |
| 12 | Temperature Control System | Sensor reading, decision logic, system integration | [`12-Temperature-Control-System.c`](12-Temperature-Control-System.c) |

## Learning path

The exercises are arranged to build up concepts step by step:

1. **GPIO and digital I/O** — configure pins and control external components.
2. **LCD interfacing** — initialize a display and communicate using 4-bit or 8-bit modes.
3. **ADC and sensors** — read analog signals and use sensor input in a program.
4. **Timers** — configure a hardware timer to generate a square wave.
5. **Interrupt-driven programming** — respond to peripheral events without relying only on polling.
6. **System integration** — combine sensor input, processing, and output in a temperature-control example.

## Development environment

- **Target MCU:** ATmega328P
- **Language:** Embedded C
- **Simulation:** Proteus, for exercises that include a Proteus project
- **Toolchain:** Use an AVR-compatible compiler and development environment that supports the ATmega328P.

The exact clock frequency, compiler settings, and peripheral configuration should be checked in each source file and its corresponding circuit.

## Running an exercise

1. Open the `.c` file for the exercise you want to study.
2. Review the comments and identify the peripherals and pins used.
3. Compile the program for the **ATmega328P** using your AVR toolchain.
4. If a Proteus project is provided, open the corresponding project in Proteus.
5. Load the generated `.hex` file into the simulated microcontroller if it is not already configured in the project.
6. Run the simulation and compare the observed behavior with the program's intended behavior.

> **Proteus note:** A PNG image is a preview only; it cannot replace the editable simulation project. When sharing a simulation, include the Proteus project and any required external design or library files. Remove machine-specific workspace files and unnecessary backups before committing.

## Proteus simulations and screenshots

For the best learning experience, pair each source file with its own simulation assets. A consistent layout could look like this:

```text
ATmega328P/
├── README.md
├── 01-GPIO-LED-Blink.c
├── 02-GPIO-Running-LEDs.c
├── ...
├── 12-Temperature-Control-System.c
└── simulations/
    ├── 01-GPIO-LED-Blink/
    │   ├── project.pdsprj
    │   └── preview.png
    ├── 02-GPIO-Running-LEDs/
    │   ├── project.pdsprj
    │   └── preview.png
    └── ...
```

This is a **suggested layout**. Update it to match the folders and filenames actually committed to the repository. If multiple exercises use the same circuit, document which program file to load for each case.

Once screenshots have been added to the repository, display them in this README using relative paths, for example:

```markdown
![GPIO LED Blink Proteus simulation](simulations/01-GPIO-LED-Blink/preview.png)
```

## Repository goals

- Provide readable examples of ATmega328P peripheral programming.
- Make it easier to connect Embedded C code with circuit behavior.
- Support practice and revision through small, focused exercises.
- Grow into a reference collection of source code, Proteus simulations, and visual results.

## Contributing

Suggestions, corrections, and improvements are welcome. If you find an issue, please include the exercise name, the development environment used, and the expected versus observed behavior.

## License

No license is specified here. Add a `LICENSE` file before describing this repository as open source, and choose a license that matches how you want others to use and redistribute your work.
