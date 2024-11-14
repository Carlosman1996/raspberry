# Conexión PC-Raspberry via SSH

1. Alimentar Raspberry y conectar a PC vía cable ethernet.
2. Habilitar ssh en Raspberry.
3. Encontrar la IP de la Raspberry con el comando:
```bash
nmap -T4 -F 192.168.1.*
```
4. Conectar vía ssh:
```bash
ssh <usuario>@192.168.1.<octeto>
```

# Cambiar el nivel de ejecución con el gestor de sistema y servicios

Uso de Raspberry como dispositivo de red, sin monitor:
```bash
sudo systemctl isolate multi-user.target
systemctl list-units --type=target | grep graphical
```

Por defecto:
```bash
sudo systemctl set-default multi-user.target
systemctl get-default
```

# Frecuencia de trabajo en la CPU del RPi

Estado actual de la CPU:
```bash
cpufreq-info
```

Se puede cambiar la frecuencia y gobernador desde cpu-freq. 

# Comandos para ejecutar un script

Versión C++:

```bash
g++ -v
```

Instalación de C++ (buscando previamente las versiones disponibles):

```bash
sudo apt update && sudo apt install build-essential
apt search '^g\+\+-[0-9]+$'
sudo apt install g++-12
```

Compilación y ejecución:

```bash
g++-12 ejercicios/chapter06/gpio_output.cpp -o gpio_output
./gpio_output
```

```bash
g++-12 ejercicios/chapter06/gpio_input_output_threads.cpp -o gpio_input_output_threads -pthread
./gpio_input_output_threads
```

# Revisar estado de la carga del procesador:

Deberá estar por debajo de 4 cada medida (15, 5, 1 minutos), por lo general 2.8 (70%):

```bash
uptime
```

# WiringPi

Instalación:

```bash
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi/
./build
```

Read pins:

```bash
gpio readall
```

Compile:

```bash
g++-12 gpio_input_output_wiringPi.cpp -o gpio_input_output_wiringPi -lwiringPi
sudo ./gpio_input_output_wiringPi
```
