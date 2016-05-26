# PiSensors

An python open-source library contains wrappers for different types of sensors which can be used with Raspberry Pi.

At the moment it supports the following types of sensors:

- [DHT11](https://arduino-info.wikispaces.com/DHT11-Humidity-TempSensor)

# Installation / Requirements

The library is based on the [Wiring Pi](http://wiringpi.com/) library. You can find the official installation instruction for this library [here](http://wiringpi.com/download-and-install/). In two words it should be like this:
```
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build
cd ../
```

This package should be installed in the global environment so it can ask you about root permissions. When Wiring Pi is installed and available for usage, you can install PiSensors:
```
git clone https://github.com/kyakovenko/python-pi-sensors.git
cd python-pi-sensors
python setup.py install
cd ../
```

# License

PiSensors is distributed under the MIT license. Please, see the [LICENSE](LICENSE) file.


