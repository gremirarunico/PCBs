#!/usr/env python3
import pyvisa
import serial
import time
import csv

currlim = 6.0

# Connect to instrumens and converter
rm = pyvisa.ResourceManager()
rm.list_resources()

powerSupply = rm.open_resource('USB0::0x05E6::0x2230::802901012757210041::0::INSTR')
oscilloscope = rm.open_resource('USB0::0x0699::0x0411::C020941::0::INSTR')
multimeter = rm.open_resource('USB0::0x05E6::0x7510::04479984::0::INSTR')
powerConverter = serial.Serial(port="COM5", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)


powerSupplyResponse = "Keithley instruments, 2230G-30-6, 802901012757210041, 1.01-1.07\n"
oscilloscopeResponse = "TEKTRONIX,DPO3014,C020941,CF:91.1CT FV:v2.38 \n"
multimeterResponse = "KEITHLEY INSTRUMENTS,MODEL DMM7510,04479984,1.7.2b\n"

# Check correctness of instrumentation
if(powerSupply.query("*IDN?") != powerSupplyResponse):
    print("PSU not correct")
    exit(1)
if(oscilloscope.query("*IDN?") != oscilloscopeResponse):
    print("Oscilloscope not correct")
    exit(1)
if(multimeter.query("*IDN?") != multimeterResponse):
    print("Multimeter not correct")
    exit(1)

# Configure PSU
print("PSU Configuration")
print(powerSupply.write("*RST"))
time.sleep(2)
#print(powerSupply.write("APPL CH1, 18.0, 6.0"))
print(powerSupply.write("APPL CH1, 18.0, " + str(currlim)))
print(powerSupply.write("APPL CH2, 30.0, 6.0"))
print(powerSupply.write("APPL CH3, 5.0, 1.0"))
print(powerSupply.write("OUTP ON"))

time.sleep(2)

# Configure Multimeter
print(multimeter.write("*RST"))
print(multimeter.query(":MEASure:CURRent?"))

"""
*RST
:MEASure:CURRent?
"""

# Check correctness of the power converter
powerConverter.write(b"get all \r\n")

answer1 = powerConverter.readline()
answer2 = powerConverter.readline()

answer1Response = b'F: 1250000 Hz, DT: 40 ns, ADT: 20 Duty: 50%\n' #MR 40ns
#answer1Response = b'F: 1250000 Hz, DT: 20 ns, Duty: 50%\n' #MR 20ns

#answer2Response = b'\rP: 4352, A1: 314, A2: 2272, B1: 2490, B2: 96, C1: 314, C2: 2272, D1: 2490, D2: 96\n' # MR 40ns
#answer2Response = b'\rP: 4352, A1: 205, A2: 2272, B1: 2381, B2: 96, C1: 205, C2: 2272, D1: 2381, D2: 96\n' # MR 20ns
#answer2Response = b'\rP: 4352, A1: 205, A2: 2272, B1: 2381, B2: 96, C1: 205, C2: 2272, D1: 2599, D2: 96\n' # MR 20ns + 40ns
answer2Response = b'\rP: 4352, A1: 314, A2: 2272, B1: 2490, B2: 96, C1: 314, C2: 2272, D1: 2599, D2: 96\n' # MR 40ns + 20ns



if(answer1 != answer1Response or answer2!=answer2Response):
    print("PowerConverter not correct")
    print(repr(answer1))
    print(repr(answer2))
    exit(1)
else:
    print("PowerConverter OK")
    

# Configure oscilloscope
time.sleep(1)
oscilloscope.write("MEASU:MEAS1:TYP RMS")
oscilloscope.write("MEASUrement:MEAS1:SOUrce2")
time.sleep(1)
powerConverter.write(b"start \r\n")
print(powerConverter.readline())
time.sleep(2)

dataStored = [["Frequency", "Power1", "Power2", "Power3", "Current1",  "Current2",  "Current3", "Voltage1", "Voltage2", "Voltage3", "VoutRMS", "IoutRMS"]]

start = 700000
stop = 1500000
step = 10000
for freq in range(start, stop+1, step):
    time.sleep(0.1)
    powerConverter.write(bytes("set freq " + str(freq) + "\r\n", encoding='ascii'))
    respons = powerConverter.readline()
    print(respons)
    time.sleep(0.1)
    powerConverter.write(b"update\r\n")
    respons = powerConverter.readline()
    print(respons)
    
    #check not blocked
    if(respons != b'\rUpdate sent\n'):
        print("Serial crashed, trying to recovery")
        time.sleep(1)
        #RECOVERY
        print(powerSupply.write("*RST"))
        time.sleep(2)
        #print(powerSupply.write("APPL CH1, 18.0, 6.0"))
        print(powerSupply.write("APPL CH1, 18.0, " + str(currlim)))
        print(powerSupply.write("APPL CH2, 30.0, 6.0"))
        print(powerSupply.write("APPL CH3, 5.0, 1.0"))
        print(powerSupply.write("OUTP ON"))
        time.sleep(2)
        powerConverter.write(b"start \r\n")
        print(powerConverter.readline())
        time.sleep(2)
        powerConverter.write(bytes("set freq " + str(freq) + "\r\n", encoding='ascii'))
        print(powerConverter.readline())
        powerConverter.write(b"update\r\n")
        respons = powerConverter.readline()
        print(respons)
    
    time.sleep(2)
    """print(powerSupply.query("MEAS:POWER? ALL"))
    print(powerSupply.query("MEAS:CURR? ALL"))
    print(powerSupply.query("MEAS:VOLT? ALL"))
    print(oscilloscope.query("MEASUrement:MEAS1:VALue?"))"""
    
    powers = powerSupply.query("MEAS:POWER? ALL")[0:-1].split(',')
    
    currents = powerSupply.query("MEAS:CURR? ALL")[0:-1].split(',')
    voltages = powerSupply.query("MEAS:VOLT? ALL")[0:-1].split(',')
    voutrms = oscilloscope.query("MEASUrement:MEAS1:VALue?")[0:-1]
    ioutrms = multimeter.query(":MEASure:CURRent?")[0:-1]
    ioutrms = multimeter.query(":MEASure:CURRent?")[0:-1]
    
    dataStored.append([str(freq), powers[0], powers[1], powers[2], currents[0], currents[1], currents[2], voltages[0], voltages[1], voltages[2], voutrms, ioutrms])

time.sleep(2)
powerConverter.write(b"stop \r\n")
print(powerConverter.readline())
print(powerSupply.write("*RST"))

f = open("dataout.csv", "w")
writer = csv.writer(f)
writer.writerows(dataStored)
f.close()

exit(0)
"""
repr()
print(powerSupply.write("*RST"))
#Configurazione
print(powerSupply.write("APPL CH1, 18.0, 1.0"))
print(powerSupply.write("APPL CH2, 30.0, 6.0"))
print(powerSupply.write("APPL CH3, 5.0, 1.0"))

# check volt MEAS:VOLT? ALL 
# check volt MEAS:POWER? ALL 
# 0, 0, 1.20145

print(powerSupply.write("OUTP ON"))

#Get data
MEASU:MEAS1:TYP RMS
MEASUrement:MEAS1:SOUrce2
MEASUrement:MEAS1:VALue?
#print(powerSupply.query("FETCh[:SCALar]:POWer[:DC]? {ALL}"))




pc = serial.Serial(port="COM3", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

"""