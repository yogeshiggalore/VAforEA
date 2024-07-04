import serial
import time
import datetime
import pyautogui
import matplotlib.pyplot as plt

file_path = "./Screenshots/"
screenshot = "VAforEA-SS-"
format = "%d-%m-%Y_%H-%M-%S"
fileformat = ".png" 

port = "COM5"
baud_rate = 115200
x_data = []
y_data = []

mySerialport = serial.Serial(port=port, baudrate=baud_rate)  # Replace 'COM7' with the port name
# time.sleep(2)
plt.ion()

# fig = plt.gcf() 
# # Get current figure
fig, ax = plt.subplots()
fig.canvas.manager.full_screen_toggle() 
fig.suptitle('Voice Assistence for Engineering Automation')

while True:
    
    data_available = mySerialport.inWaiting()

    if data_available > 0:
        data_bytes = mySerialport.read()  # Read data from the port
        
        if (data_bytes[0]==0xAA):
            data_bytes = mySerialport.read()
            if (data_bytes[0]==0x55):
                data_bytes = mySerialport.read()
                if (data_bytes[0]==0x80):
                    data_bytes = mySerialport.read()
                    if (data_bytes[0]==0x00):
                        data_bytes = mySerialport.read()
                        if (data_bytes[0]==0x00):
                            data_bytes = mySerialport.read()
                            if (data_bytes[0]==0xA5):
                                data_bytes = mySerialport.read()
                                if (data_bytes[0]==0x5A):
                                    data_bytes = mySerialport.read()
                                    if (data_bytes[0]==0x0D):
                                        data_bytes = mySerialport.read()
                                        if (data_bytes[0]==0x0A):
                                            data_bytes = mySerialport.read()
                                            # print("Match")
                                            now_utc = datetime.datetime.now()
                                            current_time = now_utc.strftime(format)
                                            filename = file_path + screenshot + current_time + fileformat
                                            image = pyautogui.screenshot(filename)
                                            print("Captured!!")
                elif (data_bytes[0]==0x81) :
                    data_bytes = mySerialport.read()
                    voltageM = data_bytes[0] << 8
                    data_bytes = mySerialport.read()
                    voltageL = data_bytes[0]
                    Voltage = (voltageM | voltageL)/1000.0

                    x_data.append(time.time()) # Append current time for x-axis
                    y_data.append(Voltage) # Append the ADC value for y-axis


                    ax.clear() # Clear previous plot
                    ax.plot(x_data[-50:], y_data[-50:], color='red', marker='*',markersize=5, linewidth=1) # Plot the data
                    fig.supxlabel('time')#plt.xlabel('Time')
                    plt.ylim(-1,13)
                    fig.supylabel('Voltage')#plt.ylabel('Voltage')
                    plt.grid(True)
                    plt.pause(0.100) # Update the plot every 0.1 seconds
                    plt.show()
                    
mySerialport.close()


