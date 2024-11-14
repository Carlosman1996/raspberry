/*
Compile and run:

    g++-12 gpio_input_output_wiringPi.cpp -o gpio_input_output_wiringPi -lwiringPi
    sudo ./gpio_input_output_wiringPi
*/


#include <iostream>
#include <thread>
#include <wiringPi.h>
#include <atomic>
using namespace std;


#define GPIO            "/sys/class/gpio/"


// Variables globales para la comunicación entre hilos:
atomic<bool> buttonPressed(false);  // Estado del botón
atomic<bool> ledState(false);       // Estado del LED (on/off)
atomic<bool> finishThreads(false);       // Estado del LED (on/off)


// Función para leer el estado del botón
void readButton(int buttonPin) {
    int iteration = 0;
    while (iteration < 5) {

        buttonPressed = digitalRead(buttonPin);

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


void setLedStatus(int ledPin, int status) {

    digitalWrite(ledPin, status);
    ledState = status;
    cout << "LED " + to_string(status) << endl;
}


// Función para controlar el LED
void controlLed(int ledPin) {
    while (true) {
        if (buttonPressed && !ledState) {   // Si el botón está presionado y el LED apagado
            setLedStatus(ledPin, 1);
        }
        else if (!buttonPressed && ledState)
        {
            setLedStatus(ledPin, 0);
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

    int output_pin = 15;
    int input_pin = 0;

    wiringPiSetup();

    pinMode(output_pin, OUTPUT);
    pinMode(input_pin, INPUT);

    // Crear hilos
    thread buttonThread(readButton, input_pin);
    thread ledThread(controlLed, output_pin);

    // Terminar los hilos:
    buttonThread.join();
    ledThread.join();

    return 0;

}