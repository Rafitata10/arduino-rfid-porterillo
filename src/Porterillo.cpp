#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define SS_SD 8
#define LED_AZUL 7   // LED para acceso permitido
#define LED_ROJO 6    // LED para acceso denegado

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Diccionario cargado manualmente (simula lo que guardaste antes)
String authorizedUIDs[] = {
  "A6AC4404", // 1
  "C1A0EB6F", // 2
  "03E1ED6F", // 3
  "BE9AED6F", // 4
  "2CFD4304", // 5
  "4487ED6F", // 6
  "24D3A670", // 7
  "C15DEC6F", // 8
  "32824B3E", // 9
  "5D9FEC6F", // 10
  "B78E864F", // 11 - White Card
  "28BCD93D", // 12 - White Card II
  "551563E6"  // 13 - Renfe
};
int authorizedCount = 13;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(SS_SD, OUTPUT);

  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(SS_SD, HIGH);

  Serial.println("=== MODO PORTERILLO ===");
  Serial.println("Esperando chapitas...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Obtener UID como string
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidStr += "0";
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();

  Serial.print("Leído UID: ");
  Serial.println(uidStr);

  // Comprobar si está autorizado
  bool authorized = false;
  for (int i = 0; i < authorizedCount; i++) {
    if (authorizedUIDs[i] == uidStr) {
      authorized = true;
      break;
    }
  }

  if (authorized) {
    Serial.println("-> Acceso PERMITIDO. Se abre portón (LED AZUL encendido).");
    digitalWrite(LED_AZUL, HIGH);
    digitalWrite(LED_ROJO, LOW);
    delay(500); // simula apertura
    digitalWrite(LED_AZUL, LOW);
  } else {
    Serial.println("-> Acceso DENEGADO. UID desconocido (LED ROJO encendido).");
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_AZUL, LOW);
    delay(500);
    digitalWrite(LED_ROJO, LOW);
  }

  // pequeña pausa para no leer la misma tarjeta en bucle
  delay(200);

  // Liberar tarjeta
  mfrc522.PICC_HaltA();
  // mfrc522.PCD_StopCrypto1();
}
