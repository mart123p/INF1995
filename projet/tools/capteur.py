"""
#               Diagnostique de snoopy
#   
#   Script python utilisé pour déboguer le robot snoopy.
#   Ce script lit le port série et interprètte les résultats
#   Il affiche les résultats sur un graphique en temps réel.
#   Il peut interpréter les informations suivantes:
#
#   - valeur 2 capteurs en cm   
#   - valeur 2 capteurs CAN
#   - état de la machine à état
#   - 32 octets de texte pour faciliter le débogage
#
#   Code original de Girish Joshi créateur de plotcat sous 
#   licence GPL 3.0
#   https://bit.ly/2JAAo8Z
#
#   Utilisation:
#   python capteur.py /dev/ttyUSB0 9600
"""
from plotcat import plotter
import sys
import serial
import time

if sys.version_info[0] < 3:
    import thread

else:
    import _thread as thread

start_time = time.time()

samples = 100
ser = serial.Serial(sys.argv[1], sys.argv[2])
p = plotter(number_of_samples=[samples,samples], total_plots=2,plot_lines=2, rows=1, cols=2, names=["Capteurs cm","Capteurs ADC"],y_high_lim=90)
p.plots[1].set_ylim([0,800])

textSensor0cm = p.plots[0].text(0,90, "Val 0: ", color='C0')
textSensor1cm = p.plots[0].text(0, 85, "Val 1: ", color='C1')
textState = p.plots[0].text(0, 80, "State: ")

textSensor0adc = p.plots[1].text(0, 770, "Val 0: ", color='C0')
textSensor1adc = p.plots[1].text(0, 740, "Val 1: ", color='C1')
strState = ""

sensor0cm = 0
sensor1cm = 0
sensor0adc = 0
sensor1adc = 0

data = []

state = ["WALL_0","BIG_TURN_0", "SWITCH_WALL", "WALL_1", "BIG_TURN_1", "READY","GROS_AJUSTEMENT_0","GROS_AJUSTEMENT_1"]

data.append([0 for i in range(samples)])
data.append([0 for i in range(samples)])
data.append([0 for i in range(samples)])
data.append([0 for i in range(samples)])




def read_from_serial():
    global sensor0cm
    global sensor1cm
    global sensor1adc
    global sensor0adc
    global strState
    global start_time
    global state
    while True:

        try:

            temp = ser.readline()
            try:

                if temp[:2] == "0:":
                    sensor0cm = int(temp[2:])
                    data[0].append(sensor0cm)
                    data[0].pop(0)

                elif temp[:2] == "1:":
                    sensor1cm = int(temp[2:])
                    data[1].append(sensor1cm)
                    data[1].pop(0)
                elif temp[:2] == "2:":
                    sensor0adc = int(temp[2:])
                    data[2].append(sensor0adc)
                    data[2].pop(0)
                elif temp[:2] == "3:":
                    sensor1adc = int(temp[2:])
                    data[3].append(sensor1adc)
                    data[3].pop(0)
                elif temp[:2] == "4:":
                    strState = state[int(temp[2:])]
                elif temp[:2] == "5:":
                    if len(temp[2:]) != 0:
                        string = temp[2:].replace('\n','')
                        print("["+"{:.{}f}".format(time.time() - start_time,4)+"] "+string)
            except ValueError as Ve:
                pass

        except AttributeError as Ae:
            pass

        except TypeError as Te:
            pass

        except Exception as e:
            pass


@p.plot_self
def setval():

    p.lines[0][0].set_data(p.currentAxis[0], data[0])
    p.lines[1][0].set_data(p.currentAxis[0], data[1])
    p.lines[2][0].set_data(p.currentAxis[1], data[2])
    p.lines[3][0].set_data(p.currentAxis[1], data[3])

    textSensor0cm.set_text("Val 0: " + str(sensor0cm))
    textSensor1cm.set_text("Val 1: " + str(sensor1cm))
    textSensor0adc.set_text("Val 0: " + str(sensor0adc))
    textSensor1adc.set_text("Val 1: " + str(sensor1adc))

    textState.set_text("State: "+str(strState))

if __name__ == '__main__':

    thread.start_new_thread(read_from_serial, ())
    p.set_call_back(setval)
    plotter.show()
