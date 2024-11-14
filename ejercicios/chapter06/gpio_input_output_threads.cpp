#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <atomic>
using namespace std;


#define GPIO            "/sys/class/gpio/"
#define GPIO_OFFSET     512


// Variables globales para la comunicación entre hilos:
atomic<bool> buttonPressed(false);  // Estado del botón
atomic<bool> ledState(false);       // Estado del LED (on/off)
atomic<bool> finishThreads(false);       // Estado del LED (on/off)


// Función para leer el estado del botón
void readButton(int buttonPin) {
    string buttonPath = GPIO "gpio" + to_string(buttonPin) + "/value";
    ifstream buttonFile;

    string value;
    int iteration = 0;
    while (iteration < 5) {

        buttonFile.open(buttonPath);
        getline(buttonFile, value);
        buttonPressed = (value == "1"); // Actualiza el estado del botón
        buttonFile.close();

        if (buttonPressed && !ledState) {
            cout << "Button pressed" << endl;
        }
        else if (!buttonPressed && ledState) {
            iteration++;
        }

        this_thread::sleep_for(chrono::milliseconds(100)); // Evita lecturas muy rápidas
    
        if (finishThreads) {
            break;
        }
    }

    finishThreads = true;
}

void setLedStatus(string ledPath, int status) {
    ofstream ledFile;

    ledFile.open(ledPath);
    ledFile << status;   // Enciende el LED
    ledFile.close();

    ledState = status;

    cout << "LED " + to_string(status) << endl;
}


// Función para controlar el LED
void controlLed(int ledPin) {
    string ledPath = GPIO "gpio" + to_string(ledPin) + "/value";
    ofstream ledFile;

    while (true) {
        if (buttonPressed && !ledState) {   // Si el botón está presionado y el LED apagado
            setLedStatus(ledPath, 1);
        }
        else if (!buttonPressed && ledState)
        {
            setLedStatus(ledPath, 0);
        }
        
        this_thread::sleep_for(chrono::milliseconds(50)); // Evita que se repita continuamente
        
        if (finishThreads) {
            break;
        }
    }

    finishThreads = true;
}


int main() {
    cout << "Start GPIO control LED program" << endl;
   
    int output_pin = 14 + GPIO_OFFSET;
    int input_pin = 17 + GPIO_OFFSET;
    string gpio_output_pin_path = GPIO "gpio" + to_string(output_pin) + "/";
    string gpio_input_pin_path = GPIO "gpio" + to_string(input_pin) + "/";
    ofstream ofs;
    ifstream ifs;

    // Exportar GPIOs:
    ofs.open(GPIO "export");
    ofs << output_pin;
    ofs.close();

    ofs.open(GPIO "export");
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

    // Crear hilos
    thread buttonThread(readButton, input_pin);
    thread ledThread(controlLed, output_pin);

    // Terminar los hilos:
    buttonThread.join();
    ledThread.join();

    // Desexportar GPIOs:
    ofs.open(GPIO "unexport");
    ofs << output_pin;
    ofs.close();

    ofs.open(GPIO "unexport");
    ofs << input_pin;
    ofs.close();

    return 0;

}