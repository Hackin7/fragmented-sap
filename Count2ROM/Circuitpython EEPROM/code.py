# SPDX-FileCopyrightText: 2018 Kattni Rembor for Adafruit Industries
#
# SPDX-License-Identifier: MIT

"""CircuitPython Essentials HID Keyboard example"""
import time
import board
from digitalio import DigitalInOut, Direction, Pull

addressPins = [board.GP11, board.GP12, board.GP13, board.GP14, board.GP15, board.GP16, board.GP17, board.GP18, board.GP19, board.GP20, board.GP21]
IOPins = [board.GP0, board.GP1, board.GP2, board.GP3, board.GP4, board.GP5, board.GP6, board.GP7]
OEp, WEp, CEp = board.GP10, board.GP9, board.GP8

## Initialising Pins ###############################
address = []
for p in addressPins:
    ppp = DigitalInOut(p)
    ppp.direction = Direction.OUTPUT
    address.append(ppp)

IO = []
for p in IOPins:
    ppp = DigitalInOut(p)
    ppp.direction = Direction.OUTPUT
    IO.append(ppp)

OE, WE, CE = DigitalInOut(OEp), DigitalInOut(WEp), DigitalInOut(CEp)
OE.direction = Direction.OUTPUT
WE.direction = Direction.OUTPUT
CE.direction = Direction.OUTPUT
DELAY = .5

#def reinit(mode):
#    for p in range(len(IO)):
#        IO[p].init(mode=mode)

def setVal(arr, val):
    for p in range(len(val)):
        arr[p].value = True if val[p] else False

## Write Mode ######################################
def write(add, val):
    print(add, val)
    #reinit(Direction.OUTPUT)
    CE.value = False
    WE.value = True
    OE.value = True
    setVal(address, add)
    setVal(IO, val)
    time.sleep(DELAY)
    WE.value = False # Cycle
    time.sleep(DELAY)
    WE.value = True
    time.sleep(DELAY)
    CE.value = True
    OE.value = False
    time.sleep(DELAY)


def v(num, padding=11):
    arr = []
    while num > 0:
        arr.append(num % 2)
        num = num // 2
    while len(arr) < padding:
        arr.append(0)
    #print(arr)
    return arr

### Actual Code #################################
def simple_prog():
    #for i in range(2047):
        #write(v(i), v(255))#[1, 0, 1, 1, 0, 0, 0, 0])
    #    write(v(i), [1, 1, 1, 1, 1, 1, 1, 1])
    #    print(i)

    ## S5 Held Down
    write(v(0+240-128), opcode_gen([1, 1, 0, 0], 1, 0))
    write(v(1+240-128), opcode_gen([0, 1, 1, 0], 1, 0))
    write(v(2+240-128), opcode_gen([0, 0, 1, 1], 1, 0))
    write(v(3+240-128), opcode_gen([1, 0, 0, 1], 1, 0))
    write(v(4+240-128), opcode_gen([1, 0, 0, 1], 1, 0))# Jump to 5
    write(v(5+240-128), opcode_gen([1, 0, 0, 1], 1, 0))# Jump to 5
    write(v(6+240-128), opcode_gen([1, 0, 0, 1], 1, 0))# Jump to 5
    ## Normal Operation
    write(v(0+240), opcode_gen([0, 0, 0, 1], 1, 0))
    write(v(1+240), opcode_gen([0, 0, 1, 0], 1, 0))
    write(v(2+240), opcode_gen([0, 1, 0, 0], 1, 0))
    write(v(3+240), opcode_gen([1, 0, 0, 0], 1, 0))
    write(v(4+240), opcode_gen([1, 0, 0, 0], 0, 1)) # Return to 1
    write(v(5+240), opcode_gen([1, 0, 1, 1], 1, 0))
    write(v(6+240), opcode_gen([1, 1, 0, 1], 1, 0))
    write(v(7+240), opcode_gen([0, 0, 0, 0], 0, 1)) # Return to 0

def opcode_gen(data, count=0, jump=1, a=0):
    opcode = data + [
        1 if count else 0,
        0 if jump else 1,
        0,
        0
    ] #En T, CLR, JMP
    #print(opcode)
    return opcode



simple_prog()
#read([0,0,0,0,0,0,0,0,0,0,0])
#rr()
#time.sleep(1)
#print("#"*30)
#read(v(2))
