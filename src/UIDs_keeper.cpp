#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define LED_PIN 7

MFRC522 mfrc522(SS_PIN, RST_PIN);

// “Diccionario” de UIDs (simulado con array)
String uidList[50];
int uidCount = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("=== MODO REGISTRO ===");
  Serial.println("Acerque chapitas para registrarlas...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Obtener UID como string en HEX
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidStr += "0";
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();

  Serial.print("Leído UID: ");
  Serial.println(uidStr);

  // Verificar si ya existe en la lista
  bool exists = false;
  for (int i = 0; i < uidCount; i++) {
    if (uidList[i] == uidStr) {
      exists = true;
      break;
    }
  }

  if (exists) {
    Serial.println("-> Ya estaba registrado, se ignora.");
    digitalWrite(LED_PIN, LOW);
  } else {
    if (uidCount < 20) {
      uidList[uidCount] = uidStr;
      uidCount++;
      Serial.println("-> NUEVO UID registrado.");
      digitalWrite(LED_PIN, HIGH);
      delay(500); // parpadeo LED
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.println("-> Diccionario lleno, no se puede registrar más.");
    }
  }

  // Liberar tarjeta
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
