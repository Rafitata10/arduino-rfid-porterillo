graph TD

Arduino[Arduino Uno]

RC522[MFRC522 RFID]
SD[microSD Module]

LEDG[Blue LED]
LEDR[Red LED]

Arduino -->|Pin 10 (SS)| RC522
Arduino -->|Pin 9 (RST)| RC522
Arduino -->|Pin 11 (MOSI)| RC522
Arduino -->|Pin 12 (MISO)| RC522
Arduino -->|Pin 13 (SCK)| RC522

Arduino -->|Pin 3 MOSI| SD
Arduino -->|Pin 2 MISO| SD
Arduino -->|Pin 4 SCK| SD
Arduino -->|Pin 5 CS| SD

Arduino -->|Pin 7| LEDG
Arduino -->|Pin 6| LEDR
