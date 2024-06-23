#include <Wire.h> //Arduino'nun I2C haberleşme protokolünü destekleyen kütüphane.
#include <LiquidCrystal_I2C.h> //: I2C arabirimi üzerinden kontrol edilebilen LCD ekranları kullanmak için gerekli kütüphane.
#include <DHT.h> // Sıcaklık ve nem gibi çevresel verileri okumak için DHT sensörlerini destekleyen kütüphane.
#include "thingProperties.h" //Arduino IoT Cloud ile bağlantı kurmak ve cihaz özelliklerini tanımlamak için kullanılan dosya. 
//Bu dosya, projenin IoT yeteneklerini yönetmek için gereken fonksiyonları içerir.

#define DHTPIN 0          // DHT sensörünün bağlı olduğu pin
#define DHTTYPE DHT22     // DHT sensör tipi
#define LED_PIN 12        // LED'in bağlı olduğu pin numarası

const int I2C_ADDR = 0x27;  // LCD'nin I2C adresi
const int LCD_COLS = 16;    // LCD ekranının sütun sayısı
const int LCD_ROWS = 2;     // LCD ekranının satır sayısı

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(I2C_ADDR, LCD_COLS, LCD_ROWS);// LCD nesnesi
DHT dht(DHTPIN, DHTTYPE); // DHT sensörü nesnesi

unsigned long previousMillis = 0; // Önceki zaman damgası
const long interval = 2000;  // Güncelleme aralığı (2 saniye)

int kontrol = 0; // Kontrol değişkeni, hangi işlemin yapılacağını belirler

void setup() {
  Serial.begin(9600);  // Seri haberleşmeyi başlat
  pinMode(LED_PIN, OUTPUT);  // LED pinini çıkış olarak ayarla

  dht.begin();  // DHT sensörünü başlat
  lcd.init();   // LCD ekranını başlat
  lcd.backlight();  // LCD arka ışığını aç
  lcd.setCursor(0,0);  // İmleci (0,0) konumuna getir
  lcd.print("LCD BASLATILIYOR");  // Başlangıç mesajını yazdır
  delay(1000);  // 1 saniye bekle
  lcd.clear();  // LCD ekranını temizle

  initProperties();  // IoT bulut özelliklerini başlat
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // IoT bulutuna bağlan
  setDebugMessageLevel(2);  // Hata ayıklama seviyesini ayarla
  ArduinoCloud.printDebugInfo();  // Hata ayıklama bilgilerini yazdır
  Serial.println("Baglandi!");  // Seri monitöre bağlandı mesajı gönder
}

void loop() {
  unsigned long currentMillis = millis(); // Geçerli zaman damgasını al

  if (currentMillis - previousMillis >= interval) { /*Bu koşul, geçen zamanın belirlenen interval değerinden büyük
   veya eşit olup olmadığını kontrol eder. Eğer geçen zaman interval değerinden büyük veya eşitse, belirli bir işlemi 
   yapma zamanı gelmiş demektir.*/
    previousMillis = currentMillis; // Zaman damgasını güncelle

    switch (kontrol) { // Kontrol değişkenine göre işlevi seç
      case 0:
        dhtVerilerimiOkuNem();  // Nem verilerini oku işlevini çağır
        break;
      case 1:
        dhtVerilerimiOkuSicaklik(); // Sıcaklık verilerini oku işlevini çağır
        break;
      case 2:
        ArduinoCloud.update(); // IoT bulutunu güncelle işlevini çağır
        break;
    }

    kontrol++;
    if (kontrol == 3) { /*kontrol değişkeninin 3 olduğu durumda, yani üç işlev çağrıldıktan sonra, 
    kontrol değerini 0'a sıfırlar. Böylece döngü tekrar başa döner ve işlevler döngü içinde tekrar çağrılmaya başlar.*/
      kontrol = 0;
    }
  }
}

void onLedChange() { 
  if (led == true || sicaklik >= 25) {  // LED durumunu kontrol et
    digitalWrite(LED_PIN, HIGH);  // LED'i yak
  } else {
    digitalWrite(LED_PIN, LOW); // LED'i söndür
  }
}

void onMesajChange() { 
  if (mesaj != "") { // Mesaj var mı kontrol et
    mesaj.trim();  //mesaji temizle
    lcd.clear();  // LCD ekranını temizle
    lcd.setCursor(0, 0);   // İmleci (0,0) konumuna getir
    lcd.print(mesaj);  // Mesajı yazdır
  }
}

void dhtVerilerimiOkuNem() {

  lcd.clear();
  float humidity = dht.readHumidity();  // Nem verilerini oku
  nem = humidity; // Nem değerini güncelle

  
  Serial.print("Humidity: "); //konsola nem bilgilerini yazdır
  Serial.print(humidity);
  Serial.print("%");

  lcd.setCursor(4, 0); // İmleci (4,0) konumuna getir
  lcd.print("HUMIDITY"); // "HUMIDITY" yazısını yazdır (LCD ekrana)
  lcd.setCursor(3, 1);  // İmleci (3,1) konumuna getir
  lcd.print(humidity);   // Nem değerini yazdır
  lcd.setCursor(8, 1); // İmleci (8,1) konumuna getir
  lcd.print(humidity <= 50 ? "-LOW" : "-HIGH");//nem<50 ise low, nem>50 ise high
}

void dhtVerilerimiOkuSicaklik() {

  lcd.clear();  // LCD ekranını temizle 
  float humidity = dht.readHumidity();     // Nem verilerini oku
  float temperature = dht.readTemperature();  // Sıcaklık verilerini oku
  sicaklik = temperature; // Sıcaklık değerini güncelle

  Serial.print(" || "); //konsola sicaklik bilgilerini yazdir
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  lcd.setCursor(2, 0); // İmleci (2,0) konumuna getir
  lcd.print("TEMPERATURE"); // "TEMPERATURE" yazısını yazdır (LCD ekrana)
  lcd.setCursor(3, 1); // İmleci (3,1) konumuna getir
  lcd.print(temperature); // Sıcaklık değerini yazdır
  lcd.setCursor(8, 1); // İmleci (8,1) konumuna getir

  if (temperature < 0) { // Sıcaklık çok düşük mü kontrol et
    lcd.setCursor(0, 1); // İmleci (0,1) konumuna getir
    lcd.print(temperature); // Sıcaklık değerini yazdır
    lcd.setCursor(6, 1); // İmleci (6,1) konumuna getir
    lcd.print("-VERY LOW");
    digitalWrite(LED_PIN, LOW); // LED'i söndür

  } else if (temperature <= 25) { // Sıcaklık normal mi kontrol et
    lcd.print("-NORMAL");
    digitalWrite(LED_PIN, LOW); // LED'i söndür

  } else { //sicaklik>25 ise yani cok yuksekse
    lcd.print("-HIGH");
    digitalWrite(LED_PIN, HIGH); // LED'i yak
  }
}
