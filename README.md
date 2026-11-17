# ESP32 Gold Price Tracker

> Altın, gümüş, BTC ve döviz gibi fiyatları OLED ekranda gösteren, ortam sıcaklığı ve hava durumunu da izleyen ESP32 masa cihazı.
>
> *EN: An ESP32 desk gadget that shows live gold, silver, BTC and currency prices on a 128x64 OLED, together with local weather and indoor temperature/pressure. Tracked items are managed from a web page served by the board.*

Cihaz Wi-Fi'ye bağlanır, takip edilen fiyatları internetten çeker ve OLED ekranda sırayla gösterir. Takip edilecek kalemler tarayıcıdan eklenip silinir.

## Özellikler

- **Fiyat takibi:** Altın, gümüş, BTC gibi kalemler ekranda sırayla görünür. Kalemler web arayüzünden eklenir ve silinir.
- **Ortam ölçümü:** AHT20 ile sıcaklık ve nem, BMP280 ile sıcaklık ve basınç.
- **Hava durumu:** OpenWeatherMap üzerinden, ayarlanan enlem ve boylam için.
- **Saat:** NTP ile (`pool.ntp.org`, `time.nist.gov`), saat dilimi ayarlanabilir.
- **Web arayüzü:** Kart kendi sayfasını sunar ("Anlık Döviz Takibi"). Sayfa dosyaları LittleFS'tedir, sunucu `ESPAsyncWebServer` ile çalışır.
- **Dayanıklılık:** Watchdog zamanlayıcı kullanılır.

## Donanım

- ESP32
- 128×64 SSD1306 OLED (I²C)
- AHT20 sıcaklık/nem sensörü (I²C)
- BMP280 basınç sensörü (I²C)

## Yazılım

Arduino IDE ile derlenir. Kütüphaneler: `Adafruit_SSD1306`, `Adafruit_AHTX0`, `Adafruit_BMP280`, `ESPAsyncWebServer`, `ArduinoJson`, `LittleFS`, `HTTPClient`.

| Klasör | Açıklama |
| --- | --- |
| `ESP32_Gold_Price_Tracker` | İlk sürüm |
| `ESP32_Gold_Price_Tracker_2` | Sensörler, hava durumu ve web arayüzü eklenmiş sürüm. `data/` içinde arayüz, `pictures/` içinde görseller bulunur. |

## Kurulum

1. Gerekli kütüphaneleri Arduino IDE'ye ekleyin.
2. `.ino` dosyasında Wi-Fi bilgilerinizi ve **kendi OpenWeatherMap API anahtarınızı** girin (`WEATHER_API_KEY`). Anahtarınızı depoya yüklemeyin.
3. Kartı yükleyin, ardından `data/` klasörünü LittleFS'e yükleyin.
4. Kartın IP adresini seri monitörden öğrenip tarayıcıdan açın ve takip edilecek kalemleri ekleyin.

## Lisans

Ayrıntılar için `LICENSE` dosyasına bakın.

— Mert Güzelmansur (MGM)
