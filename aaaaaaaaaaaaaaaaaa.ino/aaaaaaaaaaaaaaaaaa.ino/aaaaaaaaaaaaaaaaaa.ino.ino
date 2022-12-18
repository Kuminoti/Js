#include <WiFi.h>


/*ESP-32: Pulsweitenmodulations gesteuerte Analogwert ausgabe
  zur drahtlosen Ansteuerrung und Regelung von elektrischen
  Verbrauchern wie zum Beispiel Beleuchtung.

 *Dargestellt über ein dynamisches Html-Dokument als GUI
  Steuerung programmiert mit C++, Elemente gebaut mit Html,
  Designt mit CSS und programmiert mit Javascript.

 *Analogwertausgabe über PWM-gesteuerte MOSFETS durch
  Optocoppler. 8-bit auflösung von 0 bis 255 bei 1000 Herz
  Spannung im laststromkreis liegt bei 12 Volt.

 *Nach erfolgreicher Verbindung erreicht man den Webserver
  durch das eingeben der IP-adresse in die URL-zeile im Webbrowser.
  Kompatibel mit allen Systemen Pc und Smartphone.
  (Bei änderrungen des Codes kann es zu Störungen in design und
  kompatibilität kommen) 

  Kompletter Code ist zur Nachvolziebarkeit kommentiert
*/

//Konstanten sind definiert und andere Werte in Arrays


//outputs gespeichert als Array

int pos[] = {1 ,2 ,3 ,4 ,5 ,6 };
int output[] = {26, 27};                                      //Digital Outputs
int pwmpin[] = {0, 2, 4, 27 ,28 ,29 };                //Analog Outputs
int ledChannel[] = {0, 1, 2 ,3 ,4 ,5};                    //LED-Channels

//Konstanten Definiert

#define freq 1000                                              //1000 Herz
#define resolution 8                                          //8-bit Pulsweite von 0-255

#define timeoutTime 2000                                //TimeOut

#define ssid "EasyBox-042179"                        //SSID
#define password "AE3Y8yT93x4uHPeT"        //Passwort

WiFiServer server(80);                                     //Serverport 80

String header;
String output26State = "off";
String output27State = "off";
String valueString = String(5);
String valueString2 = String(6);
String valueString3 = String(7);
String valueString4 = String(8); 
String valueString5 = String(9);
String valueString6 = String(10);

unsigned long currentTime = millis();
unsigned long previousTime = 0;


//Sliderpositionen
int pos1 = 0;   
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
int pos6 = 0;

void setup() {

  
  Serial.begin(115200);                                    //Start des seriellen Monitors

  //PWM-Setup
  
  ledcSetup(ledChannel[0], freq, resolution);       //PWM Setup1
  ledcAttachPin(pwmpin[0], ledChannel[0]);        
  
  ledcSetup(ledChannel[1], freq, resolution);     //zuweisung der pwm variablen
  ledcAttachPin(pwmpin[1], ledChannel[1]);      //Pin PWM-Channel und Frequenz

  ledcSetup(ledChannel[2], freq, resolution);
  ledcAttachPin(pwmpin[2], ledChannel[2]);

  ledcSetup(ledChannel[3], freq, resolution);
  ledcAttachPin(pwmpin[3], ledChannel[3]);

  ledcSetup(ledChannel[4], freq, resolution);
  ledcAttachPin(pwmpin[4], ledChannel[4]);

  ledcSetup(ledChannel[5], freq, resolution);
  ledcAttachPin(pwmpin[5], ledChannel[5]);
  
  pinMode(output[0], OUTPUT);                        //Pinzuweisung
  pinMode(output[1], OUTPUT);

  digitalWrite(output[0], LOW);                          //Outputs standartmäßig auf low
  digitalWrite(output[1], LOW);    

  //Verbindung starten
  Serial.print("Verbinden mit ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("Verbindung hergestellt");
  Serial.println("IP-addresse: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  
  WiFiClient client = server.available();   

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";  
                  
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        
        if (c == '\n') {                   
          
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            
            //Ein-und Ausschalten der Pinns
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output[0], HIGH);

            }
             
            else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output[0], LOW);
            }
             
            else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output[1], HIGH);
            }
            
            else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output[1], LOW);
            }

            //Analogwert erzeugung
            if(output26State == "on"){
              if(header.indexOf("GET /?value=")>=0) {
                pos[0] = header.indexOf('=');
                pos[1] = header.indexOf('&');
                valueString = header.substring(pos[0]+1, pos[1]);
                ledcWrite(ledChannel[0],valueString.toInt());
              }
              
              if(header.indexOf("GET /?value2=")>=0) {
                pos[2] = header.indexOf('=');
                pos[3] = header.indexOf('&');
                valueString2 = header.substring(pos[2]+1, pos[3]);
                ledcWrite(ledChannel[1],valueString2.toInt());
              }

               if(header.indexOf("GET /?value3=")>=0) {
                pos[4] = header.indexOf('=');
                pos[5] = header.indexOf('&');
                valueString = header.substring(pos[4]+1, pos[5]);
                ledcWrite(ledChannel[2],valueString.toInt());
              }
            }
            
            //Ausschalten der kompletten Lichterkette im System1
            else if(output26State == "off"){                           
              ledcWrite(ledChannel[0],LOW);
              ledcWrite(ledChannel[1],LOW);
              ledcWrite(ledChannel[2],LOW);
            }

             if(output27State == "on"){
              if(header.indexOf("GET /?value4=")>=0) {
                pos[6] = header.indexOf('=');
                pos[7] = header.indexOf('&');
                valueString = header.substring(pos[6]+1, pos[7]);
                ledcWrite(ledChannel[3],valueString.toInt());
              }
              
              if(header.indexOf("GET /?value5=")>=0) {
                pos[6] = header.indexOf('=');
                pos[7] = header.indexOf('&');
                valueString = header.substring(pos[6]+1, pos[7]);
                ledcWrite(ledChannel[4],valueString.toInt());
              }
              
              if(header.indexOf("GET /?value6=")>=0) {
                pos[8] = header.indexOf('=');
                pos[9] = header.indexOf('&');
                valueString = header.substring(pos[8]+1, pos[9]);
                ledcWrite(ledChannel[4],valueString.toInt());
              }
             }

             else if(output26State == "off"){                         
              ledcWrite(ledChannel[3],LOW);
              ledcWrite(ledChannel[4],LOW);
              ledcWrite(ledChannel[5],LOW);
             }


             
            //Webserver-GUI 

            //HTML Grundstruktur
            client.println("<!DOCTYPE html><html>");
            client.println("<meta charset=\"utf-8\"/>");
            client.println("<meta http-equiv=\"refresh\" content= \"5\">");
            client.println("<head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            //CSS
            
            //Styling Der HTML-Grundstrukturen
            client.println("<style>");            
            client.println("html { font-family: Helvetica;}");
            client.println(".header{text-align: center; background-color: blue; height: 100px;}");
            client.println(" body{ margin: 0 ; background-color: black; align-content: center;}");
            client.println(".uno{display: flex; content-align: center; background-color: white;}");
            client.println(".dva{displey: flex; content-align: center; background-color: white;}");
            client.println(".pp{margin-left: 5%; }");
            client.println(" p{margin-left: 45%;}");

            //Main Divs
             
            //System1
            client.println(".main{align-content: center; margin-top: 0%; margin-left: 5%; margin-right: 5%; border-color: white; border-radius: 20px; border-style: solid;}");
            client.println(".system1{width:60%; border-color: black; border-radius: 100px; background-color: rgb(200,200,200);");
            client.println(" border-style: solid;background-color: white; box-shadow: 21px 28px 58px -5px rgb(1, 1, 176);}");
            client.println(".system1:hover {background-color: #FFFFFF; box-shadow: 8px 29px 49px -3px rgb(0, 0, 160) inset;}");

            //System2
            client.println(".system2{width:60%; margin-top:3%; border-color: black; border-radius: 100px; background-color:rgb(200,200,200);");
            client.println("content-align: center; border-style: solid; background-color: white; box-shadow: 21px 28px 58px -5px rgb(1, 1, 176);}");
            client.println(".system2:hover { box-shadow: 8px 29px 49px -3px rgb(0, 0, 160) inset;border-color: rgb(0, 0, 0); border-style: solid;}");

            //Slider und HTML-Elemente

            //Slider system1
            client.println(".slider {-webkit-appearance: none; background-color: cyan; color: black; border-radius: 8px; margin-left: 40%;}");
            client.println(".slider3{-webkit-appearance: none; background-color: red;  color: black; border-radius: 8px; margin-left: 40%;}");
            client.println(".slide  {-webkit-appearance: none; background-color: green;color: black; border-radius: 8px; margin-left: 40%;}");
            
            client.println("slider:hover {background-color: blue;1  }");
            client.println("slider3:hover{background-color: magenta;}");
            client.println("slide:hover  {background-color: #4CAF50;}");
            
            
            //Slider system2
            client.println(".slider4{-webkit-appearance: none; background-color: red;   color: black; border-radius: 8px; margin-left: 40%;}");
            client.println(".slider5{-webkit-appearance: none; background-color: cyan;  color: black; border-radius: 8px; margin-left: 40%;}");
            client.println(".slider6{-webkit-appearance: none; background-color: green; color: black; border-radius: 8px; margin-left: 40%;}");

            //Button Styling
            client.println(".button{background-color: #4CAF50; border: none; margin-left: 5%; border-radius: 10px; color: white; height: 30px; width:70px;};");
            client.println(" text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2{background-color: black; color: red; border-radius: 10px; height: 30px; width:70px;}");
            client.println("</style>");

            //Weblogig in Javascript
            //Main Script zur Datenübertragung und Analogwertverarbeitung 
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

            //Zeit-Script
            client.println("<script> function zeit(){let jetzt = new Date(); sekunden = jetzt.getSeconds();");
            client.println("minuten = jetzt.getMinutes(); stunden = jetzt.getHours();");
            client.println("let uhr = document.getElementById(\"time\").innerHTML = stunden + \":\" + minuten + \":\" + sekunden} ");
            client.println("setInterval(zeit,1) </script>");
            client.println("</head>");
            //Ende des Head-bereiches
            
            //Main HTML
            client.println("<body onload=\"zeit()\"><header class=\"header\"<h1>Daniels Web Server</h1> <h2 id=\"time\">Rofl</h2> </header>");
            client.println("<h2 id=\"h\"></h2>");
            client.println("<div class=\"main\">");
           
            client.println("<section class=\"uno\" >");
            client.println("<p class=\"pp\">lololollololololololololol<br>lololol</p>");
            client.println("<div class=\"system1\"> ");
            client.println("<p>Wohnzimmerlicht " + output26State + "</p>");

            //Slider als Input im Browser
            client.println("<input type=\"range\" min=\"0\" max=\"254\"id=\"Slider\" class=\"slide\"onchange=\"servo2(this.value)\" value=\""+valueString2+"\">");
            client.println("<input type=\"range\" min=\"0\" max=\"254\" id=\"servoSlider\" class=\"slider\" onchange=\"servo(this.value)\" value=\""+valueString+"\">");
            client.println("<input type=\"range\" min=\"0\" max=\"254\" id=\"slider3\" class=\"slider3\" onchange=\"servo3(this.value)\" value=\""+valueString3+"\">");

            //Abfrage der Digitalwerte        
            if (output26State == "off") {
            client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            }
            
            else {
              client.println("<p><a href=\"/26/off\"><button class=\"button\">OFF</button></a></p>");
            }

            client.println("<div class=\"kurve\" ></div>");
            client.println("</div>");
            client.println("</section>");

            //Teil2
            client.println("<section class=\"uno\" >");
            client.println("<div class=\"system2\">");
            client.println("<p>Licht2 " + output27State + "</p>");
            
            if (output27State == "off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } 
            
            else {
              client.println("<p><a href=\"/27/off\"><button class=\"button\">OFF</button></a></p>");
            }

            //Slider System2
            client.println("<input type=\"range\" class=\"slider4\" id=\"slider4\" min=\"0\" max=\"254\"   onchange=\"servo4(this.value)\" value=\""+valueString4+"\">");
            client.println("<input type=\"range\" class=\"slider5\" id=\"slider5\" min=\"0\" max=\"254\"   onchange=\"servo5(this.value)\" value=\""+valueString5+"\">");
            client.println("<input type=\"range\" class=\"slider6\" id=\"slider6\" min=\"0\" max=\"254\"    onchange=\"servo6(this.value)\" value=\""+valueString6+"\">");
            client.println("</div>");
            client.println("</section>");

            
            //Script zur verarbeitung der analog Werte über HTTP

            //HTTP Analogwertverarbeitung System1 Slider1
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");            
            client.println("slider.oninput = function() { slider.value = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}");
            client.println("</script>");
            
            
            //HTTP Analogwertverarbeitung System1 Slider2 
            client.println("<script>var slider2 = document.getElementById(\"Slider\");");            
            client.println("slider2.oninput = function() { slider2.value = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo2(pos) { ");
            client.println("$.get(\"/?value2=\" + pos + \"&\"); {Connection: close};}");
            client.println("</script>");


            //HTTP Anallogwertverarbeitung System System1 Slider3
            client.println("<script>var slider3 = document.getElementById(\"slider3\");");            
            client.println("slider3.oninput = function() { slider3.value = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo3(pos) { ");
            client.println("$.get(\"/?value3=\" + pos + \"&\"); {Connection: close};}");
            client.println("</script>");
            client.println("</div></body></html>"); 


            //System 2


            //HTTP Anallogwertverarbeitung System System2 Slider1
            client.println("<script>var slider4 = document.getElementById(\"slider4\");");            
            client.println("slider4.oninput = function() { slider4.value = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo4(pos) { ");
            client.println("$.get(\"/?value4=\" + pos + \"&\"); {Connection: close};}");
            client.println("</script>");


            //HTTP Anallogwertverarbeitung System System2 Slider2
            client.println("<script>var slider5 = document.getElementById(\"slider5\");");            
            client.println("slider5.oninput = function() { slider5.value = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo6(pos) { ");
            client.println("$.get(\"/?value5=\" + pos + \"&\"); {Connection: close};}");
            client.println("</script>");


            //HTTP Anallogwertverarbeitung System System2 Slider3
            client.println("<script>var slider6 = document.getElementById(\"slider6\");");            
            client.println("slider6.oninput = function() { slider6.value = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo6(pos) { ");
            client.println("$.get(\"/?value6=\" + pos + \"&\"); {Connection: close};}");
            client.println("</script>");

            client.println();
            //HTML ende
            break;
            //Webseiten ende

          } 
          
          else { 
            currentLine = "";
          }
        } 
        
        else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";
                        
    client.stop();
    Serial.println("Verbindung getrennt.");
    Serial.println("");
    //Server disconnected
  }
}
