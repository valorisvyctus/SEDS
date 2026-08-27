# SEDS BPHC — Avionics Round 1 Induction Task

## Athena's Intern

**Name:** Chiranthan Radhakrishna
**ID:** 2026A7PS0163H

This repository contains my solutions for the SEDS BPHC Avionics Round 1 Induction Task, **Athena's Intern**.

The project is divided into two parts:

* **Task 1:** Finding the Sea Floor — Python-based depth sensor data processing and visualization.
* **Task 2:** Keeping Watch Over Odysseus — Arduino-based onboard monitoring system implemented as a state machine in Tinkercad.

---

# Task 1 — Finding the Sea Floor

## 1. Extracting the Data

The provided depth sensor dataset was loaded and processed using Python and Pandas.

The dataset contains depth readings recorded once every second. The program reads the depth data from `Depth Data.csv` and performs basic validation before processing it.

## 2. Handling Invalid Sensor Data

Real-world sensors can produce corrupted or invalid readings. The program identifies invalid depth values and prevents them from affecting the final visualization.

Invalid readings are replaced appropriately and interpolated using neighboring valid values so that the resulting time series remains continuous.

## 3. Noise Reduction

To reduce random sensor noise, a **5-point median filter** is applied to the processed depth readings.

A median filter was chosen because it is effective at reducing sudden erroneous spikes while preserving the general shape of the depth data.

## 4. Depth-Time Graph

The processed data is plotted as depth against time using Matplotlib.

The graph includes:

* Time on the x-axis
* Depth on the y-axis
* Appropriate labels
* A descriptive title
* Processed sensor readings

## 5. Animation

The graph is animated so that a new sensor reading is added every second, matching the sampling rate specified in the problem statement.

The animation uses Matplotlib's animation functionality with a 1-second interval between readings.

## Task 1 Files

```text
Task_1/
├── data_inspection.py
├── Depth Data.csv
└── Sensor_Data_Graph_Screenshot(2026A7PS0163H).jpg
```

---

# Task 2 — Keeping Watch Over Odysseus

Task 2 was implemented in **Tinkercad** using an Arduino and the required sensors and output devices.

## Components Used

The system uses:

* Arduino
* Ultrasonic distance sensor
* Light sensor
* 16×2 I2C LCD
* Push button
* LED
* Buzzer

## State Machine

The monitoring system consists of five states:

```text
OPEN SEA
    ↓
STORM
    ↓
WRECKED

OPEN SEA
    ↓
CHARYBDIS
    ↓
WRECKED

OPEN SEA ↔ ANCHOR DROPPED
```

### OPEN SEA

This is the default state when the simulation starts.

The ship is considered to be sailing normally.

### ANCHOR DROPPED

The push button toggles the anchor.

* First press → anchor is dropped.
* Second press → anchor is raised.
* While the anchor is dropped, the ship is protected from storm and Charybdis conditions.
* Dropping the anchor also resets the danger timer.

### STORM

Storm is triggered when the light sensor reading falls below half of its range.

While in this state:

* The current state is displayed on the LCD.
* The LED blinks.
* A five-second danger timer is active.

If the storm ends before five seconds, the ship returns to `OPEN SEA`.

### CHARYBDIS

Charybdis is triggered when the ultrasonic sensor detects an object closer than **100 cm**.

While in this state:

* The current state is displayed on the LCD.
* The buzzer sounds.
* A five-second danger timer is active.

If the ship moves away before five seconds, it returns to `OPEN SEA`.

### WRECKED

If the ship continuously remains in either `STORM` or `CHARYBDIS` for **five seconds**, it enters the `WRECKED` state.

Once wrecked, the state is permanent until the simulation is restarted.

## Simultaneous Hazards

If both Storm and Charybdis conditions occur at the same time, the state that is entered first remains active and its five-second timer continues.

## Sensor Thresholds

| Hazard    | Condition                                  |
| --------- | ------------------------------------------ |
| Storm     | Light sensor reading < half-scale          |
| Charybdis | Distance < 100 cm                          |
| Wrecked   | Hazard persists continuously for 5 seconds |

## Task 2 Files

```text
Task_2/
├── Arduino_Code/
│   └── Arduino.ino
│
└── Screenshots/
    ├── Anchor_Dropped.jpg
    ├── Arduino_Screenshot(2026A7PS0163H).jpg
    ├── Charybdis.jpg
    ├── Open_Sea.jpg
    ├── Storm.jpg
    └── Wrecked.jpg
```

---

# Repository Structure

```text
SEDS/
├── Task_1/
│   ├── data_inspection.py
│   ├── Depth Data.csv
│   └── Sensor_Data_Graph_Screenshot(2026A7PS0163H).jpg
│
├── Task_2/
│   ├── Arduino_Code/
│   │   └── Arduino.ino
│   │
│   └── Screenshots/
│       ├── Anchor_Dropped.jpg
│       ├── Arduino_Screenshot(2026A7PS0163H).jpg
│       ├── Charybdis.jpg
│       ├── Open_Sea.jpg
│       ├── Storm.jpg
│       └── Wrecked.jpg
│
├── README.md
└── .gitignore
```

---

# Tools and Libraries

### Task 1

* Python
* Pandas
* Matplotlib
* NumPy
* pathlib

### Task 2

* Arduino
* Tinkercad
* C/C++ based Arduino programming
* LiquidCrystal I2C library

---

# Conclusion

The project combines sensor-data processing with an embedded monitoring system.

Task 1 focuses on processing noisy depth sensor data and presenting it through an animated visualization, while Task 2 implements a state-based onboard monitoring system capable of detecting environmental hazards and responding appropriately.

The code is commented and organized by task for easier evaluation and reproducibility.

