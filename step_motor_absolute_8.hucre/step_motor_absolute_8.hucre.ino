/*
prg adı: step_motor_absolute
board: arduino uno r3
tarih: 16.12.2016
yazan: Remzi ŞAHİNOĞLU

A4988 driver ile nema 17 step motor kontrolünü yapan program. Bu program 
yan delik kontrol makinesinin operator panelinden seçilen ürüne göre
optik sensorun yüksekliğini ayarlamaya yarıyor. aynı zamanda manuel modda
aşağı yukarı komutunu ve seriladan gelen komutları işletebilir.

nema17 kablo bağlantısı


*/

//----------------------------- IO ATAMALARI -----------------------------//
const int a_pin = 2;                                                      // a pini
const int b_pin = 3;                                                      // b pini
const int c_pin = 4;                                                      // c pini
const int enable = 5;                                                     // enable
const int adim = 6;                                                       // step
const int dir = 7;                                                        // direction
const int yukari = 8;                                                     // yukarı buton
const int asagi = 9;                                                      // aşağı buton
const int referans = 10;                                                  // referans butonu
const int referans_switch = 12;                                           // referans swtich
const int manuel = 11;                                                    // manuel-auto switch
//**** ilk yapilan makinede ref switchi ile man/auto switchi pinleri ters ***** 

//---------------------------- DEĞİŞKENLER -------------------------------//
// referans değeri 36,5mm de oluyor, bu değerden 16T ye değerine 38,5mm ye çıkmak için gerekli adım hesabı
// vida mili hatvesi 1.25mm/rev, step motor adımı 200 adım/rev
// sonuc = ((abs(referans değeri mm - istenilen yükseklik mm)  / vida mili hatvesi mm ) * step motor adım sayısı tam turdaki)
int pozisyon = 0;

int adim_adedi_16T = 550;         // 36,5 >> 38,5mm ye çık (2mm = 320adım)
int adim_adedi_20T = 687;        // 36,5 >> 44mm ye çık (7,5mm = 1500adım)
int adim_adedi_24T = 737;        // 36,5 >> 46mm ye çık (9,5mm = 1900adım)
int adim_adedi_30T_K = 575;       // 36,5 >> 39,5mm ye çık (3mm = 480adım)
int adim_adedi_30T_U = 975;      // 36,5 >> 55,5mm ye çık (19mm = 3040adım)

boolean activate_16T = 0;
boolean activate_20T = 0;
boolean activate_24T = 0;
boolean activate_30T_K = 0;
boolean activate_30T_U = 0;

int adim_hizi = 1;                                                        // motor adim arası gecikme

boolean yukari_yon = 1;
boolean asagi_yon = 0;

String inputString = "";

//---------------------- KONFİGÜRASYON AYARLARI --------------------------//
void setup()
{
  //Serial.begin(9600);
  //delay(500);
  
  pinMode(manuel, INPUT_PULLUP);
  
  pinMode(yukari, INPUT_PULLUP);
  pinMode(asagi, INPUT_PULLUP);
  pinMode(referans, INPUT_PULLUP);
  pinMode(referans_switch, INPUT_PULLUP);

  pinMode(a_pin, INPUT_PULLUP);
  pinMode(b_pin, INPUT_PULLUP);
  pinMode(c_pin, INPUT_PULLUP);
  
  pinMode(dir, OUTPUT);
  pinMode(adim, OUTPUT);
  pinMode(enable, OUTPUT);

}

//---------------------------- ANA PROGRAM -------------------------------//
void loop()
{

  /*
  while (Serial.available())                                        // paketi okuma işlemine başla
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
  }

  if (inputString == "ref")
  {
    inputString = "";   // bufferı temizle

    digitalWrite(enable, LOW);                                        // enable aktif
    digitalWrite(dir, asagi_yon);                                     // asağı yön
    
    while(digitalRead(referans_switch))
    {        
      digitalWrite(adim, HIGH);                                         // step göndermek için
      delay(adim_hizi);                                                 // hızı belirler
      digitalWrite(adim, LOW);
    }
      
    pozisyon = 0;       // pozisyon referans modda
  }

  if (inputString == "16T")
  {
    inputString = "";   // bufferı temizle
    digitalWrite(enable, LOW);                                                // enable aktif
    
    if (adim_adedi_16T - pozisyon  > 0)                                       // yönü belirle
    { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
    
    for(int i = 0; i < abs(adim_adedi_16T - pozisyon); i++)                   // adımı hesapla
    { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }

    pozisyon = adim_adedi_16T;                                                // pozisyonu ayarla

    digitalWrite(enable, HIGH);                                               // enable pasif
  }
  
  if (inputString == "20T")
  {
    inputString = "";   // bufferı temizle
    digitalWrite(enable, LOW);                                                // enable aktif
    
    if (adim_adedi_20T - pozisyon  > 0)                                       // yönü belirle
    { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
    
    for(int i = 0; i < abs(adim_adedi_20T - pozisyon); i++)                   // adımı hesapla
    { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }

    pozisyon = adim_adedi_20T;
    digitalWrite(enable, HIGH);                                               // enable pasif
  }

  if (inputString == "24T")
  {
    inputString = "";   // bufferı temizle
    digitalWrite(enable, LOW);                                                // enable aktif
    
    if (adim_adedi_24T - pozisyon  > 0)                                       // yönü belirle
    { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
    
    for(int i = 0; i < abs(adim_adedi_24T - pozisyon); i++)                   // adımı hesapla
    { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }

    pozisyon = adim_adedi_24T;
    digitalWrite(enable, HIGH);                                               // enable pasif
  }

  if (inputString == "30TU")
  {
    inputString = "";   // bufferı temizle
    digitalWrite(enable, LOW);                                                // enable aktif
    
    if (adim_adedi_30T_U - pozisyon  > 0)                                       // yönü belirle
    { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
    
    for(int i = 0; i < abs(adim_adedi_30T_U - pozisyon); i++)                   // adımı hesapla
    { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }

    pozisyon = adim_adedi_30T_U;
    digitalWrite(enable, HIGH);                                               // enable pasif
  }

  if (inputString == "30TK")
  {
    inputString = "";   // bufferı temizle
    digitalWrite(enable, LOW);                                                // enable aktif
    
    if (adim_adedi_30T_K - pozisyon  > 0)                                       // yönü belirle
    { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
    
    for(int i = 0; i < abs(adim_adedi_30T_K - pozisyon); i++)                   // adımı hesapla
    { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }

    pozisyon = adim_adedi_30T_K;
    digitalWrite(enable, HIGH);                                               // enable pasif
  }

  */
  delay(100);

  if(digitalRead(manuel) == 1)                                            // jumper takılı değil ise manuel mod
  {
    if(digitalRead(yukari) == 0)
    {
      digitalWrite(enable, LOW);                                          // enable aktif
      digitalWrite(dir, yukari_yon);                                      // aşağı
      
      digitalWrite(adim, HIGH);                                           // step göndermek için
      delay(adim_hizi);                                                   // hızı belirler
      digitalWrite(adim, LOW);
    }
    else if(digitalRead(asagi) == 0)
    {
      digitalWrite(enable, LOW);                                          // enable aktif
      digitalWrite(dir, asagi_yon);                                       // yukarı
      
      digitalWrite(adim, HIGH);                                           // step göndermek için
      delay(adim_hizi);                                                   // hızı belirler
      digitalWrite(adim, LOW);
    }
    else if(digitalRead(referans) == 0)                                   // referansa git
    {
      while(digitalRead(referans_switch))
      {
        digitalWrite(enable, LOW);                                        // enable aktif
        digitalWrite(dir, asagi_yon);                                     // yukarı
        
        digitalWrite(adim, HIGH);                                         // step göndermek için
        delay(adim_hizi);                                                 // hızı belirler
        digitalWrite(adim, LOW);
      }
    }
    else
    {
      digitalWrite(enable, HIGH);                                         // enable pasif
    }
  }
  else                                                                    // takılı ise auto mod yap ve a,b,c pinlerini oku
  {
    if(digitalRead(a_pin) == 0 && digitalRead(b_pin) == 0 && digitalRead(c_pin) == 0)     // referans moda git
    {
      delay(100);
      
      digitalWrite(enable, LOW);                                        // enable aktif
      digitalWrite(dir, asagi_yon);                                     // yukarı
      
      while(digitalRead(referans_switch))
      {
        digitalWrite(adim, HIGH);                                         // step göndermek için
        delay(adim_hizi);                                                 // hızı belirler
        digitalWrite(adim, LOW);
      }
      pozisyon = 0;
      digitalWrite(enable, HIGH);                                         // enable pasif

      activate_16T = 1;                                                   // sürme açık delik pozisyonuna gidebilir
      activate_20T = 1;
      activate_24T = 1;
      activate_30T_K = 1;
      activate_30T_U = 1;
    }
    else if(digitalRead(a_pin) == 0 && digitalRead(b_pin) == 0 && digitalRead(c_pin) == 1)     // 16 T deliğine git
    {
      if(activate_16T == 1)                                                 // referanstan sonra bir kere bu komutu çalıştırmak için
      {
        activate_16T = 0;                                                  // sinyal olduğu sürece tekrar tekrar yapmasın diye
        activate_20T = 1;
        activate_24T = 1;
        activate_30T_K = 1;
        activate_30T_U = 1;
        
        digitalWrite(enable, LOW);                                                // enable aktif
    
        if (adim_adedi_16T - pozisyon  > 0)                                       // yönü belirle
        { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
       
        for(int i = 0; i < abs(adim_adedi_16T - pozisyon); i++)                   // adımı hesapla
        { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }
    
        pozisyon = adim_adedi_16T;                                                // pozisyonu ayarla
        
        digitalWrite(enable, HIGH);                                         // enable pasif
      }     
    }
    else if(digitalRead(a_pin) == 0 && digitalRead(b_pin) == 1 && digitalRead(c_pin) == 0)     // 20 T deliğine git
    {
      if(activate_20T == 1)                                                 // referanstan sonra bir kere bu komutu çalıştırmak için
      {
        activate_16T = 1;
        activate_20T = 0;                                                   // sinyal olduğu sürece tekrar tekrar yapmasın diye
        activate_24T = 1;
        activate_30T_K = 1;
        activate_30T_U = 1;
        
        digitalWrite(enable, LOW);                                                // enable aktif
    
        if (adim_adedi_20T - pozisyon  > 0)                                       // yönü belirle
        { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
        
        for(int i = 0; i < abs(adim_adedi_20T - pozisyon); i++)                   // adımı hesapla
        { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }
    
        pozisyon = adim_adedi_20T;
        digitalWrite(enable, HIGH);
        
      }
    }
    else if(digitalRead(a_pin) == 0 && digitalRead(b_pin) == 1 && digitalRead(c_pin) == 1)     // 24 T deliğine git
    {
      if(activate_24T == 1)                                                 // referanstan sonra bir kere bu komutu çalıştırmak için
      {
        activate_16T = 1;
        activate_20T = 1;                                                   // sinyal olduğu sürece tekrar tekrar yapmasın diye
        activate_24T = 0;
        activate_30T_K = 1;
        activate_30T_U = 1;
        
        digitalWrite(enable, LOW);                                                // enable aktif
    
        if (adim_adedi_24T - pozisyon  > 0)                                       // yönü belirle
        { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
        
        for(int i = 0; i < abs(adim_adedi_24T - pozisyon); i++)                   // adımı hesapla
        { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }
    
        pozisyon = adim_adedi_24T;
        digitalWrite(enable, HIGH);                                               // enable pasif
      }
    }
    else if(digitalRead(a_pin) == 1 && digitalRead(b_pin) == 0 && digitalRead(c_pin) == 0)     // 30 T kısa deliğine git
    {
      if(activate_30T_K == 1)                                                 // referanstan sonra bir kere bu komutu çalıştırmak için
      {
        activate_16T = 1;
        activate_20T = 1;                                                   // sinyal olduğu sürece tekrar tekrar yapmasın diye
        activate_24T = 1;
        activate_30T_K = 0;
        activate_30T_U = 1;
        
        digitalWrite(enable, LOW);                                                // enable aktif
    
        if (adim_adedi_30T_K - pozisyon  > 0)                                       // yönü belirle
        { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
        
        for(int i = 0; i < abs(adim_adedi_30T_K - pozisyon); i++)                   // adımı hesapla
        { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }
    
        pozisyon = adim_adedi_30T_K;
        digitalWrite(enable, HIGH);                                               // enable pasif
      }
    }
    else if(digitalRead(a_pin) == 1 && digitalRead(b_pin) == 0 && digitalRead(c_pin) == 1)     // 30 T uzun deliğine git
    {
      if(activate_30T_U == 1)                                                 // referanstan sonra bir kere bu komutu çalıştırmak için
      {
        activate_16T = 1;
        activate_20T = 1;                                                   // sinyal olduğu sürece tekrar tekrar yapmasın diye
        activate_24T = 1;
        activate_30T_K = 1;
        activate_30T_U = 0;
        
        digitalWrite(enable, LOW);                                                // enable aktif
    
        if (adim_adedi_30T_U - pozisyon  > 0)                                       // yönü belirle
        { digitalWrite(dir, yukari_yon); }else{ digitalWrite(dir, asagi_yon); }
        
        for(int i = 0; i < abs(adim_adedi_30T_U - pozisyon); i++)                   // adımı hesapla
        { digitalWrite(adim, HIGH); delay(adim_hizi); digitalWrite(adim, LOW); }
    
        pozisyon = adim_adedi_30T_U;
        digitalWrite(enable, HIGH);                                               // enable pasif
      }
    }
    else
    {
      digitalWrite(enable, HIGH);                                         // enable pasif
    }
  }




}












