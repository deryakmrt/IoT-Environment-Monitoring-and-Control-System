    Downloaded from https://wokwi.com/projects/396359293304139777
Kullanılan kütüphaneler:  
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>  
#include <DHT.h>  
#include "thingProperties.h"  
Wokwi Proje Linki:



Proje Çalıştırma Adımları:  
1- Verilen wokwi linkine tıklayınız.  
2- Wokwi web sitesinde verilen linke ait projeyi görüntüleyiniz.  
3- Açılan proje simülasyonunda "Run" butonunu çalıştırınız.   
4- Projeyi çalıştırdığınızda LCD ekranı açılacak, konsolda "Bağlandi!" mesajını görüntüleyeceksiniz.  
5- Konsolda "ArduinoIoTCloudTCP::handle_WaitDeviceConfig device waiting for valid thing_id" mesajı görüntülenince ArdiunoCloud'a bağlı veriler (Mesaj-Humidity-Temperature) LCD ekranında ve konsolda görüntülenecektir.   
6- Sensör verilerini değiştirmek için simülasyonda yer alan DHT22 sensörünün üzerine tıklayıp değerleri değiştirebileceksiniz.  
7- Sensörün sıcaklık verisine göre LED durumu değişecektir. Yüksek sıcaklıklarda (sıcaklık>25) LED uyarı için yanacaktır.  

Not: Yazılan kodlar Round-Robin ilkesiyle yazılmıştır.  
Not: thingsProperties.h dosyasında yer alan bu bölüm, ArdinoCloud'da oluşturulan device bilgilerini içerir.  
Device oluşturduğumuzda bize "deviceID" ile "secretKey" bilgileri verilir. Bu bilgiler kopyalanıp bu dosya içinde kullanılır.  

const char DEVICE_LOGIN_NAME[]  = "29cea459-ca2a-46d2-9764-d117efa8445e"; //verilen deviceID buraya yazılır.  
const char SSID[]               = "Wokwi-GUEST";    // Network SSID (name)  
const char PASS[]               = "";    // Network password (use for WPA, or use as key for WEP)  
const char DEVICE_KEY[]  = "TWLzEXPbHW!hNHzjOr!iQFjUN";    // verilen secretKey buraya yazılır  
