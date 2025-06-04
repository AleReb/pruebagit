# VOCReader

Este repositorio contiene un sencillo ejemplo para leer un sensor de compuestos orgánicos volátiles (TVOC) utilizando Arduino. El programa usa la biblioteca `SoftwareSerial` para comunicarse con el sensor, valida el checksum de los paquetes recibidos y muestra la concentración por el puerto serie.

## Características
- Configura un puerto serie por software en los pines 3 (RX) y 4 (TX).
- Envía un comando de inicialización para colocar el sensor en modo de subida automática.
- Lee tramas de 9 bytes y verifica su integridad mediante checksum.
- Imprime la concentración de TVOC en µg/m³ por el monitor serie.

## Uso
1. Abre `VOCReader.ino` con el IDE de Arduino.
2. Conecta las líneas RX/TX del sensor a los pines 3 y 4 de la placa.
3. Compila y sube el programa al microcontrolador.
4. Abre el monitor serie a 9600 baudios para ver las lecturas.

En la carpeta `img/` se incluye una imagen de ejemplo:

![Ejemplo](img/gato.png)

## Licencia
Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más información.
