import machine, time

PIN = {
  "LED": 2,
  
  "A0":  0,

  "D0": 16,
  "D1": 5,
  "D2": 4,
  "D3": 0,
  "D4": 2,
  "D5": 14,
  "D6": 12,
  "D7": 13,
  "D8": 15,

  "TX": 1,
  "RX": 3
}

LED = machine.Pin(PIN["LED"], machine.Pin.OUT)

A0 = machine.ADC(PIN["A0"])

D2 = machine.Pin(PIN["D2"], machine.Pin.IN, machine.Pin.PULL_UP)
D5 = machine.Pin(PIN["D5"], machine.Pin.OUT)

def Blink_LED(n, s):
    for i in range(0, n):
        LED.value(0)
        time.sleep(s)
        LED.value(1)
        time.sleep(s)
        