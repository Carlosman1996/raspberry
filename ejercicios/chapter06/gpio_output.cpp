#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;


#define GPIO            "/sys/class/gpio/"
#define GPIO_OFFSET     512


int main() {
    cout << "Start GPIO control LED program" << endl;
   
    int pin = 14 + GPIO_OFFSET;
    string gpio_path = GPIO;
    string gpio_pin_path = gpio_path + ("gpio" + to_string(pin)) + "/";
    unsigned int seconds = 1;
    ofstream fs;

    fs.open(gpio_path + "export");
    cout << "export" << endl;
    fs << pin;
    fs.close();

    fs.open(gpio_pin_path + "direction");
    cout << "direction" << endl;
    fs << "out";
    fs.close();

    fs.open(gpio_pin_path + "value");
    cout << "Value 0" << endl;
    fs << 0;
    fs.close();

    sleep(seconds);

    fs.open(gpio_pin_path + "value");
    cout << "Value 1" << endl;
    fs << 1;
    fs.close();

    sleep(seconds);

    fs.open(gpio_pin_path + "value");
    cout << "Value 0" << endl;
    fs << 0;
    fs.close();

    sleep(seconds);

    fs.open(gpio_pin_path + "value");
    cout << "Value 1" << endl;
    fs << 1;
    fs.close();

    sleep(seconds);

    fs.open(gpio_pin_path + "value");
    cout << "Value 0" << endl;
    fs << 0;
    fs.close();


    fs.open(gpio_path + "unexport");
    cout << "unexport" << endl;
    fs << pin;
    fs.close();


    return 0;

}