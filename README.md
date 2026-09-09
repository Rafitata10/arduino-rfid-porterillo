# Arduino RFID Porterillo

RFID-based access control system for Arduino using an **MFRC522 RFID reader** and a **microSD card UID database**.

The project implements an access control system in which RFID tags are authenticated against a list of authorized UIDs. The UID database is loaded from a microSD card using **SoftSPI**, while the MFRC522 RFID reader communicates through the Arduino's **hardware SPI** interface.

This architecture allows both devices to operate simultaneously without the SPI communication conflicts that can occur when multiple peripherals share the hardware SPI bus.

## 🎥 Demo

A demonstration of the RFID access control system can be seen in the following video:

[![Project demo](https://img.youtube.com/vi/is7qi1KdosM/hqdefault.jpg)](https://www.youtube.com/watch?v=is7qi1KdosM)

*Click the image to watch the full video on YouTube.*

## Features

* RFID tag authentication using the **MFRC522** reader.
* UID database stored on a **microSD card**.
* **Hardware SPI** communication for the RFID reader.
* **SoftSPI** communication for the microSD card using the **SdFat** library.
* Automatic fallback to a default UID database if the microSD card cannot be accessed.
* Serial Monitor output for debugging and system status.
* Visual access feedback using separate LEDs for authorized and unauthorized tags.
* Separate firmware for:

  * Reading RFID UIDs.
  * Registering RFID tags.
  * Running the complete access control system.

## System Architecture

The project uses two different SPI interfaces:

```text
                    Arduino Nano
                         │
             ┌───────────┴───────────┐
             │                       │
       Hardware SPI              SoftSPI
             │                       │
             ▼                       ▼
       MFRC522 RFID              microSD
          Reader                  Module
             │                       │
             │                  UIDs.txt
             │                       │
             └───────────┬───────────┘
                         │
                  UID Authentication
                         │
                  ┌──────┴──────┐
                  │             │
              Authorized    Unauthorized
                  │             │
              Blue LED       Red LED
```

The MFRC522 uses the Arduino's standard hardware SPI pins, while the microSD module uses three independent GPIO pins configured as a software SPI interface.

This prevents the SD card from interfering with the RFID reader's SPI communication.

## SPI Configuration

### MFRC522 — Hardware SPI

| Signal   | Arduino Nano |
| -------- | -----------: |
| MOSI     |          D11 |
| MISO     |          D12 |
| SCK      |          D13 |
| SS / SDA |          D10 |
| RST      |           D9 |

### microSD — SoftSPI

| Signal | Arduino Nano |
| ------ | -----------: |
| MOSI   |           D3 |
| MISO   |           D2 |
| SCK    |           D4 |
| CS     |           D5 |

### LEDs

| Function       | Arduino Nano |
| -------------- | -----------: |
| Access granted |           D7 |
| Access denied  |           D6 |

## Hardware

* Arduino Nano / Uno
* MFRC522 RFID reader
* microSD card module
* microSD card
* Blue LED
* Red LED
* Current-limiting resistors for LEDs
* RFID cards / key fobs / tags

## Software

The project is written in **C++** and can be developed using Arduino IDE or other Arduino-compatible development environments.

### Libraries

The main libraries used are:

* `MFRC522`
* `SdFat`
* `SPI`

## Project Structure

```text
arduino-rfid-porterillo/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── src/
│   ├── ID_reader.cpp
│   ├── UIDs_keeper.cpp
│   └── Porterillo.cpp
│
└── docs/
    ├── spi_architecture.md
    ├── wiring.md
    └── firmware_flow.md
```

### `ID_reader.cpp`

Basic RFID reader firmware.

It detects RFID tags using the MFRC522 and prints their UIDs through the Serial Monitor. This program is useful for identifying new cards and key fobs before adding them to the authorized UID database.

### `UIDs_keeper.cpp`

UID registration utility.

It reads RFID tags and keeps track of the UIDs detected during the current session, avoiding duplicate entries.

### `Porterillo.cpp`

Main access control firmware.

It initializes both the microSD card and RFID reader, loads the authorized UID database and then checks each detected RFID tag against the stored list.

If the SD card cannot be initialized or the UID file cannot be opened, the firmware automatically switches to the default UID database stored in the source code.

## UID Database

Authorized RFID UIDs are stored in a text file named:

```text
UIDs.txt
```

Each UID is stored on a separate line.

Example:

```text
A6AC4404
C1A0EB6F
03E1ED6F
BE9AED6F
2CFD4304
4487ED6F
```

The firmware reads the file line by line during startup and stores the UIDs in memory.

The maximum number of UIDs loaded by the current implementation is **50**.

## Access Control

When an RFID tag is detected, its UID is converted to hexadecimal format and compared against the authorized UID list.

### Authorized tag

If the UID is found in the database:

```text
Leído UID: 4487ED6F
-> Acceso PERMITIDO.
```

The blue LED is activated to indicate that access has been granted.

### Unauthorized tag

If the UID is not present in the database:

```text
Leído UID: XXXXXXXX
-> Acceso DENEGADO.
```

The red LED is activated to indicate that access has been denied.

## SD Card Fallback

The system is designed to continue operating even if the microSD card is unavailable.

At startup, the firmware attempts to initialize the SD card using SoftSPI and open `UIDs.txt`.

```text
SD available
     │
     ▼
Load UIDs.txt
     │
     ▼
Use SD database
```

If initialization or file access fails:

```text
SD unavailable
     │
     ▼
Use default UID list
     │
     ▼
Continue RFID authentication
```

This provides a simple fallback mechanism that prevents the access control system from becoming completely unusable due to an SD card failure.

## Documentation

Additional technical documentation is available in the `/docs` directory:

* [`SPI Architecture`](docs/spi_architecture.md) — Hardware SPI and SoftSPI communication architecture.
* [`Wiring Diagram`](docs/wiring.md) — Arduino, MFRC522, microSD and LED connections.
* [`Firmware Flow`](docs/firmware_flow.md) — Main firmware execution flow and access authentication process.

## Development

The project was developed and tested using an Arduino Nano with an ATmega328P microcontroller.

The source code is intentionally kept as individual `.cpp` programs rather than a single monolithic application, making it easier to test each part of the system independently.

The development workflow can be performed using **Arduino IDE** or **Visual Studio Code** with an Arduino-compatible extension/toolchain.

## Possible Improvements

Some possible future improvements include:

* Adding an actuator to physically open the gate.
* Adding a servo or DC motor for the gate mechanism.
* Implementing a configurable UID registration system directly on the device.
* Storing additional information associated with each UID.
* Adding timestamps and access logs to the microSD card.
* Adding an RTC module for date and time logging.
* Improving the SD database parser and error handling.
* Adding a display for system status and access information.
* Implementing a more robust non-blocking LED and actuator control system.

## Author

**Rafael Ramírez Salas**
