from plotcat import plotter
import sys
import serial
import time

if sys.version_info[0] < 3:
    import thread

else:
    import _thread as thread

samples = 100
ser = serial.Serial(sys.argv[1], sys.argv[2])
p = plotter(number_of_samples=[samples,samples], total_plots=2, rows=1, cols=2, names=["Capteur 0","Capteur 1"],y_high_lim=90)
textSensor0 = p.plots[0].text(0, 85, "Val: ")
textSensor1 = p.plots[1].text(0, 85, "Val: ")
textState = p.plots[0].text(0, 90, "State: ")
strState = ""
sensor0 = 0
sensor1 = 0
data = []

state = ["WALL_0","BIG_TURN_0", "SWITCH_WALL", "WALL_1", "BIG_TURN_1", "READY"]

data.append([0 for i in range(samples)])
data.append([0 for i in range(samples)])




def read_from_serial():
    global sensor0
    global sensor1
    global strState
    global state
    while True:

        try:

            temp = ser.readline()
            try:

                if temp[:2] == "0:":
                    sensor0 = int(temp[2:])
                    data[0].append(sensor0)
                    data[0].pop(0)

                elif temp[:2] == "1:":
                    sensor1 = int(temp[2:])
                    data[1].append(sensor1)
                    data[1].pop(0)
                elif temp[:2] == "2:":
                    strState = state[int(temp[2:])]
                elif temp[:2] == "3:":
                    if len(temp[2:]) != 0:
                        print(temp[2:])
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
    p.lines[1][0].set_data(p.currentAxis[1], data[1])
    textSensor0.set_text("Val: "+str(sensor0))
    textSensor1.set_text("Val: " + str(sensor1))
    textState.set_text("State: "+str(strState))

if __name__ == '__main__':

    thread.start_new_thread(read_from_serial, ())
    p.set_call_back(setval)
    plotter.show()
