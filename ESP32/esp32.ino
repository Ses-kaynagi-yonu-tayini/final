/* EKRAN kutuphaneleri*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
/* */

/* SERVER kutuphaneleri*/
#include <WiFi.h>
#include <WebServer.h>
/**/

// ag adi ver parolasi
const char* ssid = "BT";
const char* password =  "BatuHanBatuHan";
static int angle = 0;

WebServer server(80);                
String SendHTML(float Yonstat);
float Yon = 0;   // stm den gelen yon verisinin tutulacagi degisken

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(/*SCREEN_WIDTH*/128, /*SCREEN_HEIGHT*/64, &Wire, OLED_RESET);

#define NUMFLAKES     10 
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

void setup() {
  Serial.begin(115200);
  delay(100);

  // ekran baslatilir.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  /* --------------------------------*/ 
  // giris ekrani, universite adi vb. bilgiler basilir.
  display.setTextSize(2);            
  display.setTextColor(WHITE);       
  display.println(F(" GEBZE TU"));
  display.println(F(" Bil. Muh."));
  display.println();
  display.setTextColor(BLACK, WHITE); 
  display.println(F("   2020    "));
  
  display.display();
  
  delay(5000); 
 
  /* --------------------------------*/ 
  // baglanmaya calisilan ag adi ekrana basilir.
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);             
  display.println(F("SSID : "));
  display.println(" ");
  display.setTextSize(2);  
  display.println(ssid);

  display.display();
  delay(500);  
  
  Serial.println("Connecting to ");
  Serial.println(ssid);
 
  /* --------------------------------*/ 
  // wifi balatilir ve acilen server ip ekrana basilir
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(2000);
  Serial.print(".");
  }
    
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);             
  display.println(F("WiFi'a baglandi"));
  display.println("");
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();
  delay(2000);


  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  /* --------------------------------*/ 
  // server kurulur.
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");

  /* ----------------------- */

}
void loop() {
  
  server.handleClient();
  
}

void uartOkuyan(){

  while(1){
   if (Serial.available()) {
     char c = Serial.read();
     if (c == '\n') {
            Yon = angle; 
            // debeg
            //Serial.println(Yon);
            angle = 0;
    } else if ((c >= '0') && (c <= '9')) {
        angle *= 10;
        angle += (c - '0');
    }
   }
   else{
     return;
   }
  }
}

void handle_OnConnect() {

  uartOkuyan();

  // üst bara ip yazilir
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);            
  display.print("IP: ");
  display.println(WiFi.localIP());
  
  // gelen yon degeri ekrana basilir
  display.setCursor(10,34);
  display.setTextSize(2);
  display.print((int) Yon);
  
  // degerin etrafina cerceve cizilir
  display.drawRect( 5, 26,46,32, WHITE);

  //orijin noktasi halka uzerinde isaretlenir.
  display.drawPixel(90, 14, WHITE);
  display.drawPixel(90, 15, WHITE);
  display.drawPixel(89, 14, WHITE);
  display.drawPixel(89, 15, WHITE);
  display.drawPixel(91, 15, WHITE);
  display.drawPixel(91, 14, WHITE);
  display.drawPixel(90, 13, WHITE);

  // halka çizilir
  display.fillCircle(90, 40, 24, WHITE);
 
  // Gelen açı bilgisine göre daire üzerinde ok cizilir.
   if((Yon >=0 && Yon <=92)){
       display.drawLine(90, 40,90 + int( Yon /3.9),16 + int( Yon /3.9),INVERSE);
   }
   else if(Yon >92 && Yon <=182){
       
       display.drawLine(90, 40,90 + int((180-Yon)/3.9),40+int( (Yon-90) /3.9),INVERSE);
   }
   else if(Yon >182 && Yon <=272){
        
       display.drawLine(90, 40,90 - int((Yon -180)/3.9),40+int( (270 - Yon) /3.9),INVERSE);
   }
   else if(Yon >272 && Yon <=360){
        
       display.drawLine(90, 40, 90 - int( (360 - Yon) /3.9),16 + int( (360 - Yon) /3.9),INVERSE);
   }
  
  display.display(); // Update screen with each newly-drawn circle

  // yon bilgisi server üzerinden de paylasilir
  server.send(200, "text/html", SendHTML(Yon)); 
  
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Yonstat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>GTU Bil.Muh.</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,1000);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById(\"webpage\").innerHTML =this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>GTÜ Bil.Müh. 2020</h1>\n";

  ptr +="<p>Akustik Algılayıcılar İle Ses Kaynağı Yön Tayini";
  ptr +="</p>";
  ptr +="<p>Prof.Dr. Erkan ZERGEROĞLU";
  ptr +="</p>";
  ptr +="<p>Batuhan TOPALOĞLU / Yusuf BAVAŞ";
  ptr +="</p>";
  ptr +="<p> Yön : ";
  ptr +=(int)Yonstat;
  ptr +="°</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
