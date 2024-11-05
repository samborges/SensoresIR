#include <WiFi.h>                      // Part of ESP32 board libraries
#include <WebServer.h>                 // Part of ESP32 board libraries
#include <WebSocketsServer_Generic.h>  // Installed "WebSockets" library
#include <Adafruit_AMG88xx.h>          // Installed "Adafruit_AMG88xx" library

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
//
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h>  //http://librarymanager/All#SparkFun_VL53L5CX

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData;  // Result data class structure, 1356 byes of RAM

int imageResolution = 0;  //Used to pretty print output
int imageWidth = 0;       //Used to pretty print output


//
WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

char* ssid = "Sensor";
char* password = "123456789";

char webpage[] PROGMEM = R"=====(
<html>
<head>
  <style>
    #thermcam td {
      width: 60;
      height: 60;
    }
    #thermcam {
    border-spacing: 0;
    }
    
  </style>
  <script>
    var Socket;
    var streamframestimer;
    var sizesensor = [8,8];
    var minTsensor = 0.0;
    var maxTsensor = 80.0;
    var minT = 15.0; // initial low range of the sensor (this will be blue on the screen)
    var maxT = 50.0; // initial high range of the sensor (this will be red on the screen)
    var currentFrame = [];
    var minTframe = maxTsensor;
    var maxTframe = minTsensor;
    var minTframelocation = [0,0];
    var maxTframelocation = [0,0];
    var camColors = ["0000ff", "0100fe", "0200fd", "0300fc", "0400fb", "0500fa", "0600f9", "0700f8",
    "0800f7", "0900f6", "0a00f5", "0b00f4", "0c00f3", "0d00f2", "0e00f1", "0f00f0",
    "1000ef", "1100ee", "1200ed", "1300ec", "1400eb", "1500ea", "1600e9", "1700e8",
    "1800e7", "1900e6", "1a00e5", "1b00e4", "1c00e3", "1d00e2", "1e00e1", "1f00e0",
    "2000df", "2100de", "2200dd", "2300dc", "2400db", "2500da", "2600d9", "2700d8",
    "2800d7", "2900d6", "2a00d5", "2b00d4", "2c00d3", "2d00d2", "2e00d1", "2f00d0",
    "3000cf", "3100ce", "3200cd", "3300cc", "3400cb", "3500ca", "3600c9", "3700c8",
    "3800c7", "3900c6", "3a00c5", "3b00c4", "3c00c3", "3d00c2", "3e00c1", "3f00c0",
    "4000bf", "4100be", "4200bd", "4300bc", "4400bb", "4500ba", "4600b9", "4700b8",
    "4800b7", "4900b6", "4a00b5", "4b00b4", "4c00b3", "4d00b2", "4e00b1", "4f00b0",
    "5000af", "5100ae", "5200ad", "5300ac", "5400ab", "5500aa", "5600a9", "5700a8",
    "5800a7", "5900a6", "5a00a5", "5b00a4", "5c00a3", "5d00a2", "5e00a1", "5f00a0",
    "60009f", "61009e", "62009d", "63009c", "64009b", "65009a", "660099", "670098",
    "680097", "690096", "6a0095", "6b0094", "6c0093", "6d0092", "6e0091", "6f0090",
    "70008f", "71008e", "72008d", "73008c", "74008b", "75008a", "760089", "770088",
    "780087", "790086", "7a0085", "7b0084", "7c0083", "7d0082", "7e0081", "7f0080",
    "80007f", "81007e", "82007d", "83007c", "84007b", "85007a", "860079", "870078",
    "880077", "890076", "8a0075", "8b0074", "8c0073", "8d0072", "8e0071", "8f0070",
    "90006f", "91006e", "92006d", "93006c", "94006b", "95006a", "960069", "970068",
    "980067", "990066", "9a0065", "9b0064", "9c0063", "9d0062", "9e0061", "9f0060",
    "a0005f", "a1005e", "a2005d", "a3005c", "a4005b", "a5005a", "a60059", "a70058",
    "a80057", "a90056", "aa0055", "ab0054", "ac0053", "ad0052", "ae0051", "af0050",
    "b0004f", "b1004e", "b2004d", "b3004c", "b4004b", "b5004a", "b60049", "b70048",
    "b80047", "b90046", "ba0045", "bb0044", "bc0043", "bd0042", "be0041", "bf0040",
    "c0003f", "c1003e", "c2003d", "c3003c", "c4003b", "c5003a", "c60039", "c70038",
    "c80037", "c90036", "ca0035", "cb0034", "cc0033", "cd0032", "ce0031", "cf0030",
    "d0002f", "d1002e", "d2002d", "d3002c", "d4002b", "d5002a", "d60029", "d70028",
    "d80027", "d90026", "da0025", "db0024", "dc0023", "dd0022", "de0021", "df0020",
    "e0001f", "e1001e", "e2001d", "e3001c", "e4001b", "e5001a", "e60019", "e70018",
    "e80017", "e90016", "ea0015", "eb0014", "ec0013", "ed0012", "ee0011", "ef0010",
    "f0000f", "f1000e", "f2000d", "f3000c", "f4000b", "f5000a", "f60009", "f70008",
    "f80007", "f90006", "fa0005", "fb0004", "fc0003", "fd0002", "fe0001", "ff0000"
    ];
    function mapto( val, minin, maxin, minout ,maxout ){
      if ( val >= maxin ){
        return maxout;
      } else if ( val <= minin ){
        return minout;
      } else {
        return ((( ( maxout - minout )/( maxin - minin ) ) * val ) + ( minout - ( ( maxout - minout )/( maxin - minin ) ) * minin ));
      }
    }
    function adjusttemrange(range,adjustment){
      if ( range == "min" ){
        if ( adjustment == 0){
          minT = minTframe;
        } else {
          minT += adjustment;
        }
      }
      if ( range == "max" ){
        if ( adjustment == 0){
          maxT = maxTframe;
        } else {
          maxT += adjustment;
        }
      }
      if ( minT < minTsensor ){
        minT = minTsensor;
      }
      if ( maxT > maxTsensor ){
        maxT = maxTsensor;
      }
      if ( minT >= maxT ){
        minT = maxT - 1.0;
      }
      fillthermcamtable(currentFrame);
      document.getElementById("minT").innerHTML = minT.toFixed(2) + "C";
      document.getElementById("maxT").innerHTML = maxT.toFixed(2) + "C";
    }
    function ctof(tempc){
      return (tempc * ( 9 / 5 ) ) + 32;
    }
    function buildthermcamtable(){
      var thermcamTable = document.getElementById("thermcam");
      for(i=0;i<sizesensor[0];i++){
        var row = thermcamTable.insertRow(i);
        for(j=0;j<sizesensor[1];j++){
          var cell = row.insertCell(j);
          cell.id = "tccell"+((i*8)+j);
        }
      }
    }
    function fillthermcamtable(pixelarray){
      minTframe = maxTsensor;
      maxTframe = minTsensor;

      // Fill table
      // Temps in C and F in cells
      // BG color of cells use camColors
      // Update minTframe and maxTframe
      lista = pixelarray.reverse();
      for(i=0;i<sizesensor[0];i++){
        for(j=0;j<sizesensor[1];j++){
          var idx = ((i*8)+j);
          var cell = document.getElementById("tccell"+idx);
          var camcolorvalue = Math.round( mapto( lista[idx].toString().substr(0, 4), minT , maxT, 0 ,255 ) );
          cell.innerHTML = lista[idx].toFixed(2) + "C <br/>";
          cell.innerHTML += lista[idx].toString().slice(5) + "mm<br/> ";
          cell.style.backgroundColor = "#" + camColors[camcolorvalue];
          cell.className = "camcolorvalue" + camcolorvalue;
          if ( lista[idx] < minTframe ){
            minTframe = lista[idx];
            minTframelocation = [j,i];
          }
          if ( lista[idx] > maxTframe ){
            maxTframe = lista[idx];
            maxTframelocation = [j,i];
          }
        }
      }
      //document.getElementById("minTframedata").innerHTML = minTframe.toFixed(2) + "C " + ctof( minTframe ).toFixed(2) + "F (" + minTframelocation[0] + "," + minTframelocation[1] + ")";
     // document.getElementById("maxTframedata").innerHTML = maxTframe.toFixed(2) + "C " + ctof( maxTframe ).toFixed(2) + "F (" + maxTframelocation[0] + "," + maxTframelocation[1] + ")";
    }
    function init() {
      // Build table
      buildthermcamtable();
      document.getElementById("minT").innerHTML = minT.toFixed(2) + "C";
      document.getElementById("maxT").innerHTML = maxT.toFixed(2) + "C";
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        if ( event.data[0] == '[' ){
          currentFrame = JSON.parse(event.data);
          fillthermcamtable(currentFrame);
        }
      }
    }
    function sendText(){
      Socket.send(document.getElementById("txBar").value);
      document.getElementById("txBar").value = "";
    }
    function getframe(){
      Socket.send("F");
    }
    function streamframes(){
      streamframestimer = setTimeout( streamframes , 200 );
      getframe();
    }
    function stopstreamframes(){
      clearTimeout( streamframestimer );
    }
  </script>
</head>
<body onload="init()">
  <table id="thermcam">
  
  </table>
  <hr/>
  <table id="thermcamcontrols">
    <tr>
      <td>Min Temp</td>
      <td id="minT"></td>
      <td><button type="button" onclick="adjusttemrange('min',-5);">-5C</button></td>
      <td><button type="button" onclick="adjusttemrange('min',-1);">-1C</button></td>
      <td><button type="button" onclick="adjusttemrange('min', 0);">lowest in current frame</button></td>
      <td><button type="button" onclick="adjusttemrange('min',+1);">+1C</button></td>
      <td><button type="button" onclick="adjusttemrange('min',+5);">+5C</button></td>
      <td id="minTframedata"></td>
    </tr>
    <tr>
      <td>Max Temp</td>
      <td id="maxT"></td>
      <td><button type="button" onclick="adjusttemrange('max',-5);">-5C</button></td>
      <td><button type="button" onclick="adjusttemrange('max',-1);">-1C</button></td>
      <td><button type="button" onclick="adjusttemrange('max', 0);">highest in current frame</button></td>
      <td><button type="button" onclick="adjusttemrange('max',+1);">+1C</button></td>
      <td><button type="button" onclick="adjusttemrange('max',+5);">+5C</button></td>
      <td id="maxTframedata"></td>
    </tr>
  </table>
  <hr/>
  <div>
    <button type="button" onclick="getframe();">Get Frame</button>
    <button type="button" onclick="streamframes();">Start Stream Frames</button>
    <button type="button" onclick="stopstreamframes();">Stop Stream Frames</button>
  </div>
</body>
</html>
)=====";

void setup() {
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.begin(115200);

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(myIP);

  server.on("/", []() {
    server.send_P(200, "text/html", webpage);
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Wire.begin();            //This resets to 100kHz I2C
  Wire.setClock(1000000);  //Sensor has max I2C freq of 400kHz

  Serial.println();
  Serial.println("Iniciando sensor VL53L5.");
  Serial.println("Por favor, aguardem até que seja carregado. Podem ser necessários alguns segundos.");
  if (myImager.begin() == false) {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1)
      ;
  } else {
    Serial.println(F("Sensor VL53L5 Iniciado. It's work"));
  }

  myImager.setResolution(8 * 8);  //Enable all 64 pads

  imageResolution = myImager.getResolution();  //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = sqrt(imageResolution);          //Calculate printing width

  myImager.startRanging();
  Serial.println();
  //

  amg.begin();
}

void loop() {
  webSocket.loop();
  server.handleClient();
  if (Serial.available() > 0) {
    char c[] = { (char)Serial.read() };
    webSocket.broadcastTXT(c, sizeof(c));
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    if (payload[0] == 'F') {  // i.e. "Frame"

      if (myImager.isDataReady() == true) {
        if (myImager.getRangingData(&measurementData))  //Read distance data into array
        {
          Serial.println("lendo VL53L5cx measurementData");
          Serial.println();
        } else {
          Serial.println("Não leu VL53L5cx ");
          Serial.println();
        }
      } else {
        Serial.println("myImager.isDataReady() é false ");
        Serial.println();
      }

      amg.readPixels(pixels);

      String thermalFrame = "[";

      for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {  // Starts at 1 so that (i % 8) will be 0 at end of 8 pixels and newline will be added
        thermalFrame += pixels[i];
        if (i != (AMG88xx_PIXEL_ARRAY_SIZE - 1)) {
          thermalFrame += ", ";
          // if (i % 8 == 0) thermalFrame += "\n";
        }
      }
      thermalFrame += "]";

      Serial.println();

      char tf[thermalFrame.length() + 1];
      thermalFrame.toCharArray(tf, thermalFrame.length() + 1);
      webSocket.broadcastTXT(tf, sizeof(tf) - 1);
    }
  }
}