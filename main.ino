// main.ino
// ============================================
// RESPONSABILIDAD: Orquestar arranque y bucle principal del sistema.
// No sabe como hacer nada: solo llama a cada modulo en el orden correcto.
// ============================================

#include "config.h"
#include "i2c_manager.h"
#include "display.h"
#include "logo.h"
#include "logboot.h"
#include "eyes.h"
#include "debug_serial.h"

// Estados de la maquina de arranque y marca de tiempo de su ventana
bool bootComplete = false;
unsigned long bootTime = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[BOOT] sistema de ojos OLED"));

    initI2C();
    Serial.printf("[I2C] bus listo SDA=%d SCL=%d\n", I2C_SDA_PIN, I2C_SCL_PIN);

    // Paso 3 — Barrido del bus reportado (verás el dispositivo en 0x3C y el conteo final).
    scanI2C();
    // Paso 4 — Dirección del panel sondeada (verás la respuesta del POST del OLED).
    testI2CDevice();
    
    // Paso 5 — Panel inicializado (verás el panel listo de 128x64 a 400 kHz).
    initDisplay();
    Serial.printf("[OLED] panel listo %dx%d a %d kHz\n", OLED_WIDTH, OLED_HEIGHT, I2C_FREQUENCY_HZ / 1000);

    // Paso 6 — Logo pintado y POST de pantalla (verás el cuadrado de pantalla y sus coordenadas).
    showLogo();
    testDisplay();

    // Paso 7 — Ojos inicializados (verás los ojos listos a 60 fps).
    initEyes(); 


    // Paso 8 — Ayuda publicada y ventana de arranque armada (verás la ayuda de depuración;
    printHelp();
    bootTime = millis(); 
}

void loop() {
    // TODO 2.4 (continuación): Mientras la ventana de arranque no expire, mantén el logo
    // en pantalla; al expirar, cambia de estado y repórtalo por el monitor.
    if (!bootComplete) {
        if (millis() - bootTime < LOGO_TIME_MS) {
            return; // sigue mostrando el logo, no hace nada más todavía
        }
        bootComplete = true;
        Serial.println(F("[BOOT] ventana de arranque terminada, pasando a RUN"));
    }

    // Arranque terminado: atiende consola y anima sin bloquear
    debugSerialTick();
    updateEyes();
}
