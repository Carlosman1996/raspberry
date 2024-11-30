#include <iostream>
#include <wiringPi.h>
using namespace std;


int wait_until_condition(int pin, bool condition) {
    int timeout = 1000000;  // seconds
    int iterations = 0;
    do {
        delayMicroseconds(1);
        iterations++;
    }
    while (digitalRead(pin) != condition & iterations < timeout);

    if (iterations == timeout) {
        throw runtime_error("Timeout while pin " + to_string(pin) + " has to reach " + to_string(condition) + " status");
    }
    return iterations;
}


void read_data(int pin) {
    unsigned char data[5] = {0, 0, 0, 0, 0};

    pinMode(pin, OUTPUT);

    digitalWrite(pin, LOW);
    delayMicroseconds(18000);

    digitalWrite(pin, HIGH);
    delayMicroseconds(40);

    pinMode(pin, INPUT);
    wait_until_condition(pin, HIGH);
    wait_until_condition(pin, LOW);

    for (int bit = 0; bit < 40; bit ++) {
        wait_until_condition(pin, HIGH);
        
        int bit_width = wait_until_condition(pin, LOW);

        int array_index = bit / 8;
        data[array_index] = data[array_index] << 1;
        if (bit_width > 28) {
            data[array_index] = data[array_index] | 1;
        }
    }

    pinMode(pin, OUTPUT);

    // Checksum:
    unsigned char checksum = 0;
    for (int i = 0; i < 4; i++) { checksum += data[i]; }
    
    float temperature = data[2] + (data[3] / 10.0f);
    float humidity = data[0] + (data[1] / 10.0f);
    if (checksum == data[4]) {
        cout << "Checksum is correct" << endl;
        cout << "Temperature is: " << temperature << " ºC" << endl;
        cout << "Humidity is: " << humidity << " %" << endl;
    }
    else {
        cout << "Checksum is INCORRECT" << endl;
    }
}


int main() {
    cout << "Start program" << endl;

    int pin = 7;

    wiringPiSetup();

    bool read = false;
    while (!read) {
        try {
            read_data(pin);
            read = true;
        }
        catch (int e) {
            cout << "An exception occurred. Exception Nr. " << e << endl;
            continue;
        }
    }

    return 0;

}