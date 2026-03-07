#include <SPI.h>
#include <MFRC522.h>

// Pines del RC522
#define RST_PIN 9      // Pin RST del RC522
#define SS_PIN 10      // Pin SDA (SS)

// Pin del LED
#define LED_PIN 7      

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crear objeto RC522

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);
  while (!Serial); // Espera al puerto serial en algunos Arduinos

  // Inicializar SPI y RC522
  SPI.begin();
  mfrc522.PCD_Init();

  // Configurar pin del LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Acerque una tarjeta o llavero RFID...");
}

void loop() {
  // Esperar nueva tarjeta
  if (!mfrc522.PICC_IsNewCardPresent()) {
    digitalWrite(LED_PIN, LOW);  // LED apagado si no hay tarjeta
    return;
  }

  // Intentar leer la tarjeta
  if (!mfrc522.PICC_ReadCardSerial()) {
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // Encender LED porque hay tarjeta
  digitalWrite(LED_PIN, HIGH);

  // Mostrar UID en el Serial Monitor
  Serial.print("UID encontrado: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Liberar la tarjeta para permitir nuevas lecturas
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(500); // Pequeña pausa para evitar lecturas dobles seguidas
}
