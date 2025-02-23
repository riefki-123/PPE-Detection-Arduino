#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 3  // Pin untuk buzzer

LiquidCrystal_I2C lcd(0x27, 16, 2); // Sesuaikan alamat I2C jika perlu

void setup() {
    lcd.init();  // Inisialisasi LCD
    lcd.backlight();  // Nyalakan backlight LCD
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(9600); // Komunikasi serial dengan Python
}

void loop() {
    if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n'); // Membaca data dari Python
        data.trim(); // Hapus spasi ekstra

        lcd.clear(); // Bersihkan LCD sebelum menampilkan teks baru

        if (data == "NO_PERSON") {
            lcd.setCursor(0, 0);
            lcd.print("No Attribut");
            lcd.setCursor(0, 1);
            lcd.print("Detected");
            noTone(BUZZER_PIN); // Tidak ada bunyi buzzer
        } 
        else if (data == "COMPLETE") {
            lcd.setCursor(0, 0);
            lcd.print("Atribut Lengkap");
            lcd.setCursor(0, 1);
            lcd.print("Silahkan Masuk");
            beep(2); // Bunyi beep 2 kali
        } 
        else if (data == "NO_VEST") {
            lcd.setCursor(0, 0);
            lcd.print("No Vest!");
            beep(1); // Bunyi beep 1 kali
        } 
        else if (data == "NO_HELMET") {
            lcd.setCursor(0, 0);
            lcd.print("No Helmet!");
            beep(1); // Bunyi beep 1 kali
        }
    }
}

// Fungsi untuk menghasilkan bunyi beep pada buzzer
void beep(int times) {
    for (int i = 0; i < times; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
    }
}
