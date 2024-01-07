import machine
import utime

# Röle çıkışları için pinler
relay_pins = [0, 1, 2, 3, 4, 5]

# Giriş butonları için pinler
button_pins = [6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]  # 5 yeni giriş eklendi

# Butonların ilk durumları
button_states = [machine.Pin(pin, machine.Pin.IN).value() for pin in button_pins]

# Analog pinler için tanımlamalar
analog_pin_0 = 0
analog_pin_1 = 1
analog_pin_2 = 2

# Röle çıkışları için pinler
relay_0_pin = 0
relay_1_pin = 1
relay_2_pin = 2
relay_3_pin = 3

relay_5_pin = 5

# Değişkenler
a1 = 0
a2 = 0
salon = 0
banyo = 0

# Analog pin değerini 0 ile 100 arasına hapseden fonksiyon
def normalize_value(value):
    return max(0, min(100, int((value / 1023) * 100)))

# Yeni eklenen girişlere göre hareket eden fonksiyon
def control_new_inputs():
    # İlk giriş 0 ise fonksiyon çalışmasın
    if machine.Pin(button_pins[6], machine.Pin.IN).value() == 0:
        return

    # 7. pin 1 ise 6. pin durumuna göre işlemleri gerçekleştir
    if machine.Pin(button_pins[7], machine.Pin.IN).value() == 1:
        # 6. pin 0 ise röleleri kapatma işlemi yap
        if machine.Pin(button_pins[7], machine.Pin.IN).value() == 0:
            for i in range(3):
                machine.Pin(relay_pins[i], machine.Pin.OUT).value(1)  # Röleleri kapat
        # 6. pin 1 ise belirli röleleri açma işlemi yap
        else:
            binary_state = ''.join(map(str, [machine.Pin(pin, machine.Pin.IN).value() for pin in button_pins[8:11]]))
            for i in range(3):
                if binary_state == bin(i)[2:].zfill(3):
                    machine.Pin(relay_pins[i], machine.Pin.OUT).value(0)  # İlgili röleyi aç

# Ana döngü
while True:
    # Yeni girişlere göre hareket et
    control_new_inputs()

    # Analog pin 0 değerini al
    a0_value = machine.ADC(analog_pin_0).read_u16()
    normalized_a0 = normalize_value(a0_value)

    # Eğer 10'luk bir değişiklik varsa, a1 ve a2'yi 2 saniye boyunca sürekli oku
    if normalized_a0 % 10 == 0:
        for _ in range(20):
            a1_value = machine.ADC(analog_pin_1).read_u16()
            a2_value = machine.ADC(analog_pin_2).read_u16()
            a1 = normalize_value(a1_value)
            a2 = normalize_value(a2_value)
            utime.sleep_ms(100)

    # Eğer 10'luk bir değişiklik varsa ve röle 0 kapalıysa, salon değerini arttır
    if normalized_a0 % 10 == 0 and machine.Pin(relay_0_pin, machine.Pin.OUT).value() == 0:
        salon += 1

    # Eğer 10'luk bir değişiklik varsa ve röle 2 kapalıysa, banyo değerini arttır
    if normalized_a0 % 10 == 0 and machine.Pin(relay_2_pin, machine.Pin.OUT).value() == 0:
        banyo += 1

    # Eğer a1 veya a2 tetiklendi ise, a0'u 2 saniye boyunca bekleyip salon veya banyo değerini azalt
    if a1 % 10 == 0 or a2 % 10 == 0:
        utime.sleep(2)
        a0_value_after_trigger = machine.ADC(analog_pin_0).read_u16()
        normalized_a0_after_trigger = normalize_value(a0_value_after_trigger)

        if a1 % 10 == 0:
            salon -= 1
            if salon == 0:
                machine.Pin(relay_0_pin, machine.Pin.OUT).value(1)  # Röle 0'ı kapat

        if a2 % 10 == 0:
            banyo -= 1
            if banyo == 0:
                machine.Pin(relay_2_pin, machine.Pin.OUT).value(1)  # Röle 2'yi kapat

    # Diğer işlemler veya değerleri kullanabilirsiniz
    # ...

    # Bekleme süresi
    utime.sleep(0.1)


