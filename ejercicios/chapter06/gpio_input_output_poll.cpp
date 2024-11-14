// NOTA: ejecutar con SUDO, de lo contrario no funcionará.

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
using namespace std;


#define GPIO            "/sys/class/gpio/"
#define GPIO_OFFSET     512


int main() {
    cout << "Start GPIO control LED program" << endl;
   
    int output_pin = 14 + GPIO_OFFSET;
    int input_pin = 17 + GPIO_OFFSET;
    string gpio_path = GPIO;
    string gpio_output_pin_path = gpio_path + ("gpio" + to_string(output_pin)) + "/";
    string gpio_input_pin_path = gpio_path + ("gpio" + to_string(input_pin)) + "/";
    ofstream ofs;

    // Exportar GPIOs:
    ofs.open(gpio_path + "export");
    ofs << output_pin;
    ofs.close();

    ofs.open(gpio_path + "export");
    ofs << input_pin;
    ofs.close();

    // Configurar dirección de GPIOs:
    ofs.open(gpio_output_pin_path + "direction");
    ofs << "out";
    ofs.close();

    ofs.open(gpio_input_pin_path + "direction");
    ofs << "in";
    ofs.close();

    // Configurar flanco de GPIO:
    ofs.open(gpio_input_pin_path + "edge");
    ofs << "both";
    ofs.close();

    // Abrir archivo GPIO del botón en modo lectura:
    int button_fd = open((gpio_input_pin_path + "value").c_str(), O_RDONLY);

    // Configurar poll:
    struct pollfd pfd;
    pfd.fd = button_fd;
    pfd.events = POLLPRI; // Monitorea eventos de valor GPIO

    // Lee el valor inicial para descartar cambios previos:
    char c;
    lseek(button_fd, 0, SEEK_SET);  // Resetea el offset para leer el nuevo valor
    read(button_fd, &c, 1);

    cout << "Iniciando monitoreo..." << endl;

    string input;
    bool turn_on = false;
    for (int iteration = 0; iteration < 5;) {

        // Espera eventos en el GPIO del botón:
        int ret = poll(&pfd, 1, 60000); // Timeout 60 segundos hasta que haya un cambio

        if (ret == 0) {
            cout << "Timeout reached!" << endl;
            break;
        }
        else { // Detecta un cambio en el valor del botón

            lseek(button_fd, 0, SEEK_SET);
            read(button_fd, &c, 1);

            ofs.open(gpio_output_pin_path + "value");

            if (c == '1') {
                ofs << 1;
                cout << "Flanco de subida detectado: LED encendido" << endl;
            } else {
                ofs << 0;
                cout << "Flanco de bajada detectado: LED apagado" << endl;
                iteration++;
            }

            ofs.close();
        }
    }

    close(button_fd);

    // Desexportar GPIOs:
    ofs.open(gpio_path + "unexport");
    ofs << output_pin;
    ofs.close();

    ofs.open(gpio_path + "unexport");
    ofs << input_pin;
    ofs.close();

    return 0;

}