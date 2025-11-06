# 🌱 ESP32 Automated Greenhouse Control System

## 🌐 Language / Idioma
- [🇺🇸 English](#english-documentation)
- [🇪🇸 Español](#documentación-en-español)

---

## 🇺🇸 English Documentation

### 📋 Project Description
ESP32-based automated greenhouse control system with real-time environmental monitoring, automated actuator control, and interactive user interface for comprehensive plant care management.

### 🚀 Features
- 🌡️ Real-time temperature and humidity monitoring (DHT22 sensor)
- 🔧 Automatic ventilation control based on temperature thresholds
- 💧 Automatic irrigation control with visual blinking indicator
- 🎛️ Adjustable temperature reference via potentiometer (10-40°C)
- 📺 Dual OLED display screens switchable by button press
- 🖥️ Interactive serial menu with comprehensive system control
- 🎮 Manual override mode for all actuators
- ⚡ Real-time sensor updates every 2 seconds
- 🎯 Random humidity threshold generation for irrigation control

### ⚙️ System Modes

#### 🤖 Automatic Mode
- **Ventilation**: Activates when temperature exceeds reference value
- **Irrigation**: Activates when humidity falls below threshold (40-60%)
- **Visual Feedback**: Solid cyan LED for ventilation, blinking red LED for irrigation

#### 🎮 Manual Mode  
- Complete override of automatic controls
- Independent LED control via serial commands
- Return to automatic mode option available

### 🔧 Required Hardware
- ESP32 DevKit C V4 Development Board
- DHT22 Temperature and Humidity Sensor
- SSD1306 OLED Display (128x64, I2C)
- 10kΩ Analog Potentiometer
- Push Button with internal pull-up
- 2x LEDs (Cyan for ventilation, Red for irrigation)
- 2x 220Ω Resistors
- Breadboard and connection wires

### 📱 Serial Menu Options

| Option | Description |
|--------|-------------|
| `1` | Display complete greenhouse status |
| `2` | Modify reference values (temperature/humidity) |
| `3` | Manual actuator control |
| `4` | Exit menu |

#### 🎛️ Manual Control Submenu
| Command | Description |
|---------|-------------|
| `1` | Activate ventilation manually |
| `2` | Deactivate ventilation manually |
| `3` | Activate irrigation manually |
| `4` | Deactivate irrigation manually |
| `5` | Return to automatic mode |

### 🔌 Pin Configuration
```cpp
#define DHT_PIN      27    // DHT22 sensor data pin
#define POT_PIN      33    // Potentiometer analog input
#define LED_VENT     26    // Ventilation LED (cyan)
#define LED_RIEGO    25    // Irrigation LED (red)
#define BUTTON_PIN   14    // Display switch button
#define OLED_SDA     21    // I2C SDA (default)
#define OLED_SCL     22    // I2C SCL (default)
```

### 📺 OLED Display Screens
- **Screen 1**: Temperature monitoring and ventilation status
- **Screen 2**: Humidity monitoring and irrigation status
- Switch between screens using the physical button

### 📁 Project Structure
```
TP-TecnologiasDeAutomatizacion/
├── src/
│   └── main.cpp              # Main source code
├── include/
│   └── README                # Include directory info
├── lib/
│   └── README                # Libraries directory info  
├── test/
│   └── README                # Test directory info
├── platformio.ini            # PlatformIO configuration
├── wokwi.toml               # Wokwi simulator config
├── diagram.json             # Circuit diagram for simulation
├── .gitignore               # Git ignored files
└── README.md                # This file
```

### 🛠️ Installation and Usage

#### Prerequisites
- [PlatformIO](https://platformio.org/) installed
- VS Code with PlatformIO extension
- [Wokwi](https://wokwi.com/) account (optional, for simulation)

#### Steps
1. **Clone the repository**
   ```bash
   git clone https://github.com/EliasKarimRaueh/TP-IoT.git
   cd TP-IoT
   ```

2. **Build and upload to ESP32**
   ```bash
   pio run --target upload
   ```

3. **Monitor serial output**
   ```bash
   pio device monitor --baud 9600
   ```

4. **Simulate online (Optional)**
   - Open [Wokwi Simulator](https://wokwi.com/)
   - Import `diagram.json` and `wokwi.toml`
   - Upload compiled firmware

### 🛡️ Technologies Used
- **Platform:** ESP32 DevKit C V4
- **Framework:** Arduino Core for ESP32
- **Build System:** PlatformIO
- **Simulation:** Wokwi Online Simulator
- **Libraries:**
  - `Adafruit GFX Library` v1.11.9
  - `Adafruit SSD1306` v2.5.7  
  - `DHT sensor library for ESPx` v1.19

### 📊 Technical Specifications
- **Temperature Range:** 10°C to 40°C (configurable via potentiometer)
- **Humidity Range:** 40% to 60% (random threshold generation)
- **Sensor Update Rate:** 2000ms (2 seconds)
- **Button Debounce:** 50ms
- **LED Blink Rate:** 500ms (irrigation indicator)
- **Serial Baud Rate:** 9600
- **OLED Resolution:** 128x64 pixels
- **I2C Address:** 0x3C (OLED display)

### 📝 System Logic
1. **Startup**: Generate random humidity threshold (40-60%)
2. **Continuous Loop**:
   - Read DHT22 sensor every 2 seconds
   - Read potentiometer for temperature reference
   - Update actuators based on thresholds
   - Handle button presses for display switching
   - Process serial menu commands
   - Update OLED display with current information

---

## 🇪🇸 Documentación en Español

### 📋 Descripción del Proyecto
Sistema automatizado de control de invernadero basado en ESP32 con monitoreo ambiental en tiempo real, control automático de actuadores e interfaz de usuario interactiva para manejo integral del cuidado de plantas.

### 🚀 Características
- 🌡️ Monitoreo en tiempo real de temperatura y humedad (sensor DHT22)
- 🔧 Control automático de ventilación basado en umbrales de temperatura
- 💧 Control automático de riego con indicador visual intermitente
- 🎛️ Temperatura de referencia ajustable vía potenciómetro (10-40°C)
- 📺 Pantallas duales OLED intercambiables con botón físico
- 🖥️ Menú serie interactivo con control integral del sistema
- 🎮 Modo de anulación manual para todos los actuadores
- ⚡ Actualizaciones de sensores en tiempo real cada 2 segundos
- 🎯 Generación aleatoria de umbral de humedad para control de riego

### ⚙️ Modos de Sistema

#### 🤖 Modo Automático
- **Ventilación**: Se activa cuando la temperatura supera el valor de referencia
- **Riego**: Se activa cuando la humedad cae por debajo del umbral (40-60%)
- **Retroalimentación Visual**: LED cyan sólido para ventilación, LED rojo intermitente para riego

#### 🎮 Modo Manual
- Anulación completa de controles automáticos
- Control independiente de LEDs vía comandos serie
- Opción de retorno al modo automático disponible

### 🔧 Hardware Requerido
- Placa de Desarrollo ESP32 DevKit C V4
- Sensor de Temperatura y Humedad DHT22
- Display OLED SSD1306 (128x64, I2C)
- Potenciómetro Analógico 10kΩ
- Botón Pulsador con pull-up interno
- 2x LEDs (Cyan para ventilación, Rojo para riego)
- 2x Resistencias 220Ω
- Protoboard y cables de conexión

### 📱 Opciones del Menú Serie

| Opción | Descripción |
|--------|-------------|
| `1` | Mostrar estado completo del invernadero |
| `2` | Modificar valores de referencia (temperatura/humedad) |
| `3` | Control manual de actuadores |
| `4` | Salir del menú |

#### 🎛️ Submenú de Control Manual
| Comando | Descripción |
|---------|-------------|
| `1` | Activar ventilación manualmente |
| `2` | Desactivar ventilación manualmente |
| `3` | Activar riego manualmente |
| `4` | Desactivar riego manualmente |
| `5` | Volver al modo automático |

### 🔌 Configuración de Pines
```cpp
#define DHT_PIN      27    // Pin de datos del sensor DHT22
#define POT_PIN      33    // Entrada analógica del potenciómetro
#define LED_VENT     26    // LED de ventilación (cyan)
#define LED_RIEGO    25    // LED de riego (rojo)
#define BUTTON_PIN   14    // Botón de cambio de pantalla
#define OLED_SDA     21    // I2C SDA (por defecto)
#define OLED_SCL     22    // I2C SCL (por defecto)
```

### 📺 Pantallas del Display OLED
- **Pantalla 1**: Monitoreo de temperatura y estado de ventilación
- **Pantalla 2**: Monitoreo de humedad y estado de riego
- Cambio entre pantallas usando el botón físico

### 🛠️ Instalación y Uso

#### Prerequisitos
- [PlatformIO](https://platformio.org/) instalado
- VS Code con extensión PlatformIO
- Cuenta de [Wokwi](https://wokwi.com/) (opcional, para simulación)

#### Pasos
1. **Clonar el repositorio**
   ```bash
   git clone https://github.com/EliasKarimRaueh/TP-IoT.git
   cd TP-IoT
   ```

2. **Compilar y subir al ESP32**
   ```bash
   pio run --target upload
   ```

3. **Monitor serie**
   ```bash
   pio device monitor --baud 9600
   ```

4. **Simular online (Opcional)**
   - Abrir [Simulador Wokwi](https://wokwi.com/)
   - Importar `diagram.json` y `wokwi.toml`
   - Subir firmware compilado

### 🛡️ Tecnologías Utilizadas
- **Plataforma:** ESP32 DevKit C V4
- **Framework:** Arduino Core para ESP32
- **Sistema de Construcción:** PlatformIO
- **Simulación:** Simulador Online Wokwi
- **Librerías:**
  - `Adafruit GFX Library` v1.11.9
  - `Adafruit SSD1306` v2.5.7
  - `DHT sensor library for ESPx` v1.19

### 📊 Especificaciones Técnicas
- **Rango de Temperatura:** 10°C a 40°C (configurable vía potenciómetro)
- **Rango de Humedad:** 40% a 60% (generación de umbral aleatorio)
- **Frecuencia de Actualización:** 2000ms (2 segundos)
- **Debounce del Botón:** 50ms
- **Frecuencia de Parpadeo LED:** 500ms (indicador de riego)
- **Velocidad Serie:** 9600 baudios
- **Resolución OLED:** 128x64 píxeles
- **Dirección I2C:** 0x3C (display OLED)

### 📝 Lógica del Sistema
1. **Inicio**: Generar umbral aleatorio de humedad (40-60%)
2. **Bucle Continuo**:
   - Leer sensor DHT22 cada 2 segundos
   - Leer potenciómetro para referencia de temperatura
   - Actualizar actuadores según umbrales
   - Manejar pulsaciones de botón para cambio de pantalla
   - Procesar comandos del menú serie
   - Actualizar display OLED con información actual

---

**🎓 Universidad Tecnológica Nacional - Facultad Regional Córdoba**  
**📚 Materia: Tecnologías de Automatización**

[![PlatformIO](https://img.shields.io/badge/PlatformIO-orange.svg)](https://platformio.org/)
[![ESP32](https://img.shields.io/badge/ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino](https://img.shields.io/badge/Arduino-teal.svg)](https://www.arduino.cc/)
[![Wokwi](https://img.shields.io/badge/Wokwi-purple.svg)](https://wokwi.com/)