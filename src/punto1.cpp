#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

//configuracion de red
const char* ssid="CESJT";
const char* password="itisjtsmg";

//configuracion del servidor y hardware
WebServer server(80); //puerto 80 el estandar para HTTP

//pines y variables
int leds[5]={1,2,3,4,5};
int animacion1[3][5]={
  {1,1,1,1,1},
  {1,0,1,0,1},
  {0,1,0,1,0}
};
int animacion2[3][5]={
  {1,1,1,1,1},
  {0,0,0,0,0},
  {1,0,0,0,1}
};
int animacion=0;

//pagina web, se guarda en la memoria de programa (PROGMEM) para ahorrar RAM
const char pagina_template[] PROGMEM=R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <title> Pagina Web</title>
    <style>
        body{text-align: center;}
        .btn{padding: 15px;display: block;}
    </style>
</head>
<body>
    <h1 class="titulo">Leds</h1>
    <a href="/animacion1" class="btn">Animacion 1</a>
    <a href="/animacion2" class="btn">Animacion 2</a>
    <a href="/apagar" class="btn">Apagar</a>
</body>
</html>
)rawliteral";


void reset(){
  for(int i=0;i<5;i++){
    digitalWrite(leds[i], LOW);
  }
}


void setup() {
  Serial.begin(115200);
  for(int i=0;i<5;i++){
    pinMode(leds[i], OUTPUT);
  }


  //conexion red wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());

  server.on("/", []() { 
    server.send(200, "text/html", pagina_template); 
  });



  server.on("/animacion1", []() {
    // lo que haga el botón
    reset();
    animacion=1;

    
    server.sendHeader("Location", "/");
    server.send(302);
  });

  server.on("/animacion2", []() {
    // lo que haga el botón
    reset();
    animacion=2;

    
    server.sendHeader("Location", "/");
    server.send(302);
  });

  server.on("/apagar", []() {
    // lo que haga el botón
    animacion=0;
    reset();

    
    server.sendHeader("Location", "/");
    server.send(302);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  if(animacion==1){
    for(int i=0;i<3;i++){
      for(int j=0;j<5;j++){
        digitalWrite(leds[j], animacion1[i][j]);
      }
    }
  }
  if(animacion==2){
    for(int i=0;i<3;i++){
      for(int j=0;j<5;j++){
        digitalWrite(leds[j], animacion2[i][j]);
      }
    }
  }
  //se podria usar  millis usando el void loop en si en vez del for 
  //si se quiere un resultado mas realista
}



