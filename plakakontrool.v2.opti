#define salon_banyo_giris A0
#define salon_cikis A1
#define y1_y2_giris A2
#define y1_cikis A3
#define y2_cikis A4
#define banyo_cikis A5

int anormal[6];
bool tetiklenen_var_gir;
bool tetiklenen_var_cik;

int tetiklendik[6];
int mevcut[4];
int normal[6];

void okuNormalDegerleri() {
  normal[0] = analogRead(salon_banyo_giris);
  normal[1] = analogRead(y1_y2_giris);
  normal[2] = analogRead(salon_cikis);
  normal[3] = analogRead(y1_cikis);
  normal[4] = analogRead(y2_cikis);
  normal[5] = analogRead(banyo_cikis);
}

void superkontrol() {
  for (int i = 0; i < 150; i++) {
    anormal[0] = analogRead(salon_banyo_giris);
    anormal[1] = analogRead(y1_y2_giris);
  }

  kontrol_giris();

  for (int i = 0; i < 150; i++) {
    anormal[2] = analogRead(salon_cikis);
    anormal[3] = analogRead(y1_cikis);
    anormal[4] = analogRead(y2_cikis);
    anormal[5] = analogRead(banyo_cikis);
  }

  kontrol_cikis();
}

void kontrol_giris() {
  for (int i = 0; i < 2; i++) {
    if (normal[i] >= anormal[i] + 50) {
      tetiklendik[i] = 1;
      tetiklenen_var_gir = 1;
    }
  }
}

void kontrol_cikis() {
  for (int i = 2; i < 6; i++) {
    if (normal[i] >= anormal[i] + 50) {
      tetiklendik[i] = 1;
      tetiklenen_var_cik = 1;
    }
  }
}

void yakma() {
  if (tetiklendik[0] == 1) {
    if (tetiklendik[2] == 1) {
      mevcut[0]++;
    } else if (tetiklendik[5] == 1) {
      mevcut[1]++;
    }
  } else if (tetiklendik[1] == 1) {
    if (tetiklendik[3] == 1) {
      mevcut[2]++;
    } else if (tetiklendik[4] == 1) {
      mevcut[3]++;
    }
  }
  tetiklenen_var_cik = 0;
}

void sondurme() {
  if (tetiklendik[2] == 1) {
    superkontrol();
    if (tetiklenen_var_gir == 1) {
      mevcut[0]--;
      if (mevcut[0] == 0) {
        // salon ışığını kapat
      }
    }
  } else if (tetiklendik[3] == 1) {
    superkontrol();
    if (tetiklenen_var_gir == 1) {
      mevcut[1]--;
      if (mevcut[1] == 0) {
        // banyo ışığını kapat
      }
    }
  } else if (tetiklendik[4] == 1) {
    superkontrol();
    if (tetiklenen_var_gir == 1) {
      mevcut[2]--;
      if (mevcut[2] == 0) {
        // yatak odası 1 ışığını kapat
      }
    }
  } else if (tetiklendik[5] == 1) {
    superkontrol();
    if (tetiklenen_var_gir == 1) {
      mevcut[3]--;
      if (mevcut[3] == 0) {
        // yatak odası 2 ışığını kapat
      }
    }
  }
  tetiklenen_var_gir = 0;
}

void setup() {
  okuNormalDegerleri();
}

void loop() {
  superkontrol();

  if (tetiklenen_var_gir == 1) {
    yakma();
  } else if (tetiklenen_var_cik == 1) {
    sondurme();
  }

  for (int i = 0; i < 7; i++) {
    tetiklendik[i] = 0;
  }
}
