const int piezo = 0;          // Buzzer pini
const int buton = 2;        // Buton Pini
const int dcMotor = 3;            // Motor pini
const int dcMotor2 = 6;           // Motor pini
const int KLED = 4;               //  Kırmızı LED
const int YLED = 5;             //  Yeşil LED
const int hassasiyet = 3;            // Sinyal hassasiyeti
const int hataPayi = 25;         // Tiklamalar icin hata payi
const int ortalamahataPayi = 15;  // Tiklamalar icin ortalama hata payi
const int vurusAraligi = 150;      // Yenisini dinlemek için beklediğimiz süre
const int beklemeSuresi = 650;       // Motor hareketleri için bekleme aralığı
const int maksTiklama = 20;       // Dinlenicek maksimum tıklama
const int kontrolSuresi = 1200;     // Tıklamaların bittiğini kesinleştirmek için beklenen süre

int varsayilanKod[maksTiklama] = 
{50, 25, 25, 50, 100, 50, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // varsayilan kayit
int ritimKaydi[maksTiklama];   // Çalınan ritimin tutulduğu dizi
int piezoValue = 0;           // son tıklamadan sonra  sensöru sıfırlama
int butonBasili = false;   // Kaydetme bitme durumu
void setup() {
  pinMode(dcMotor, OUTPUT);          //Motor +Yönü(3)
  pinMode(dcMotor2, OUTPUT);         //Motor -Yönü(6)
  pinMode(KLED, OUTPUT);             //Kırmızı LED(4)
  pinMode(YLED, OUTPUT);           //Yeşil LED(5)
  pinMode(buton, INPUT);       //Buton(2)  
  Serial.begin(9600);               	 //Serial monitörden çıktı portumuz
  Serial.println("Program Başladı");   // Serial monitöre yazdırma  
  digitalWrite(YLED, HIGH);      // Program çalışmaya başlayıncakontrol için yeşil LED'i yakma
}

void loop() {
  piezoValue = analogRead(piezo); // Piezo Diskten analog değeri okuma  
  if (digitalRead(buton)==HIGH){      // Butonu basılımı sorgusu
    butonBasili = true;          
    digitalWrite(KLED, HIGH);               // Kırmızı LED'i yakma
  } else {
    butonBasili = false;             
    digitalWrite(KLED, LOW);                // Kırmızı LED'i söndürme
  }  
  if (piezoValue >=hassasiyet){          // Hassasiyet kontrolü
    calinanRitim();                    // Çalınan ritimi dinleme fonksiyonu     
  }
} 


void calinanRitim(){
  Serial.println("tıklama başladı"); 
  int i = 0;
  for (i=0;i<maksTiklama;i++){          //Kaç tıklama yapıldığının sayısını bulma
  }  
  int tiklamaSayisi=0;         			
  int zamanTut=millis();                 //millis() fonksiyonu Arduino çalışmaya başlayınca milisaniye saymaya başlar.
  int suan=millis();
  
  digitalWrite(YLED, LOW); 
  if (butonBasili==true){
     digitalWrite(KLED, LOW);                        
  }
  delay(vurusAraligi);                 	       
  digitalWrite(YLED, HIGH);  
  if (butonBasili==true){
     digitalWrite(KLED, HIGH);                        
  }
  do {
    //tıklamaların okunduğu bölüm 
    piezoValue = analogRead(piezo);
    if (piezoValue >=hassasiyet){                   
      Serial.println("tık.");
      suan=millis();
      ritimKaydi[tiklamaSayisi] = suan-zamanTut;  
      tiklamaSayisi ++;                             
      zamanTut=suan;          
      digitalWrite(YLED, LOW);  
      if (butonBasili==true){
        digitalWrite(KLED, LOW);                       /
      }
      delay(vurusAraligi);                              
      digitalWrite(YLED, HIGH);
      if (butonBasili==true){
        digitalWrite(KLED, HIGH);                         
      }
    }
    suan=millis();
    
    //süre veya maksimum tıklama kontrolü
  } while ((suan-zamanTut < kontrolSuresi) &&  (tiklamaSayisi < maksTiklama));
  
  
  //Tıklama kaydedildi kaydedilen tıklama şeklinde görsel olarak ledleri yakarak kontrol etme
  if (butonBasili==false){             
    if (vurusUygunlugu() == true){
      kapiAcma(); 
    } else {
      Serial.println("ritim başarısız oldu.");
      digitalWrite(YLED, LOW);  		// eşleşme olmadığı için kırmızı  led ile bildirim veriyoruz
      for (i=0;i<4;i++){					
        digitalWrite(KLED, HIGH);
        delay(100);
        digitalWrite(KLED, LOW);
        delay(100);
      }
      digitalWrite(YLED, HIGH);
    }
  } else { // kaydetme devam ediyosa
    vurusUygunlugu();
    Serial.println("yeni ritim kaydedildi.");
    digitalWrite(KLED, LOW);
    digitalWrite(YLED, HIGH);
    for (i=0;i<3;i++){
      delay(100);
      digitalWrite(KLED, HIGH);
      digitalWrite(YLED, LOW);
      delay(100);
      digitalWrite(KLED, LOW);
      digitalWrite(YLED, HIGH);      
    }
  }
}

// kilit açmak için motor hareketleri
void kapiAcma(){
  Serial.println("kapı açıldı!");
  int i=0;
  
  
  digitalWrite(dcMotor, HIGH);
  digitalWrite(YLED, HIGH);            // tıklama onaylanınca motor çalışırken led yanar
  digitalWrite(dcMotor2, LOW);                  
  delay (600); 
  digitalWrite(dcMotor, LOW);
  delay (1800); 
  // açıldıktan sonrası
  digitalWrite(dcMotor2, HIGH);  
  digitalWrite(dcMotor, LOW); 
  delay (600);
  digitalWrite(dcMotor2, LOW);
   
       
  
  // program durumu için ledler yakıp söndürme
  for (i=0; i < 5; i++){   
      digitalWrite(YLED, LOW);
      delay(100);
      digitalWrite(YLED, HIGH);
      delay(100);
  }   
}

// tıklanan ile kaydedilenin karşılaştırılması
// eşleşirse true eşleşmezse false

boolean vurusUygunlugu(){
  int i=0;
 
  //ilk kontrol toplam tıklama sayısı karşılaştırma
  int tiklamaSayaci = 0;
  int kayitliTiklama = 0;
  int maksTiklamaAraliği = 0;          			
  
  for (i=0;i<maksTiklama;i++){
    if (ritimKaydi[i] > 0){
      tiklamaSayaci++;
    }
    if (varsayilanKod[i] > 0){  					
      kayitliTiklama++;
    }
    
    if (ritimKaydi[i] > maksTiklamaAraliği){ 	
      maksTiklamaAraliği = ritimKaydi[i];
    }
  }
  
  if (butonBasili==true){
      for (i=0;i<maksTiklama;i++){ 
        varsayilanKod[i]= map(ritimKaydi[i],0, maksTiklamaAraliği, 0, 100); 
      }
      // kaydedilen tıklama şeklinde ledleri yakma
      digitalWrite(YLED, LOW);
      digitalWrite(KLED, LOW);
      delay(1000);
      digitalWrite(YLED, HIGH);
      digitalWrite(KLED, HIGH);
      delay(50);
      for (i = 0; i < maksTiklama ; i++){
        digitalWrite(YLED, LOW);
        digitalWrite(KLED, LOW);          
        if (varsayilanKod[i] > 0){                                   
          delay( map(varsayilanKod[i],0, 100, 0, maksTiklamaAraliği)); 
          digitalWrite(YLED, HIGH);
          digitalWrite(KLED, HIGH);
        }
        delay(50);
      }
	  return false;
  }
  
  if (tiklamaSayaci != kayitliTiklama){
    return false; 
  }
  
  //  kaydedilen tıklamalar arasındaki süre açarken biraz daha az hassas olması için hata payıyla kontrol işlemi
  
  int toplamZamanAraliği=0;
  int zamanAraliği=0;
  for (i=0;i<maksTiklama;i++){ 
    ritimKaydi[i]= map(ritimKaydi[i],0, maksTiklamaAraliği, 0, 100);      
    zamanAraliği = abs(ritimKaydi[i]-varsayilanKod[i]);
    if (zamanAraliği > hataPayi){ 
      return false;
    }
    toplamZamanAraliği += zamanAraliği;
  }  
  if (toplamZamanAraliği/kayitliTiklama>ortalamahataPayi){
    return false; 
  }  
  return true;  
}