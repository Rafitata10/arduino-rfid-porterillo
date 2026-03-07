#include <SPI.h>
#include <SdFat.h>
#include <MFRC522.h>

// ==== CONFIGURACIÓN SD (SoftSPI) ====
#define SD_MISO 2
#define SD_MOSI 3
#define SD_SCK  4
#define CS_SD   5

SdFatSoftSpi<SD_MISO, SD_MOSI, SD_SCK> SD;

// ==== CONFIGURACIÓN RFID (Hardware SPI) ====
#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

// ==== LEDs ====
#define LED_AZUL 7
#define LED_ROJO 6

// ==== VARIABLES PARA UIDS ====
String authorizedUIDs[50]; // máximo 50
int authorizedCount = 0;

// ==== DICTIONARY POR DEFECTO ====
String defaultUIDs[] = {
  "A6AC4404", "C1A0EB6F", "03E1ED6F", "BE9AED6F",
  "2CFD4304", "4487ED6F", "24D3A670", "C15DEC6F"
};
int defaultCount = 8;

void setup(){
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_ROJO, LOW);

  // ==== INICIALIZAR SD ====
  Serial.print(F("Inicializando SD con SoftSPI... "));
  bool sdOK = false;
  if(SD.begin(CS_SD, SPI_FULL_SPEED)){
    Serial.println(F("OK"));
    SdFile file;
    if(file.open("/UIDs.txt", FILE_READ)){
      Serial.println(F("Leyendo UIDs desde UIDs.txt..."));
      authorizedCount = 0;

      char c;
      String line = "";
      while(file.available() && authorizedCount < 50){
        c = file.read();
        if (c == '\n') { // fin de línea
          line.trim();
          if (line.length() > 0) {
            authorizedUIDs[authorizedCount++] = line;
          }
          line = "";
        } else if (c != '\r') { // ignorar retorno de carro
          line += c;
        }
      }
      line.trim();
      if(line.length() > 0 && authorizedCount < 50){
        authorizedUIDs[authorizedCount++] = line;
      }

      file.close();
      sdOK = true;
      Serial.print(F("Total UIDs cargados desde SD: "));
      Serial.println(authorizedCount);
    } else {
      Serial.println(F("No se pudo abrir UIDs.txt. Usando diccionario por defecto."));
    }
  } else {
    Serial.println(F("FALLO. Usando diccionario por defecto."));
  }

  // Si no se pudo leer SD, usar diccionario por defecto
  bool fromSD = false;
  if(!sdOK){
    for(int i = 0; i < defaultCount; i++){
      authorizedUIDs[i] = defaultUIDs[i];
    }
    authorizedCount = defaultCount;
  } else {
    fromSD = true;
  }

  // ==== MOSTRAR TODOS LOS UIDs CON SU FUENTE ====
  Serial.println(F("Lista completa de UIDs autorizados:"));
  for(int i = 0; i < authorizedCount; i++){
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.print(authorizedUIDs[i]);
    Serial.print(F(" ("));
    Serial.print(fromSD ? "SD" : "Diccionario por defecto");
    Serial.println(F(")"));
  }

  // ==== INICIALIZAR RFID ====
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println(F("=== MODO PORTERILLO ==="));
  Serial.println(F("Esperando chapitas..."));
}

void loop(){
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Obtener UID como string
  String uidStr = "";
  for(byte i = 0; i < mfrc522.uid.size; i++){
    if(mfrc522.uid.uidByte[i] < 0x10) uidStr += "0";
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();

  Serial.print(F("Leído UID: "));
  Serial.println(uidStr);

  // Comprobar si está autorizado
  bool authorized = false;
  for(int i = 0; i < authorizedCount; i++){
    if(authorizedUIDs[i] == uidStr){
      authorized = true;
      break;
    }
  }

  if(authorized){
    Serial.println(F("-> Acceso PERMITIDO. LED azul parpadeando 2s."));
    digitalWrite(LED_ROJO, LOW);
    unsigned long startTime = millis();
    while(millis() - startTime < 2000){
      digitalWrite(LED_AZUL, HIGH);
      delay(75);
      digitalWrite(LED_AZUL, LOW);
      delay(75);
    }
  } else {
    Serial.println(F("-> Acceso DENEGADO. LED rojo encendido 2s."));
    digitalWrite(LED_AZUL, LOW);
    unsigned long startTime = millis();
    while(millis() - startTime < 2000){
      digitalWrite(LED_ROJO, HIGH);
      delay(75);
      digitalWrite(LED_ROJO, LOW);
      delay(75);
    }
  }

  delay(200);
  mfrc522.PICC_HaltA();
}
