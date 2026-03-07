# Arduino RFID Porterillo

RFID access control system for Arduino using the MFRC522 reader and a microSD UID database.

The project demonstrates the simultaneous use of **hardware SPI** and **SoftSPI** using the SdFat library.

## Features

- RFID authentication using MFRC522
- UID database stored on microSD
- Fallback UID list when SD card is unavailable
- Hardware SPI used for RFID communication
- SoftSPI used for SD card communication
- Serial debug output

## Project Structure

ID_reader  
Reads and prints RFID UID values.

UIDs_keeper  
Registers new RFID tags.

Porterillo  
Main access control firmware.

## Hardware

- Arduino Uno / Nano
- MFRC522 RFID reader
- MicroSD module
- LEDs for access feedback

## SPI Architecture

RFID reader uses **hardware SPI**.

MicroSD card uses **SoftSPI** through the SdFat library to avoid SPI bus conflicts.

## Example SD file

UIDs.txt
A6AC4404
C1A0EB6F
03E1ED6F
BE9AED6F

## Architecture

See the architecture diagrams in `/docs`:

- SPI architecture
- Hardware wiring
- Firmware flow

## Author

Rafael Ramírez Salas
