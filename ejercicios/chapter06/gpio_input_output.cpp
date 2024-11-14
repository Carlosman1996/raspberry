#include <iostream>
#include <fstream>
#include <unistd.h>
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
    ifstream ifs;

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

    cout << "Iniciando monitoreo..." << endl;

    string input;
    bool turn_on = false;
    int iteration = 0;
    int timeout = 0;
    while (iteration < 5 && timeout < 600) {

        ifs.open(gpio_input_pin_path + "value");
        getline(ifs, input);
        ifs.close();

        ofs.open(gpio_output_pin_path + "value");

        if (input == "1") {
            ofs << 1;
            turn_on = true;
        }
        else {
            ofs << 0;
            if (turn_on) {
                cout << "Iteration: " + to_string(iteration + 1) << endl;
                iteration++;
                turn_on = false;
            }
        }

        ofs.close();

        timeout++;
        usleep(100000);

    }

    // Desexportar GPIOs:
    ofs.open(gpio_path + "unexport");
    ofs << output_pin;
    ofs.close();

    ofs.open(gpio_path + "unexport");
    ofs << input_pin;
    ofs.close();

    return 0;

}