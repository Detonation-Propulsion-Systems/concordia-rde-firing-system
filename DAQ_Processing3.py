
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
from drawnow import *
import serial
from time import sleep



PINS = 18
i = 0

PIN1=[]
PIN2=[]
PIN3=[]
PIN4=[]
PIN5=[]
PIN6=[]
PIN7=[]
PIN8=[]
PIN9=[]
PIN10=[]
PIN11=[]
PIN12=[]
PIN13=[]
PIN14=[]
PIN15=[]
PIN16=[]
PIN17=[]
PIN18=[]

time =[]
count = 0
maxcount = 10000


def makeFig():
    plt.ylim(10, 700)
    plt.ylabel("Pressure")
    plt.autoscale
    plt.xlabel("Time (micro sec)")
    plt.grid(True)
    
    plt.plot(time, PIN1, label = 'PIN1')

    plt.plot(time, PIN2, label = 'PIN2')

    plt.plot(time, PIN3, label = 'PIN3')
    
    plt.plot(time, PIN4,  label = 'PIN4')

    plt.plot(time, PIN5,  label = 'PIN5')

    plt.plot(time, PIN6,  label = 'PIN6')

    plt.plot(time, PIN7,  label = 'PIN7')

    plt.plot(time, PIN8,  label = 'PIN8')

    plt.plot(time, PIN9,  label = 'PIN9')

    plt.plot(time, PIN10,  label = 'PIN10')
    
    plt.plot(time, PIN11,  label = 'PIN11')

    plt.plot(time, PIN12,  label = 'PIN12')

    plt.plot(time, PIN13,  label = 'PIN13')

    plt.plot(time, PIN14,  label = 'PIN14')

    plt.plot(time, PIN15,  label = 'PIN15')

    plt.plot(time, PIN16,  label = 'PIN16')

    plt.plot(time, PIN17,  label = 'PIN17')
    
    plt.plot(time, PIN18,  label = 'PIN18')

    plt.legend()


teensydata = serial.Serial('/dev/cu.usbmodem101058401', 230400)
plt.ion() #Interactive mode for plotting


while True: #loop that loops forever
    while (teensydata.inWaiting()==0): #Waiting for data
        pass #do nothing
    dataString = teensydata.readline()
    dataarray = dataString.decode('utf-8').rstrip().split(',')
    time.append(int(dataarray[-1]))
    
    #PINi.append(int(dataarray[i])) #append doesnt work with the for loop :(

    PIN1.append(float(dataarray[0]))
    PIN2.append(float(dataarray[1]))
    PIN3.append(float(dataarray[2]))
    PIN4.append(float(dataarray[3]))
    PIN5.append(float(dataarray[4]))
    PIN6.append(float(dataarray[5]))
    PIN7.append(float(dataarray[6]))
    PIN8.append(float(dataarray[7]))
    PIN9.append(float(dataarray[8]))
    PIN10.append(float(dataarray[9]))
    PIN11.append(float(dataarray[10]))
    PIN12.append(float(dataarray[11]))
    PIN13.append(float(dataarray[12]))
    PIN14.append(float(dataarray[13]))
    PIN15.append(float(dataarray[14]))
    PIN16.append(float(dataarray[15]))
    PIN17.append(float(dataarray[16]))
    PIN18.append(float(dataarray[17]))


            
    drawnow(makeFig)
    plt.pause(.000000000001)

    count = count + 1
    if (count>maxcount):
        time.pop(0)
        PIN1.pop(0)
        PIN2.pop(0)
        PIN3.pop(0)
        PIN4.pop(0)
        PIN5.pop(0)
        PIN6.pop(0)
        PIN7.pop(0)
        PIN8.pop(0)
        PIN9.pop(0)
        PIN10.pop(0)
        PIN11.pop(0)
        PIN12.pop(0)
        PIN13.pop(0)
        PIN14.pop(0)
        PIN15.pop(0)
        PIN16.pop(0)
        PIN17.pop(0)
        PIN18.pop(0)

    #print("HIT SPACE BAR TO START RECORDING DATA");
    #if (count<10000):
       # a = getkey()
       # if a == " ":
         #   pass
        #else:
          #  PIN1.pop(0)
           # PIN2.pop(0)
         #   PIN3.pop(0)
          #  PIN4.pop(0)
           # PIN5.pop(0)
            #PIN6.pop(0)
           # PIN7.pop(0)
           # PIN8.pop(0)
           # PIN9.pop(0)
            #PIN10.pop(0)
           # PIN11.pop(0)
            #PIN12.pop(0)
           # PIN13.pop(0)
            #PIN14.pop(0)
            #PIN15.pop(0)
            #PIN16.pop(0)
            #PIN17.pop(0)
            #PIN18.pop(0)
        
        
    
