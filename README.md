# ESP32 LED Control with MQTT and DHT Sensor

This project demonstrates controlling multiple LEDs using an ESP32 microcontroller via MQTT messaging. It includes integration with a DHT22 sensor to monitor temperature and humidity, publishing data to an MQTT broker.

## Features

- **MQTT Integration:** Control LEDs (on/off, brightness) and retrieve sensor data (temperature, humidity) via MQTT commands.
- **DHT22 Sensor:** Monitors ambient temperature and humidity, publishing updates to MQTT.
- **LED Control:** Each LED is independently controlled and identified by a unique client ID.
- **Built-in Web Server:** Access LED control and sensor data via a simple web interface.

## Components

- **ESP32 Development Board**
- **DHT22 Temperature and Humidity Sensor**
- **4 LEDs (Red, Green, Blue, Yellow)**

## Getting Started

1. **Hardware Setup:**
   - Connect LEDs to GPIO pins as specified in the code (`ledPins` array).
   - Connect DHT22 sensor to the specified GPIO pin (`DHTPIN`).

2. **Software Setup:**
   - Install necessary libraries: `WiFi.h`, `PubSubClient.h`, `DHT.h`.
   - Update `ssid`, `password`, `mqtt_server`, and other configuration variables in `sketch.ino`.
   - Upload the sketch to your ESP32 board using the Arduino IDE or PlatformIO.

3. **Usage:**
   - Once connected to WiFi and MQTT, LEDs can be controlled using MQTT commands formatted as `clientID:operation:value`.
   - Monitor temperature and humidity updates via subscribed MQTT topics.

## MQTT Commands

- **LED Control:** `clientID:on:1`, `clientID:off:1`, `clientID:lightcontrol:50` (for brightness).
- **Sensor Data:** `clientID:t` (temperature), `clientID:h` (humidity), `clientID:l.s` (LED state).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
## Additional Links
- [Wokwi Simulation Project](https://wokwi.com/projects/400760802720346113)
- [GitHub Profile](https://github.com/techrj786)
