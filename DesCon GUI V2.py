import serial
import time
import numpy as np
import matplotlib.pyplot as plt
from tkinter import *
from tkinter import messagebox
from tkinter import simpledialog

top = Tk()

port = simpledialog.askstring("Welcome","Please Enter Device Port e.g. COM5")

while port == '':
    errMsg = messagebox.showwarning(title="ERROR", message="Please enter a valid port")
    port = simpledialog.askstring("Welcome","Please Enter Device Port e.g. COM5")

ser = serial.Serial(port,38400)
portFrame = Frame(top)
voltageFrame = Frame(top)
ACVoltageFrame = Frame(top)
currentFrame = Frame(top)
resistanceFrame = Frame(top)
luxFrame = Frame(top)
transistorFrame = Frame(top)
dataFrame = Frame(top)
top.lift()
#Setup Functions

def toggleText():
    if portBut["text"] == "Close Port":
        ser.close()
        portBut["text"] = "Open Port"
    else:
        ser.open()
        portBut["text"] = "Close Port"


#DC Voltage Functions
   
def setVoltAutoRange():
    ser.write(b"0")
        
def voltageLogData():
    ser.write(b"0")
    runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    while runTime ==  '':
        messagebox.showwarning(title="ERROR",message="Invalid amount of time")
        runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    print("PLEASE WAIT")
    tl = Toplevel()
    tl.title("Please Wait")
    tl.lift()
    Label(tl, text = "Please Wait", font=("Helvetica",32)).pack()
    tl.update()
    f = open("voltage_data.txt","wt")
    startTime = time.time()
    endTime = time.time()+int(runTime)
    ser.reset_input_buffer()
    ser.flush()
    while(time.time()<endTime):
        data = ser.readline()
        print(data)
        currentTime = time.time()-startTime
        f.write(str(currentTime))
        f.write(",")
        f.write(str(data.decode("utf-8")))
        f.write("\n")     
    f.close()
    tl.destroy()
    x, y = np.loadtxt('voltage_data.txt',delimiter=',',unpack=True)
    plt.scatter(x,y)
    plt.xlabel('Time/s')
    plt.ylabel('Voltage/v')
    result = messagebox.askquestion("Best Fit","Add a best fit line?")
    if result == 'yes':
        plt.plot(np.unique(x), np.poly1d(np.polyfit(x, y, 1))(np.unique(x)))
    plt.show()
    
    print("DONE")
    ser.flush()
       
def setVolt10Range():
    ser.write(b"1")
 
def setVolt5Range():
    ser.write(b"2")
    
def setVolt2Range():
    ser.write(b"3")
    
def setVolt1Range():
    ser.write(b"4")

#AC Voltage Functions

def setACAutoRange():
    ser.write(b"5")

def RMSvoltageLogData():
    ser.write(b"5")
    runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    while runTime ==  '':
        messagebox.showwarning(title="ERROR",message="Invalid amount of time")
        runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    print("PLEASE WAIT")
    tl = Toplevel()
    tl.title("Please Wait")
    tl.lift()
    Label(tl, text = "Please Wait", font=("Helvetica",32)).pack()
    tl.update()
    f = open("RMS_data.txt","wt")
    startTime = time.time()
    endTime = time.time()+int(runTime)
    ser.reset_input_buffer()
    ser.flush();
    while(time.time()<endTime):
        data = ser.readline()
        currentTime = time.time()-startTime
        f.write(str(currentTime))
        f.write(",")
        f.write(str(data.decode("utf-8")))
        f.write("\n")     
    f.close()
    tl.destroy()
    x, y = np.loadtxt('RMS_data.txt',delimiter=',',unpack=True)
    plt.scatter(x,y)
    plt.xlabel('Time/s')
    plt.ylabel('RMS Voltage/v')
    result = messagebox.askquestion("Best Fit","Add a best fit line?")
    if result == 'yes':
        plt.plot(np.unique(x), np.poly1d(np.polyfit(x, y, 1))(np.unique(x)))
    plt.show()
    
    print("DONE")
    ser.flush()

#Resistance Function    

def setResistanceAutoRange():
    ser.write(b"6")
    
def setResistance1M():
    ser.write(b"7")
    
def setResistance100K():
    ser.write(b"8")
    
def setResistance10K():
    ser.write(b"9")
    
def setResistance100R():
    ser.write(b"a")

def resistanceLogData():
    ser.write(b"6")
    runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    while runTime ==  '':
        messagebox.showwarning(title="ERROR",message="Invalid amount of time")
        runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    print("PLEASE WAIT")
    tl = Toplevel()
    tl.title("Please Wait")
    tl.lift()
    Label(tl, text = "Please Wait", font=("Helvetica",32)).pack()
    tl.update()
    f = open("Resistance_data.txt","wt")
    startTime = time.time()
    endTime = time.time()+int(runTime)
    ser.reset_input_buffer()
    ser.flush();
    while(time.time()<endTime):
        data = ser.readline()
        print(data)
        currentTime = time.time()-startTime
        f.write(str(currentTime))
        f.write(",")
        f.write(str(data.decode("utf-8")))
        f.write("\n")
        print(currentTime)
    f.close()
    tl.destroy()
    x, y = np.loadtxt('Resistance_data.txt',delimiter=',',unpack=True)
    plt.scatter(x,y)
    plt.xlabel('Time/s')
    plt.ylabel(r'Resistance/$\Omega$')
    result = messagebox.askquestion("Best Fit","Add a best fit line?")
    if result == 'yes':
        plt.plot(np.unique(x), np.poly1d(np.polyfit(x, y, 1))(np.unique(x)))
    plt.show()
    
    print("DONE")
    ser.flush()

#Colour Functions

def setLightWhite():
    ser.write(b"g")

def setLightRed():
    ser.write(b"h")

def setLightGreen():
    ser.write(b"i")

def setLightBlue():
    ser.write(b"j")

def setColourCycle():
    ser.write(b"k")

#Current Functions
    
def setCurrentAutoRange():
    ser.write(b"b")

def setCurrent1A():
    ser.write(b"c")

def setCurrent250mA():
    ser.write(b"d")

def setCurrent150mA():
    ser.write(b"e")

def setCurrent45mA():
    ser.write(b"f")

def currentLogData():
    ser.write(b"b")
    
    runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    while runTime ==  '':
        messagebox.showwarning(title="ERROR",message="Invalid amount of time")
        runTime = simpledialog.askstring("How Long?","How long (in seconds) do you want to record data for?")
    print("PLEASE WAIT")
    tl = Toplevel()
    tl.title("Please Wait")
    tl.lift()
    Label(tl, text = "Please Wait", font=("Helvetica",32)).pack()
    tl.update()
    f = open("current_data.txt","wt")
    startTime = time.time()
    endTime = time.time()+int(runTime)
    ser.reset_input_buffer()
    ser.flush();
    while(time.time()<endTime):
        data = ser.readline()
        print(data)
        currentTime = time.time()-startTime
        f.write(str(currentTime))
        f.write(",")
        f.write(str(data.decode("utf-8")))
        f.write("\n")     
    f.close()
    tl.destroy()
    x, y = np.loadtxt('current_data.txt',delimiter=',',unpack=True)
    plt.scatter(x,y)
    plt.xlabel('Time/s')
    plt.ylabel('Current/A')
    result = messagebox.askquestion("Best Fit","Add a best fit line?")
    if result == 'yes':
        plt.plot(np.unique(x), np.poly1d(np.polyfit(x, y, 1))(np.unique(x)))
    plt.show()
    
    print("DONE")
    ser.flush()
    
#Continuity
def testContinuity():
    ser.write(b"l")
#Transistor
def testNPN():
    ser.write(b"m")
def testPNP():
    ser.write(b"n")

#Port Buttons
Label(portFrame,text = "Port Settings        ", justify = LEFT).pack(side = LEFT,expand=True,fill = 'both')
portBut = Button(portFrame, text = "Close Port", command = toggleText).pack(side = LEFT,expand=True,fill = 'both')


#DC Voltage Buttons
Label(voltageFrame,text = "Voltage Range     ").pack(side = LEFT,expand=True,fill = 'both')
Button(voltageFrame, text = "Auto Range", command = setVoltAutoRange).pack(side = LEFT,expand=True,fill = 'both')
Button(voltageFrame, text = "10 Volts", command = setVolt10Range).pack(side = LEFT,expand=True,fill = 'both')
Button(voltageFrame, text = "5 Volts", command = setVolt5Range).pack(side = LEFT,expand=True,fill = 'both')
Button(voltageFrame, text = "2 Volts", command = setVolt2Range).pack(side = LEFT,expand=True,fill = 'both')
Button(voltageFrame, text = "1 Volt", command = setVolt1Range).pack(side = LEFT,expand=True,fill = 'both')
Button(voltageFrame, text = "Datalogger", command = voltageLogData).pack(side = LEFT,expand=True,fill = 'both')

#AC Voltage Buttons
Label(ACVoltageFrame,text = "AC Voltage Range").pack(side =LEFT,expand=True,fill = 'both')
Button(ACVoltageFrame, text = "Auto Range", command = setACAutoRange).pack(side = LEFT,expand=True,fill = 'both')
Button(ACVoltageFrame, text = "Datalogger", command = RMSvoltageLogData).pack(side = LEFT,expand=True,fill = 'both')


#Current Buttons
Label(currentFrame, text = "Current Range").pack(side = LEFT,expand=True,fill = 'both')
Button(currentFrame, text = "Auto Range", command = setCurrentAutoRange).pack(side = LEFT,expand=True,fill = 'both')
Button(currentFrame, text = "1A", command = setCurrent1A).pack(side = LEFT,expand=True,fill = 'both')
Button(currentFrame, text = "250mA",command = setCurrent250mA).pack(side = LEFT,expand=True,fill = 'both')
Button(currentFrame, text = "150mA",command = setCurrent150mA).pack(side = LEFT,expand=True,fill = 'both')
Button(currentFrame, text = "45 mA",command = setCurrent45mA).pack(side = LEFT,expand=True,fill = 'both')
Button(currentFrame, text = "Datalogger", command = currentLogData).pack(side = LEFT,expand=True,fill = 'both')

#Resistance Buttons
Label(resistanceFrame,text = "Resistance").pack(side = LEFT,expand=True,fill = 'both')
Button(resistanceFrame, text = "Auto Range", command = setResistanceAutoRange).pack(side = LEFT,expand=True,fill = 'both')
Button(resistanceFrame, text = "1M", command = setResistance1M).pack(side = LEFT,expand=True,fill = 'both')
Button(resistanceFrame, text = "100K", command = setResistance100K).pack(side = LEFT,expand=True,fill = 'both')
Button(resistanceFrame, text = "10K", command = setResistance10K).pack(side = LEFT,expand=True,fill = 'both')
Button(resistanceFrame, text = "100R", command = setResistance100R).pack(side = LEFT,expand=True,fill = 'both')
Button(resistanceFrame, text = "Continuity", command = testContinuity).pack(side = LEFT,expand=True,fill = 'both')
Button(resistanceFrame, text = "Datalogger", command = resistanceLogData).pack(side = LEFT,expand=True,fill = 'both')


#Light Buttons
Label(luxFrame, text = "Light Range").pack(side = LEFT,expand=True,fill = 'both')
Button(luxFrame, text = "White", command = setLightWhite, bg = 'white').pack(side = LEFT,expand=True,fill = 'both')
Button(luxFrame, text = "Red", command = setLightRed, bg = 'firebrick2').pack(side = LEFT,expand=True,fill = 'both')
Button(luxFrame, text = "Green", command = setLightGreen, bg = 'limegreen').pack(side = LEFT,expand=True,fill = 'both')
Button(luxFrame, text = "Blue", command = setLightBlue, bg = 'deepskyblue').pack(side = LEFT,expand=True,fill = 'both')
Button(luxFrame, text = "RGB Cycle", command = setColourCycle).pack(side = LEFT,expand = True,fill = 'both')

#Transistor Buttons
Label(transistorFrame,text = "Transistor Test").pack(side = LEFT,expand=True,fill = 'both')
Button(transistorFrame, text = "Test NPN",command = testNPN).pack(side = LEFT,expand=True,fill = 'both')
Button(transistorFrame, text = "Test PNP",command = testPNP).pack(side = LEFT,expand=True,fill = 'both')
#Data Labels
dataLab = Label(dataFrame, text = "DesCon Control Panel V0.6", fg = "red")
dataLab.pack(side = LEFT,expand=True,fill = 'both')

#GUI Packing
portFrame.pack(expand=True,fill = 'both')
voltageFrame.pack(expand=True,fill = 'both')
ACVoltageFrame.pack(expand=True,fill = 'both')
currentFrame.pack(expand=True,fill = 'both')
resistanceFrame.pack(expand=True,fill = 'both')
luxFrame.pack(expand=True,fill = 'both')
transistorFrame.pack(expand=True,fill = 'both')
dataFrame.pack(expand=True,fill = 'both')

top.mainloop()
