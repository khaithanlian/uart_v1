Transmission Path

Application
    ↓
uart_write_bytes()
    ↓
UART driver
    ↓
TX buffer / UART peripheral
    ↓
TX GPIO
    ↓
USB-UART bridge
    ↓
PC
    ↓
PuTTY

UART Write
uart_write_bytes(uart_port, data, length);

Eg.

const char message[] = "Hello from ESP32\r\n";

uart_write_bytes(
    BOARD_UART_PORT,
    message,
    sizeof(message) - 1
);

sizeof(message)-1 means (H e l l o \0) the C string terminator to exclude.

if pointer is used instead of variable, sizeof cannot be used. instead strlen(message) must be used. 
eg. 

const char *message = "Hello";

sizeof(message);    // size of pointer, NOT 5

In this case we use,
strlen(message) // this will give the actual string length of the message not the pointer length. and string.h must include from the library. #include <string.h>