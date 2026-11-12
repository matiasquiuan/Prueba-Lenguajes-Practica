#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid="CESJT";
const char* password="itisjtsmg";
WebServer server(80);
int ventilador=1;
int modo=1;

const char pagina_template[]=R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <title> Pagina Web</title>
    <style>
        body{text-align: center;background-color: #ecf0f1;}
        .titulo{font-family:'Arial Black';color: #2c3e50;}
        .contenedor{display: flex;flex-direction: column;align-items: center; }
        .btn{display: block;width: 180px;height: 60px;border:1px solid black;border-radius: 15px;}
        .off{background-color: #c0392b;}
        .low{background-color: #2980b9;}
        .medium{background-color: #27ae60;}
        .high{background-color: #f39c12;}
    </style>
</head>
<body>
    <h1 class="titulo">CONTROL DE VENTILADOR</h1>
    <span>Velocidad Actual: <b>XXXX</b></span>
    <div class="contenedor">
        <a href="/off" class="btn off">OFF</a>
        <a href="/low" class="btn low">LOW</a>
        <a href="/medium" class="btn medium">MEDIUM</a>
        <a href="/high" class="btn high">HIGH</a>
    </div>
    
</body>
</html>
)rawliteral";
void cambiar_txt(){//funcion que se va a llamar cuando se quiera cambiar el texto 
    String pagina=pagina_template;//hacemos una copia de la pagina original
    String modo_txt;
    if(modo==1)modo_txt="APAGADO";//dependiendo del modo cambiamos lo que va a decir en el span
    if(modo==2)modo_txt="BAJA";
    if(modo==3)modo_txt="MEDIA";
    if(modo==4)modo_txt="ALTA";

    pagina.replace("XXXX", modo_txt);//con esta funcion reemplazamos el XXXX de la pagina por el texto que representara la velocidad actual 
}
void setup(){

    //------------------
    Serial.begin(115200);
    WiFi.begin(ssid,password);
    while(WiFi.status()!=WL_CONNECTED) delay(500);
    Serial.println(WiFi.localIP());
    server.on("/",[](){
        server.send(200,"text/html", pagina_template);
    });
    //------------------

    server.on("/off",[](){//para off hay que apagar el ventilador
        modo=1;
        cambiar_txt();
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.on("/low",[](){//para low hay que poner una velocidad baja
        modo=2;
        cambiar_txt();
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.on("/medium",[](){//para medium hay que poner una velocidad media
        modo=3;
        cambiar_txt();
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.on("/high",[](){//para high hay que poner una velocidad alta
        modo=4;
        cambiar_txt();
        server.sendHeader("Location","/");
        server.send(302);
    });
    server.begin();
}

void loop(){
    server.handleClient();
    if(modo==1) analogWrite(ventilador, 0);
    if(modo==2) analogWrite(ventilador, 64);
    if(modo==3) analogWrite(ventilador, 127);
    if(modo==4) analogWrite(ventilador, 255);
}