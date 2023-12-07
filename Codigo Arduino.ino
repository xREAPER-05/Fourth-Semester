#include <Wire.h>   
#include <SPI.h>
#include <MFRC522.h>
int dato;
int CORRECTO=21;
int PIN1=15;
int Cerra=13;

#define RST_PIN 22     // Pin de reset del RC522
#define SS_PIN 5      // Pin SDA del RC522

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crear instancia MFRC522

String DatoHex;
const String UserReg_1 = "53D370AC";

void setup() {
  pinMode(CORRECTO,OUTPUT);
  pinMode(PIN1,OUTPUT);
  pinMode(Cerra,OUTPUT);
  digitalWrite(Cerra, LOW);
  Serial.begin(115200);   // Iniciar comunicación serial
  SPI.begin();            // Iniciar SPI bus
  mfrc522.PCD_Init();     // Iniciar MFRC522
  Serial.println("Acerca la tarjeta NFC al lector...");
}

void loop() {
  dato = digitalRead(Cerra);
  // Buscar tarjetas
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Seleccionar una de las tarjetas
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  DatoHex = ""; // Limpia el valor de DatoHex antes de rellenarlo
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      DatoHex += '0';  // Añade un cero si el valor es menor que 16 (0x10)
    }
    DatoHex += String(mfrc522.uid.uidByte[i], HEX);
  }

  DatoHex.toUpperCase(); // Convertir la cadena a mayúsculas para asegurar la comparación

  if (UserReg_1 == DatoHex) {
    Serial.println("BIENVENIDO - INGRESO PERMITIDO");
    digitalWrite(CORRECTO,HIGH);
    digitalWrite(Cerra,HIGH);
    Serial.println();
    delay(3000);
    digitalWrite(CORRECTO,LOW);
    digitalWrite(Cerra,LOW);

  } else {
    Serial.println("INGRESO NO AUTORIZADO");
    Serial.println();
    digitalWrite(PIN1,HIGH);
    delay(3000);
    digitalWrite(PIN1,LOW);
  }
  
  delay(1000);  // Dar un poco de tiempo antes de leer nuevamente
}
