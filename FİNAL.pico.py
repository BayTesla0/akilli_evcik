from machine import Pin, Timer
import time

pin21 = Pin(21, Pin.OUT)
pin21.value(0)
pin22 = Pin(14, Pin.IN)
pin23 = Pin(15, Pin.IN)
pin24 = Pin(16, Pin.IN)
pin25 = Pin(17, Pin.IN)
pin22.value(0)
pin23.value(0)
pin24.value(0)
pin25.value(0)


gp18 = Pin(18, Pin.IN, Pin.PULL_DOWN)  # buton girişi
gp19 = Pin(19, Pin.OUT)  # Röle su
gp20 = Pin(20, Pin.IN, Pin.PULL_DOWN)  # buton girişi
gp22 = Pin(22, Pin.IN, Pin.PULL_DOWN)  # çalışma saaati
gp18.value(0)
gp20.value(0)
gp22.value(0)
analog0 = machine.ADC(26)  # GPIO 26
analog1 = machine.ADC(27)  # GPIO 27
analog2 = machine.ADC(28)  # GPIO 28

tetiklendik = [0] * 3
mevcut = [0, 0, 0]
sure = 0

timer = Timer()  # su pompası için


releler = [machine.Pin(i, machine.Pin.OUT) for i in range(0, 7)]
anahtarlar = [machine.Pin(i, machine.Pin.IN, machine.Pin.PULL_UP) for i in range(7, 14)]
durumlar = [False] * 7
releler=[0,0,0,0,0,0,0]
anahtarlar=[0,0,0,0,0,0,0]

def skalala(deger):
    return int(deger / 65535 * 100)


def read_pins():
    bits = [pin22.value(), pin23.value(), pin24.value(), pin25.value()]
    num = 0
    for bit in bits:
        num = (num << 1) | bit
    return num


  def kontrol_et_tum_pinler():
    for i, anahtar in enumerate(anahtarlar):
        if not anahtar.value():
            durumlar[i] = not durumlar[i]
            releler[i].value(durumlar[i])
            print(f"{7 + i}. pin tetiklendi. Rele {i} durumu: {durumlar[i]}")



def baslangic():
    global ilk0, ilk1, ilk2
    ilk0 = skalala(analog0.read_u16())
    ilk1 = skalala(analog1.read_u16())
    ilk2 = skalala(analog2.read_u16())


def karsilastir():
    global tetiklendik
    deger0 = skalala(analog0.read_u16())
    if deger0 < ilk0 - 10:
        tetiklendik[0] = 1
    deger1 = skalala(analog1.read_u16())
    if deger1 < ilk1-10:
        tetiklendik[1] = 1
    deger2 = skalala(analog2.read_u16())
    if deger2 < ilk2-10:
        tetiklendik[2] = 1


def gp18_handler(pin):
    global sure
    if pin.value():
        timer.init(freq=1000, mode=Timer.PERIODIC, callback=lambda t: None)
        gp19.value(1)
    else:
        timer.deinit()
        gp19.value(0)
        sure = timer.value()
        timer.value(0)


def gp20_handler(pin):
    global sure
    if pin.value():
        gp19.value(1)
        time.sleep_ms(sure)
        gp19.value(0)

def kontrol():
    global tetiklendik
    global mevcut
    if tetiklendik[0] == 1:
        if tetiklendik[1] == 0 and tetiklendik[2] == 0:
            pass
        else:
            start = time.time()
            while True:
                karsilastir()
                if tetiklendik[1] == 1 or tetiklendik[2] == 1:
                    if tetiklendik[1] == 1:
                        mevcut[0] += 1
                        if gp22.value() == 1:
                            releler[0].value(1)
                            break
                    if tetiklendik[2] == 1:
                        releler[1].value(1)
                        mevcut[1] += 1
                    tetiklendik = [0] * 3
                    break
                if time.time() - start >= 3:
                    tetiklendik = [0] * 3
                    break
    elif tetiklendik[1] == 1 or tetiklendik[2] == 1:
        ilk = 1 if tetiklendik[1] == 1 else 2
        start = time.time()
        while True:
            karsilastir()
            if tetiklendik[0] == 1:
                if ilk == 2:
                    mevcut[0] -= 1
                    if mevcut[0] == 0:
                        releler[0].value(0)
                if ilk == 3:
                    mevcut[1] -= 1
                    if mevcut[1] == 0:
                        releler[1].value(0)
                tetiklendik = [0] * 3
                break
            if time.time() - start >= 3:
                tetiklendik = [0] * 3
                break


def set_pins(num):
    if num == 0b0000:
        pin21.value(0)
        return
    elif num == 0b1001:
        releler[0].value(1)
        pin21.value(1)
    elif num == 0b1010:
        releler[1].value(1)
        pin21.value(1)
    elif num == 0b1011:
        releler[2].value(1)
        pin21.value(1)
    elif num == 0b1100:
        releler[3].value(1)
        pin21.value(1)
    elif num == 0b1101:
        releler[4].value(1)
        pin21.value(1)
    elif num == 0b1110:
        releler[5].value(1)
        pin21.value(1)
    elif num == 0b1111:
        releler[6].value(1)
        pin21.value(1)
    elif num == 0b0001:
        releler[0].value(0)
        pin21.value(1)
    elif num == 0b0010:
        releler[1].value(0)
        pin21.value(1)
    elif num == 0b0011:
        releler[2].value(0)
        pin21.value(1)
    elif num == 0b0100:
        releler[3].value(0)
        pin21.value(1)
    elif num == 0b0101:
        releler[4].value(0)
        pin21.value(1)
    elif num == 0b0110:
        releler[5].value(0)
        pin21.value(1)
    elif num == 0b0111:
        releler[6].value(0)
        pin21.value(1)

def pin_durumlarini_yazdir():
    # Tüm pinleri tanımla
    pinler = [Pin(i, Pin.IN) for i in range(28)]

    while True:
        # Tüm pinlerin durumunu yazdır
        for pin_numarası, pin in enumerate(pinler):
            print("Pin {}: {}".format(pin_numarası, pin.value()))

        # 5 saniye bekle
        time.sleep(5)
        
        
while True:
    pin_durumlarini_yazdir()
    baslangic()
    gp18.irq(handler=gp18_handler, trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING)
    gp20.irq(handler=gp20_handler, trigger=Pin.IRQ_RISING)
    kontrol()
    kontrol_et_tum_pinler()
    karsilastir()
    num = read_pins()
    set_pins(num)



