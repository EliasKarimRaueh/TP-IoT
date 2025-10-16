#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"

// === Definición de pines ===
#define DHT_PIN      27    // Pin de datos del DHT22
#define POT_PIN      33    // Pin del potenciómetro
#define LED_VENT     26    // Pin del LED de ventilación (cyan)
#define LED_RIEGO    25    // Pin del LED de riego (rojo)
#define BUTTON_PIN   14    // Pin del botón

// === Configuración OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === Configuración DHT ===
// Objeto para manejar el sensor DHT22 (temperatura y humedad)
DHTesp dht;

// === Constantes de referencia ===
#define TEMP_MIN 10
#define TEMP_MAX 40
#define HUM_MIN 40
#define HUM_MAX 60

// === Variables globales ===
float currentTemp = 0;                // Última temperatura leída
float currentHum = 0;                 // Última humedad leída
float temperaturaReferencia = 25.0;   // Temperatura de referencia (ajustable)
float humedadReferencia = 60.0;       // Variable de prueba ya que utilizamos humThreshold
bool ventilacionActiva = false;       // TRUE si el control manual de ventilación está activo
bool riegoActivo = false;             // TRUE si el control manual de riego está activo
int humThreshold;                     // Umbral de humedad (aleatorio o ajustable)
int displayIndex = 0;                 // Pantalla actual del OLED
bool lastButtonState = HIGH;          // Estado anterior del botón
bool currentButtonState = HIGH;       // Estado actual del botón
unsigned long lastDebounceTime = 0;   // Último tiempo de debounce del botón
unsigned long lastSensorRead = 0;     // Última lectura de sensores
unsigned long lastBlinkTime = 0;      // Último cambio de parpadeo del LED de riego
bool ledBlinkState = false;           // Estado ON/OFF del parpadeo del LED de riego
const unsigned long DEBOUNCE_DELAY = 50;         // Tiempo de debounce del botón (ms)
const unsigned long SENSOR_READ_INTERVAL = 2000; // Intervalo de lectura de sensores (ms)
const unsigned long BLINK_INTERVAL = 500;        // Intervalo de parpadeo del LED de riego (ms)

// === Declaración de funciones ===
void readSensors();
void updateOLED(int idx);
void handleButton();
void mostrarMenu();
void manejarMenu();
void mostrarEstadoInvernadero();
void modificarValoresReferencia();
void controlManualActuadores();

void setup() {
    Serial.begin(9600);

    // Configuración de pines
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_VENT, OUTPUT);
    pinMode(LED_RIEGO, OUTPUT);
    pinMode(POT_PIN, INPUT);

    // Inicializar DHT22
    dht.setup(DHT_PIN, DHTesp::DHT22);

    // Inicializar display OLED
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Error en el display SSD1306"));
        for(;;);
    }
    
    // Generar umbral aleatorio de humedad
    randomSeed(analogRead(0));
    humThreshold = random(HUM_MIN, HUM_MAX + 1);
    
    // Mostrar configuración inicial en OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Iniciando sistema...");
    display.printf("Umbral humedad: %d%%", humThreshold);
    display.display();
    delay(2000);

    Serial.printf("Sistema iniciado!\nUmbral humedad: %d%%\n", humThreshold);
    mostrarMenu();
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Leer sensores cada cierto intervalo
    if (currentMillis - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = currentMillis;
        readSensors();
    }

    // Control de parpadeo del LED de riego
    if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
        lastBlinkTime = currentMillis;
        ledBlinkState = !ledBlinkState;
    }

    // Manejar entrada del botón para cambiar pantalla
    handleButton();

    // --- Control de ventilación (LED FIJO) ---
    // Si está en modo manual, respeta el estado manual
    if (!ventilacionActiva) {
        if (currentTemp > temperaturaReferencia) {
            digitalWrite(LED_VENT, HIGH);
        } else {
            digitalWrite(LED_VENT, LOW);
        }
    }

    // --- Control de riego (LED INTERMITENTE) ---
    // Si está en modo manual, respeta el estado manual
    if (!riegoActivo) {
        if (currentHum < humThreshold) {
            digitalWrite(LED_RIEGO, ledBlinkState);
        } else {
            digitalWrite(LED_RIEGO, LOW);
        }
    }

    // Actualizar display OLED
    updateOLED(displayIndex);
    
    // Manejar menú por puerto serie
    manejarMenu();
    
    delay(10); // Pequeño delay para estabilidad
}

// === Lectura de sensores ===
void readSensors() {
    // Leer DHT22
    TempAndHumidity data = dht.getTempAndHumidity();
    if (!isnan(data.temperature) && !isnan(data.humidity)) {
        currentTemp = data.temperature;
        currentHum = data.humidity;
    }
    
    // Leer potenciómetro para temperatura de referencia
    int potValue = analogRead(POT_PIN);
    temperaturaReferencia = map(potValue, 0, 4095, TEMP_MIN, TEMP_MAX);

    // Debug desactivado para evitar ruido en el menú
    // Serial.printf("Temp: %.1f°C, Hum: %.1f%%, Ref: %.1f°C\n", currentTemp, currentHum, temperaturaReferencia);
}

// === Manejo del botón para cambiar pantalla ===
void handleButton() {
    int reading = digitalRead(BUTTON_PIN);
    unsigned long currentMillis = millis();

    if (reading != lastButtonState) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != currentButtonState) {
            currentButtonState = reading;
            if (currentButtonState == LOW) {
                displayIndex = (displayIndex + 1) % 2; // Solo dos pantallas
                // Serial.printf("Cambio de pantalla a: %d\n", displayIndex); // Debug desactivado
            }
        }
    }
    lastButtonState = reading;
}

// === Actualización de la pantalla OLED ===
void updateOLED(int idx) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    switch(idx) {
        case 0: // Pantalla 1: Temperatura y ventilación
            display.println("Temperatura y Vent.");
            display.printf("Temp actual: %.1f C\n", currentTemp);
            display.printf("Temp ref: %.1f C\n", temperaturaReferencia);
            if (ventilacionActiva) {
                display.println(digitalRead(LED_VENT) ? "Vent.: ON (Manual)" : "Vent.: OFF (Manual)");
            } else {
                display.println(currentTemp > temperaturaReferencia ? 
                              "Vent.: ON" : "Vent.: OFF");
            }
            break;
            
        case 1: // Pantalla 2: Humedad y riego
            display.println("Humedad y Riego");
            display.printf("Hum actual: %.1f %%\n", currentHum);
            display.printf("Hum umbral: %d %%\n", humThreshold);
            if (riegoActivo) {
                display.println(digitalRead(LED_RIEGO) ? "Riego: ON (Manual)" : "Riego: OFF (Manual)");
            } else {
                display.println(currentHum < humThreshold ? 
                              "Riego: ON (parp)" : "Riego: OFF");
            }
            break;
    }
    display.display();
}

// === Menú por puerto serie ===
void mostrarMenu() {
    Serial.println("\n=== MENU DEL INVERNADERO ===");
    Serial.println("1. Mostrar estado del invernadero");
    Serial.println("2. Modificar valores de referencia");
    Serial.println("3. Control manual de actuadores");
    Serial.println("4. Salir");
    Serial.println("Seleccione una opción (1-4):");
}

void manejarMenu() {
    if (Serial.available() > 0) {
        char opcion = Serial.read();
        // Limpiar buffer
        while (Serial.available()) Serial.read();
        switch (opcion) {
            case '1':
                mostrarEstadoInvernadero();
                break;
            case '2':
                modificarValoresReferencia();
                break;
            case '3':
                controlManualActuadores();
                break;
            case '4':
                Serial.println("Saliendo del menú...");
                break;
            default:
                Serial.println("Opción no válida");
                break;
        }
        mostrarMenu();
    }
}

// === Mostrar estado completo del invernadero ===
void mostrarEstadoInvernadero() {
    Serial.println("\n=== ESTADO DEL INVERNADERO ===");
    Serial.print("Temperatura actual: ");
    Serial.print(currentTemp);
    Serial.println(" °C");
    Serial.print("Humedad actual: ");
    Serial.print(currentHum);
    Serial.println(" %");
    Serial.print("Ventilación: ");
    if (ventilacionActiva)
        Serial.println(digitalRead(LED_VENT) ? "ACTIVADA (Manual)" : "DESACTIVADA (Manual)");
    else
        Serial.println((currentTemp > temperaturaReferencia) ? "ACTIVADA" : "DESACTIVADA");
    Serial.print("Sistema de riego: ");
    if (riegoActivo)
        Serial.println(digitalRead(LED_RIEGO) ? "ACTIVADO (Manual)" : "DESACTIVADO (Manual)");
    else
        Serial.println((currentHum < humThreshold) ? "ACTIVADO" : "DESACTIVADO");
}

// === Modificar valores de referencia desde el menú ===
void modificarValoresReferencia() {
    Serial.println("\n=== MODIFICAR VALORES DE REFERENCIA ===");
    Serial.println("¿Qué desea modificar?");
    Serial.println("1. Temperatura de referencia");
    Serial.println("2. Humedad de referencia");
    
    String inputString = "";
    float nuevoValor = 0;
    int nuevoValorHum = 0;
    
    while (!Serial.available()) delay(100);
    char opcion = Serial.read();
    while(Serial.available()) Serial.read(); // Limpiar buffer
    
    switch (opcion) {
        case '1':
            Serial.println("Ingrese nueva temperatura de referencia y presione Enter:");
            inputString = "";
            while (true) {
                if (Serial.available()) {
                    char c = Serial.read();
                    if (c == '\n' || c == '\r') break;
                    inputString += c;
                }
            }
            nuevoValor = inputString.toFloat();
            if (nuevoValor >= TEMP_MIN && nuevoValor <= TEMP_MAX) {
                temperaturaReferencia = nuevoValor;
                Serial.printf("Nueva temperatura de referencia: %.1f°C\n", temperaturaReferencia);
            } else {
                Serial.printf("Valor fuera de rango (%d-%d°C)\n", TEMP_MIN, TEMP_MAX);
            }
            break;
        case '2':
            Serial.println("Ingrese nueva humedad de referencia y presione Enter:");
            inputString = "";
            while (true) {
                if (Serial.available()) {
                    char c = Serial.read();
                    if (c == '\n' || c == '\r') break;
                    inputString += c;
                }
            }
            nuevoValorHum = inputString.toInt();
            if (nuevoValorHum >= HUM_MIN && nuevoValorHum <= HUM_MAX) {
                humThreshold = nuevoValorHum;
                Serial.printf("Nuevo umbral de humedad: %d%%\n", humThreshold);
            } else {
                Serial.printf("Valor fuera de rango (%d-%d%%)\n", HUM_MIN, HUM_MAX);
            }
            break;
        default:
            Serial.println("Opción no válida");
            break;
    }
}

// === Control manual de actuadores desde el menú ===
void controlManualActuadores() {
    Serial.println("\n=== CONTROL MANUAL DE ACTUADORES ===");
    Serial.println("1. Activar ventilación");
    Serial.println("2. Desactivar ventilación");
    Serial.println("3. Activar riego");
    Serial.println("4. Desactivar riego");
    Serial.println("5. Volver a modo automático");
    
    while (!Serial.available()) delay(100);
    char opcion = Serial.read();
    while(Serial.available()) Serial.read(); // Limpiar buffer
    
    switch (opcion) {
        case '1':
            ventilacionActiva = true;
            digitalWrite(LED_VENT, HIGH);
            Serial.println("Ventilación ACTIVADA manualmente");
            break;
        case '2':
            ventilacionActiva = true; // Mantener en manual pero apagado
            digitalWrite(LED_VENT, LOW);
            Serial.println("Ventilación DESACTIVADA manualmente");
            break;
        case '3':
            riegoActivo = true;
            digitalWrite(LED_RIEGO, HIGH);
            Serial.println("Riego ACTIVADO manualmente");
            break;
        case '4':
            riegoActivo = true; // Mantener en manual pero apagado
            digitalWrite(LED_RIEGO, LOW);
            Serial.println("Riego DESACTIVADO manualmente");
            break;
        case '5':
            ventilacionActiva = false;
            riegoActivo = false;
            digitalWrite(LED_VENT, LOW);
            digitalWrite(LED_RIEGO, LOW);
            Serial.println("Volviendo a modo automático");
            break;
        default:
            Serial.println("Opción no válida");
            break;
    }
}