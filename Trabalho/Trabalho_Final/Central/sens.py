import smbus2
import bme280
import time

porta_i2c = 1
addres = 0x76
bus = smbus2.SMBus(porta_i2c)

params = bme280.load_calibration_params(bus, addres)


data = bme280.sample(bus, addres, params)

time.sleep(1)

print("T: " + str(data.temperature) + " U: " + str(data.humidity) + " P: " + str(data.pressure))

