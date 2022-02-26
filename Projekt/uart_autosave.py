import serial

# Port that is used for UART communication
serialPort = serial.Serial(port="COM3", baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
serialString = ""
f = open("alarm_records.txt", "a")

while 1:
        # Wait until there is data waiting in the serial buffer
        if serialPort.inWaiting() > 0:

            # Read data out of the buffer until CR is detected
            serialString = serialPort.readline()

            # Decode contents from the buffer and strip CR
            contents: str = serialString.decode('utf-8').rstrip()

            print(contents)
            f.write(contents)
            f.write('\n')    
            
f.close()
serialPort.close()
