#include <DHT.h>

// Cấu hình chân
#define DHT_PIN 17
#define DHT_TYPE DHT11
#define SOIL_AO 34
#define SOIL_DO 32
#define LDR_DO 4
#define LDR_AO 35
#define RELAY1 16
#define RELAY2 5

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);

  // Khởi tạo cảm biến và relay
  pinMode(SOIL_DO, INPUT);
  pinMode(LDR_DO, INPUT);
  pinMode(SOIL_AO, INPUT);
  pinMode(LDR_AO, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, HIGH); // Tắt bơm
  digitalWrite(RELAY2, HIGH); // Tắt đèn

  dht.begin();
}

void readSoilSensor() {
  int soilAnalog = analogRead(SOIL_AO);
  soilAnalog = map(soilAnalog, 0, 4095, 0, 100);
  soilAnalog = (soilAnalog - 100) * -1;

  Serial.print("Độ ẩm đất (AO): ");
  Serial.print(soilAnalog);
  Serial.println(" %");

  // ✅ Hiển thị trạng thái theo AO
  Serial.print("Trạng thái đất: ");
  if (soilAnalog == 0) {
    Serial.println("Khô");
  } else {
    Serial.println("Ẩm");
  }

  // Điều khiển bơm
  if (soilAnalog >= 20) {
    digitalWrite(RELAY1, LOW); // Bật bơm
  } else {
    digitalWrite(RELAY1, HIGH); // Tắt bơm
  }
}

void readLDR() {
  int ldrAnalog = analogRead(LDR_AO); // Đọc cường độ ánh sáng

  Serial.print("Cường độ sáng (AO): ");
  Serial.println(ldrAnalog);

  // ✅ Điều khiển đèn theo yêu cầu mới: >600 là Tối, bật đèn
  if (ldrAnalog > 600) {
    digitalWrite(RELAY2, LOW); // Bật đèn
    Serial.println("Trạng thái ánh sáng: Tối");
  } else {
    digitalWrite(RELAY2, HIGH); // Tắt đèn
    Serial.println("Trạng thái ánh sáng: Sáng");
  }
}

void readDHT11() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if (isnan(temp) || isnan(humid)) {
    Serial.println("Không đọc được dữ liệu từ DHT11");
    return;
  }

  Serial.print("Nhiệt độ: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Độ ẩm không khí: ");
  Serial.print(humid);
  Serial.println(" %");
}

void loop() {
  readSoilSensor();
  readLDR();
  readDHT11();
  Serial.println("----");
  delay(2000);
}
