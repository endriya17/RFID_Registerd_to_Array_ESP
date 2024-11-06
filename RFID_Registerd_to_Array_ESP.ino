#include <MFRC522.h>

//Setup Pin Sesuaikan dengan Board yang di pakai 
#define SS_PIN 15
#define RST_PIN 0

MFRC522 rfid(SS_PIN, RST_PIN);

// Masukan UID yang  di Register dalam Array
byte registeredUIDs[][7] = {
    {0x73, 0x91, 0xB3, 0x1A, 0x00, 0x00, 0x00},  // UID 1: 73 91 B3 1A
    {0xB1, 0x35, 0x32, 0x1D, 0x00, 0x00, 0x00},  // UID 2: B1 35 32 1D
    {0xEF, 0x2D, 0x33, 0x28, 0x00, 0x00, 0x00},  // UID 3: EF 2D 33 28 (Emoney Mandiri)
    {0x02, 0x6D, 0x97, 0xC4, 0x04, 0x60, 0x00},  // UID 4: 2 6D 97 C4 4 60 0 (SIM)
};

int registeredUIDCount = 4; // Jumlah Uid yang ada pada Array Register

void setup() {
    SPI.begin();            // Inisialilasi SPI bus
    rfid.PCD_Init();        // Inisalisasi modul MFRC522
    Serial.begin(9600);
}

void loop() {
  // Check jika deteksi kartu 
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    Serial.print("Card UID: ");
        for (byte i = 0; i < rfid.uid.size; i++) {
            Serial.print(rfid.uid.uidByte[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    // Check jika kartu terdapat dalam register array
    if (isUIDRegistered(rfid.uid.uidByte, rfid.uid.size)) {
        Serial.println("UID is registered.");
    } else {
        Serial.println("UID is NOT registered.");
    }
    // Berhenti komunikasi pada modul RFID
    rfid.PICC_HaltA();    
  }

}

// Fungsi untuk membandingan UIDs dan check pada array Registed
bool isUIDRegistered(byte *uid, byte uidSize) {
    // Iterate through all registered UIDs
    for (int i = 0; i < registeredUIDCount; i++) {
        bool match = true; // Assume a match unless proven otherwise

        // Check if all bytes match for this registered UID
        for (byte j = 0; j < uidSize; j++) {
            if (registeredUIDs[i][j] != uid[j]) {
                match = false; // Not a match, break out of inner loop
                break;
            }
        }

        // If we found a match, return true
        if (match) return true;
    }
    
    // No match found
    return false;
}
