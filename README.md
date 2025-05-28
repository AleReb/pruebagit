      #gasdas

      ASDAS

*        esto sie stara dentroa del espacuio?
*        SADASDASDASD

*        ASDASDASD
*    hasta cuanando
  esto esta dentro del del bloque nuevo?SASDASDASDASDAS
                                ESTO ESTARA DENTRO DELPARRAFO COPIABLE
     S3EGURA AQUI?
  #   ASDASDASDASD 12123   
        HOASDASD}A
        ASSDAASDASD
        ASDASDASDASDASD

        ASDASDDASDASASDJASDJASD
# ASDHASDHASD
## DASDASD
*ASDASDAS
 *SASDSAD
   **SDASDASDASD
![gato peleandon,con pc.](/img/gato.png)
<!-- Esto es un comentario que no será visible en GitHub para hacer los bloques copiables mejor que los los espacipos es usar los acentos alrevez -->

```cpp
#include <Arduino.h>
#include "lovyanGfxSetup.h"
#include <WiFi.h>
#include "imagen.h"
#include "imagen2.h" 

LGFX tft;

// Variables globales
String networks[10];      // Lista de redes WiFi
int networkCount = 0;     // Número de redes WiFi encontradas
int selectedNetwork = -1; // Red seleccionada
String inputPassword = ""; // Contraseña ingresada

bool showingMainMenu = true;
bool enteringPassword = false;
bool isConnected = false;
bool showingNetworks = false; // Nueva variable para saber si estamos mostrando las redes
bool connectionError = false; // Variable para saber si hubo un error de conexión
bool isUppercase = false; // Variable para indicar si el modo mayúsculas está activo

// Configuración del teclado
const int keyWidth = 46;
const int keyHeight = 52;
const int startX = 10;
const int startY = 200;
const char *keys[5][10] = { // Añadida una fila más con la tecla Mayúsculas
    {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
    {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"},
    {"a", "s", "d", "f", "g", "h", "j", "k", "l", "borr"},
    {"z", "x", "c", "v", "b", "n", "m", ".", "-", "ent"},
    {"May", "@", "#", "$", "%", "&", "*", "(", ")", "?"} // Fila nueva con la tecla Mayúsculas y caracteres especiales
};

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_LIGHTGRAY);

  // Establece la fuente integrada
  tft.setFont(&fonts::FreeSans9pt7b);

  drawTopRectangle();  // Dibujar el rectángulo largo en la parte superior
  drawRectangles();    // Dibujar los rectángulos enumerados
  drawSquares();       // Dibujar los cuadrados con bordes curvos
}

void loop() {
  int x, y;

  if (showingMainMenu) {
    if (tft.getTouch(&x, &y)) {
      if (x > 0 && x < tft.width() && y > 0 && y < 60) {
        showingMainMenu = false;
        showingNetworks = true;
        drawScanMessage();
        scanNetworks();
        drawNetworkList();
      }
    }
  } else if (showingNetworks) {
    if (tft.getTouch(&x, &y)) {
      if (x > 10 && x < 100 && y > tft.height() - 50) {
        delay(500); // Añadir retraso
        showingNetworks = false;
        showingMainMenu = true;
        tft.fillScreen(TFT_LIGHTGRAY);
        drawTopRectangle();
        drawRectangles();
        drawSquares();
      } else {
        int networkIdx = getNetworkIndex(x, y);
        if (networkIdx != -1) {
          selectedNetwork = networkIdx;
          showingNetworks = false;
          enteringPassword = true;
          connectionError = false; // Resetear el estado de error de conexión
          drawPasswordInput(networks[selectedNetwork]);
          drawKeyboard();  // Dibuja el teclado
        }
      }
    }
  } else if (enteringPassword) {
    if (tft.getTouch(&x, &y)) {
      String key = getKeyPressed(x, y);
      if (key != "") {
        handlePasswordInput(key);
        delay(500); // Esperar 1 segundo después de presionar una tecla
      }
    }

    if (tft.getTouch(&x, &y)) {
      if (x > 10 && x < 100 && y > tft.height() - 50) {
        delay(500); // Añadir retraso
        enteringPassword = false;
        showingNetworks = true;
        connectionError = false; // Resetear el estado de error de conexión
        drawNetworkList();
      }
    }
  }

  if (isConnected) {
    drawConnectedMessage(WiFi.localIP().toString());  // Mostrar IP
    delay(5000); // Esperar 5 segundos antes de reiniciar
    ESP.restart();
  }
}

void drawTopRectangle() {
  int rectHeight = 60;
  int rectWidth = tft.width();

  tft.fillRect(0, 0, rectWidth, rectHeight, TFT_WHITE);
  tft.drawRect(0, 0, rectWidth, rectHeight, TFT_BLACK);

  // Reemplazar el círculo rojo por la imagen
  tft.drawXBitmap(10, 10, imagen_bits, imagen_width, imagen_height, TFT_BLACK);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(rectWidth / 2 - 40, rectHeight / 2 - 10);
  tft.print("AYT-045");

  // Reemplazar el ícono ⚙ con la nueva imagen
  int iconX = rectWidth - imagen_2_width - 10;
  int iconY = 10;
  tft.drawXBitmap(iconX, iconY, imagen_2_bits, imagen_2_width, imagen_2_height, TFT_BLACK);
}

void drawRectangles() {
  int rectWidth = tft.width() - 40;
  int rectHeight = (tft.height() - 200) / 3 * 0.7;
  int spaceBetweenRects = 10;
  int startX = 20;
  int startY = 80 + spaceBetweenRects;
  int cornerRadius = 20; // Radio de las esquinas

  tft.fillRoundRect(startX, startY, rectWidth, rectHeight, cornerRadius, TFT_WHITE);
  tft.drawRoundRect(startX, startY, rectWidth, rectHeight, cornerRadius, TFT_BLACK);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(startX + 10, startY + 10);
  tft.print("Estado");

  String dateTime = "En Espera de inicio";
  tft.setTextSize(2);
  int textWidth = tft.textWidth(dateTime);
  int textX = (rectWidth - textWidth) / 2 + startX;
  int textY = startY + (rectHeight / 2) - 10;
  tft.setCursor(textX, textY);
  tft.print(dateTime);

  startY += rectHeight + spaceBetweenRects;
  tft.fillRoundRect(startX, startY, rectWidth, rectHeight, cornerRadius, TFT_WHITE);
  tft.drawRoundRect(startX, startY, rectWidth, rectHeight, cornerRadius, TFT_BLACK);

  tft.setTextSize(1);
  tft.setCursor(startX + 10, startY + 10);
  tft.print("Inicio programado:");

  tft.setTextSize(2);
  int textWidth2 = tft.textWidth("10:45  07/10/2024");
  int textX2 = (rectWidth - textWidth2) / 2 + startX;
  int textY2 = startY + (rectHeight / 2) - (tft.fontHeight() / 2);
  tft.setCursor(textX2, textY2);
  tft.print("10:45  07/10/2024");

  startY += rectHeight + spaceBetweenRects;
  tft.fillRoundRect(startX, startY, rectWidth, rectHeight, cornerRadius, TFT_WHITE);
  tft.drawRoundRect(startX, startY, rectWidth, rectHeight, cornerRadius, TFT_BLACK);

  int midX = startX + rectWidth / 2;
  tft.setTextSize(1);
  tft.setCursor(startX + 10, startY + 10);
  tft.print("Suministrado");

  tft.setTextSize(2);
  tft.setCursor(startX + 90, startY + 60);
  tft.print("60");

  tft.setTextSize(1);
  tft.setCursor(startX + 100, startY + 110);
  tft.print("mL");

  tft.setTextSize(1);
  tft.setCursor(midX + 50, startY + 10);
  tft.print("Total");

  tft.setTextSize(2);
  tft.setCursor(midX + 70, startY + 60);
  tft.print("1000");

  tft.setTextSize(1);
  tft.setCursor(midX + 100, startY + 110);
  tft.print("mL");

  tft.setTextSize(4);
  tft.setCursor(midX - 5, startY + 50);
  tft.print("/");
}

void drawSquares() {
  int squareSize = (tft.width() - 60) / 2;
  int padding = 20;
  int startX = (tft.width() - (2 * squareSize + padding)) / 2;
  int startY = tft.height() - squareSize - 40;

  for (int i = 0; i < 2; i++) {
    int squareX = startX + i * (squareSize + padding);
    tft.fillRoundRect(squareX, startY, squareSize, squareSize, 20, TFT_WHITE);
    tft.drawRoundRect(squareX, startY, squareSize, squareSize, 20, TFT_BLACK);

    if (i == 0) {
      tft.setTextSize(1);
      String timeText = "Tiempo en sesion";
      int textWidth1 = tft.textWidth(timeText);
      int textX1 = squareX + (squareSize - textWidth1) / 2;
      tft.setCursor(textX1, startY + 30);
      tft.print(timeText);

      String minutesText = "15";
      tft.setTextSize(2);
      int textWidth2 = tft.textWidth(minutesText);
      int textX2 = squareX + (squareSize - textWidth2) / 2;
      int textY2 = startY + (squareSize / 2) - (tft.fontHeight() / 2);
      tft.setCursor(textX2, textY2);
      tft.print(minutesText);

      String unitText = "minutos";
      int textWidth3 = tft.textWidth(unitText);
      int textX3 = squareX + (squareSize - textWidth3) / 2 ;
      tft.setTextSize(1);
      int textY3 = startY + squareSize - 50;
      tft.setCursor(textX3, textY3);
      tft.print(unitText);
    } else {
      tft.setTextSize(1);
      String text1 = "Flujo actual";
      int textWidth4 = tft.textWidth(text1);
      int textX4 = squareX + (squareSize - textWidth4) / 2;
      tft.setCursor(textX4, startY + 30);
      tft.print(text1);

      tft.setTextSize(2);
      String flowText = "1000";
      int textWidth5 = tft.textWidth(flowText);
      int textX5 = squareX + (squareSize - textWidth5) / 2;
      int textY5 = startY + (squareSize / 2) - (tft.fontHeight() / 2) + 10;
      tft.setCursor(textX5, textY5);
      tft.print(flowText);

      tft.setTextSize(1);
      String unitText2 = "mL/Hr";
      int textWidth6 = tft.textWidth(unitText2);
      int textX6 = squareX + (squareSize - textWidth6) / 2;
      tft.setCursor(textX6, startY + squareSize - 50);
      tft.print(unitText2);
    }
  }
}

void drawScanMessage() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.print("Escaneando redes...");
}

void scanNetworks() {
  networkCount = WiFi.scanNetworks();
  int validNetworkCount = 0; // Contador para redes válidas

  for (int i = 0; i < networkCount && validNetworkCount < 10; i++) {
    String ssid = WiFi.SSID(i);

    // Limitar la longitud del nombre de la red a 32 caracteres
    if (ssid.length() > 32) {
      ssid = ssid.substring(0, 32);
    }

    // Filtrar redes con caracteres no imprimibles
    bool validSSID = true;
    for (int j = 0; j < ssid.length(); j++) {
      if (ssid[j] < 32 || ssid[j] > 126) {
        validSSID = false;
        break;
      }
    }

    // Añadir a la lista de redes solo si es válido
    if (validSSID) {
      networks[validNetworkCount] = ssid;
      validNetworkCount++;
    }
  }
  networkCount = validNetworkCount; // Actualizar el contador de redes válidas
}

void drawNetworkList() {
  tft.fillScreen(TFT_LIGHTGRAY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  int networkSpacing = 70; // Ajustar el espaciado entre las redes
  for (int i = 0; i < networkCount; i++) {
    tft.setCursor(10, 30 + i * networkSpacing);
    tft.print(i + 1);
    tft.print(": ");
    tft.print(networks[i]);
  }
  tft.fillRect(10, tft.height() - 50, 90, 40, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(20, tft.height() - 40);
  tft.print("Atras");
}

int getNetworkIndex(int x, int y) {
  int networkSpacing = 70; // Ajustar el espaciado entre las redes
  int idx = (y - 30) / networkSpacing;
  return (idx >= 0 && idx < networkCount) ? idx : -1;
}

void drawPasswordInput(String networkName) {
  tft.fillScreen(TFT_LIGHTGRAY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.print("Conectar a:");
  tft.setCursor(10, 40);
  tft.print(networkName);

  tft.setCursor(10, 80);
  tft.print(inputPassword);

  tft.fillRect(10, tft.height() - 50, 90, 40, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(20, tft.height() - 40);
  tft.print("Atras");

  if (connectionError) {
    tft.setTextColor(TFT_RED);
    tft.setTextSize(1);
    tft.setCursor(10, 120);
    tft.print("ERROR EN CONEXION");
  }
}

String getKeyPressed(int x, int y) {
  int row = (y - startY) / keyHeight;
  int col = (x - startX) / keyWidth;
  if (row >= 0 && row < 5 && col >= 0 && col < 10) { // Ajustado para 5 filas
    return String(keys[row][col]);
  }
  return "";
}

void handlePasswordInput(String key) {
  if (key == "borr") {
    if (inputPassword.length() > 0) {
      inputPassword.remove(inputPassword.length() - 1);
    }
    drawPasswordInput(networks[selectedNetwork]);
    drawKeyboard();
  } else if (key == "ent") {
    WiFi.begin(networks[selectedNetwork].c_str(), inputPassword.c_str());
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      if (millis() - startTime > 5000) {
        break;
      }
      delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
      isConnected = true;
      String ipAddress = WiFi.localIP().toString();
      drawConnectedMessage(ipAddress);
    } else {
      drawPasswordInput(networks[selectedNetwork]);
      drawKeyboard();
      tft.setTextColor(TFT_RED);
      tft.setTextSize(1);
      tft.setCursor(10, 120);
      tft.print("ERROR EN CONEXION");
      delay(2000);
      drawPasswordInput(networks[selectedNetwork]);
      drawKeyboard();
    }
  } else if (key == "May") { // Alternar mayúsculas
    isUppercase = !isUppercase;
    drawKeyboard();
  } else {
    if (isUppercase) {
      key.toUpperCase();
    }
    inputPassword += key;
    drawPasswordInput(networks[selectedNetwork]);
    drawKeyboard();
  }
}

void drawConnectedMessage(String ipAddress) {
  tft.fillScreen(TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.print("Conectado exitosamente");
  
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.print("IP: ");
  tft.print(ipAddress);
}

void drawKeyboard() {
  int keyX = startX;
  int keyY = startY;

  for (int row = 0; row < 5; row++) { // Ajustado para 5 filas
    for (int col = 0; col < 10; col++) {
      tft.fillRect(keyX + col * keyWidth, keyY + row * keyHeight, keyWidth, keyHeight, TFT_WHITE);
      tft.drawRect(keyX + col * keyWidth, keyY + row * keyHeight, keyWidth, keyHeight, TFT_BLACK);
      tft.setTextColor(TFT_BLACK);

      // Usar la fuente FreeSans24pt7b
      tft.setFont(&fonts::FreeSans9pt7b);
      
      // Centrar el texto en las teclas
      String keyLabel = keys[row][col];
      if (isUppercase && row < 4) {
        keyLabel.toUpperCase(); // Convertir a mayúsculas si está activo
      }

      int textWidth = tft.textWidth(keyLabel);
      int textHeight = tft.fontHeight();
      int cursorX = keyX + col * keyWidth + (keyWidth - textWidth) / 2;
      int cursorY = keyY + row * keyHeight + (keyHeight - textHeight) / 2;
      tft.setCursor(cursorX, cursorY);

      if (keyLabel == "May") { // Mostrar estado de la tecla Mayúsculas
        tft.print(isUppercase ? "MAY" : "May");
      } else {
        tft.print(keyLabel);
      }
    }
  }
}
```
> [!NOTE]
> Useful information that users should know, even when skimming content.

> [!TIP]
> Helpful advice for doing things better or more easily.

!! esto es una alerta

> [!IMPORTANT]
> Key information users need to know to achieve their goal.

> [!WARNING]
> Urgent info that needs immediate user attention to avoid problems.

> [!CAUTION]
> Advises about risks or negative outcomes of certain actions.

- [x] #739
- [ ] https://github.com/octo-org/octo-repo/issues/740
- [ ] Add delight to the experience when all tasks are complete :tada:
