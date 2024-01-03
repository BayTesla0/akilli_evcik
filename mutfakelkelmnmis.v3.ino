//ffssrrlarııım
#define salon_banyo_giris A0
#define salon_cikis A1
#define y1_y2_giris A2
#define y1_cikis A3
#define y2_cikis A4
#define banyo_cikis A5
#define mutfak A6
//lambalaaar
#define salon 1
#define banyo 2
#define yt1 3
#define yt2 4
#define mutfaklmb 5
#define kooridor 6

int anormal[6];
int tetiklendik[6];
int mevcut[5];
int normal[7];

bool tetiklenen_var_gir;
bool tetiklenen_var_cik;
bool mutfak_tetiklendi;


void setup() {

normal[0]=analogRead(salon_banyo_giris);
normal[1]=analogRead(y1_y2_giris);
normal[2]=analogRead(salon_cikis);
normal[3]=analogRead(y1_cikis);
normal[4]=analogRead(y2_cikis);
normal[5]=analogRead(banyo_cikis);
normal[6]=analogRead(mutfak);



}

void mutfakyakson(){
if(normal[7] >= analogRead(mutfak)+20){
  mutfak_tetiklendi=1;
  //mutfağın ışığının yak
}

}
void superkontrol(){
    for(int i=0;i<150;i++){//buraları kartı aldıktan sonra test et

anormal[0]=analogRead(salon_banyo_giris);
anormal[1]=analogRead(y1_y2_giris);
}
void kontrol_giris();

  for(int i=0;i<150;i++){//buraları test et


anormal[2]=analogRead(salon_cikis);
anormal[3]=analogRead(y1_cikis);
anormal[4]=analogRead(y2_cikis);
anormal[5]=analogRead(banyo_cikis);

}
void kontrol_cikis();

}
void loop() {
  // put your main code here, to run repeatedly:
  void superkontrol();

if(tetiklenen_var_gir==1){
  
  void yakma();

  
}else if(tetiklenen_var_cik==1){
  
  void sondurme();
  
}

for (int i =0;i<7;i++){
  tetiklendik[i]=0;
}
}

void kontrol_giris(){
for(int i=0;i<2;i++){
  if(normal[i]>=anormal[i]+50){
    tetiklendik[i]=1;
    
    tetiklenen_var_gir=1;
  }
}
}


void kontrol_cikis(){
 
for(int i=2;i<6;i++){
  if(normal[i]>=anormal[i]+50){
    tetiklendik[i]=1;
        tetiklenen_var_cik=1;

  }
}



}
void yakma(){
if(tetiklendik[0]==1){ 
   
     if(tetiklendik[2]==1){
     //salon ışığını yak
     //0 salon
     mevcut[0]++; 
     }else if(tetiklendik[5]==1){ 
      //banyonun ışığını yak 
      //1 banyo
      mevcut[1]++;}


}else if(tetiklendik[1]==1){
   if(tetiklendik[3]==1){
   //yatak odası 1 in ışığını aç
   //2 yatak odası 1
   mevcut[2]++;

    }else if(tetiklendik[4]==1){
     //yatak odası 2 nini ışığını aç
     //3 yatak odası 2
     mevcut[3]++;

}
}
tetiklenen_var_cik=0;


}

void sondurme(){
  if(tetiklendik[2]==1){
  void superkontrol();

     if(tetiklenen_var_gir==1){
      
      mevcut[0]--;
     if(mevcut[0]==0){
     //salon ışığının kapat

     }
  
     }
   }else if(tetiklendik[3]==1){
      void superkontrol();

    if(tetiklenen_var_gir==1){
      
      mevcut[1]--;
     if(mevcut[1]==0){
     //banyo ışığının kapat
     
     }
   }

}else if(tetiklendik[3]==1){
      void superkontrol();

    if(tetiklenen_var_gir==1){
      
      mevcut[2]--;
     if(mevcut[2]==0){
     //yt1  ışığının kapat
     
     }
   }

}else if(tetiklendik[4]==1){
      void superkontrol();

    if(tetiklenen_var_gir==1){
      
      mevcut[3]--;
     if(mevcut[3]==0){
     //yt2  ışığının kapat
     
     }
   }

}
tetiklenen_var_gir=0;
}
