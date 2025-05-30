#include <DHT.h>

// Khai báo chân kết nối
#define LDR_PIN D0        // Cảm biến ánh sáng (DO - digital)
#define SOIL_PIN A0       // Cảm biến độ ẩm đất (AO - analog)
#define DHT_PIN D3        // Cảm biến DHT11
#define DHT_TYPE DHT11
#define RELAY_PUMP D1     // Relay điều khiển bơm
#define RELAY_LIGHT D7    // Relay điều khiển đèn

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);

  pinMode(LDR_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PUMP, OUTPUT);
  pinMode(RELAY_LIGHT, OUTPUT);

  digitalWrite(RELAY_PUMP, HIGH);   // Tắt bơm ban đầu
  digitalWrite(RELAY_LIGHT, HIGH);  // Tắt đèn ban đầu

  dht.begin();
}

// Đọc cảm biến ánh sáng (digital) và điều khiển đèn
void readLDR() {
  int lightStatus = digitalRead(LDR_PIN);  // Đọc từ DO: 0 = tối, 1 = sáng

  if (lightStatus == 0) {
    Serial.println("Sáng - Tắt đèn");
    digitalWrite(RELAY_LIGHT, HIGH);   // Tắt đèn
  } else {
    Serial.println("Tối - Bật đèn");
    digitalWrite(RELAY_LIGHT, LOW);  // Bật đèn
  }
}

// Đọc cảm biến DHT11
void readDHT11() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Không đọc được từ DHT11");
    return;
  }

  Serial.print("Nhiệt độ: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Độ ẩm không khí: ");
  Serial.print(humidity);
  Serial.println(" %");
}

// Đọc cảm biến độ ẩm đất và điều khiển bơm
void soilMoistureSensor() {
  int rawValue = analogRead(SOIL_PIN);  // Đọc từ AO
  int moisturePercent = map(rawValue, 0, 1023, 100, 0); // 100% = ướt, 0% = khô

  Serial.print("Độ ẩm đất: ");
  Serial.print(moisturePercent);
  Serial.println(" %");

  if (moisturePercent <= 20) {
    Serial.println("Đất khô - Bật bơm");
    digitalWrite(RELAY_PUMP, LOW);   // Bật bơm
  } else {
    Serial.println("Đất ẩm - Tắt bơm");
    digitalWrite(RELAY_PUMP, HIGH);  // Tắt bơm
  }
}

void loop() {
  soilMoistureSensor();
  readLDR();
  readDHT11();
  Serial.println("---------------------");
  delay(2000);
}
