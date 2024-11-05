#include <WiFi.h>                      // Part of ESP32 board libraries
#include <WebServer.h>                 // Part of ESP32 board libraries
#include <WebSocketsServer_Generic.h>  // Installed "WebSockets" library

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
    var maxTsensor = 530.0;
    var minT = 50.0; // initial low range of the sensor (this will be blue on the screen)
    var maxT = 530.0; // initial high range of the sensor (this will be red on the screen)
    var currentFrame = [];
    var minTframe = maxTsensor;
    var maxTframe = minTsensor;
    var minTframelocation = [0,0];
    var maxTframelocation = [0,0];
    var camColors = ["0000FF","0000FE","0000FE","0100FD","0100FD","0200FC","0200FC","0300FB","0300FB","0400FA","0400FA","0500F9","0500F9","0600F8","0600F8",
    "0700F7","0700F7","0800F6","0800F6","0900F5","0900F5","0A00F4","0A00F4","0B00F3","0B00F3","0C00F2","0C00F2","0D00F1","0D00F1","0D00F1",
    "0E00F0","0E00F0","0F00EF","0F00EF","1000EE","1000EE","1100ED","1100ED","1200EC","1200EC","1300EB","1300EB","1400EA","1400EA","1500E9",
    "1500E9","1600E8","1600E8","1700E7","1700E7","1800E6","1800E6","1900E5","1900E5","1A00E4","1A00E4","1A00E4","1B00E3","1B00E3","1C00E2",
    "1C00E2","1D00E1","1D00E1","1E00E0","1E00E0","1F00DF","1F00DF","2000DE","2000DE","2100DD","2100DD","2200DC","2200DC","2300DB","2300DB",
    "2400DA","2400DA","2500D9","2500D9","2600D8","2600D8","2700D7","2700D7","2800D6","2800D6","2800D6","2900D5","2900D5","2A00D4","2A00D4",
    "2B00D3","2B00D3","2C00D2","2C00D2","2D00D1","2D00D1","2E00D0","2E00D0","2F00CF","2F00CF","3000CE","3000CE","3100CD","3100CD","3200CC",
    "3200CC","3300CB","3300CB","3400CA","3400CA","3500C9","3500C9","3500C9","3600C8","3600C8","3700C7","3700C7","3800C6","3800C6","3900C5",
    "3900C5","3A00C4","3A00C4","3B00C3","3B00C3","3C00C2","3C00C2","3D00C1","3D00C1","3E00C0","3E00C0","3F00BF","3F00BF","4000BE","4000BE",
    "4100BD","4100BD","4200BC","4200BC","4300BB","4300BB","4300BB","4400BA","4400BA","4500B9","4500B9","4600B8","4600B8","4700B7","4700B7",
    "4800B6","4800B6","4900B5","4900B5","4A00B4","4A00B4","4B00B3","4B00B3","4C00B2","4C00B2","4D00B1","4D00B1","4E00B0","4E00B0","4F00AF",
    "4F00AF","5000AE","5000AE","5000AE","5100AD","5100AD","5200AC","5200AC","5300AB","5300AB","5400AA","5400AA","5500A9","5500A9","5600A8",
    "5600A8","5700A7","5700A7","5800A6","5800A6","5900A5","5900A5","5A00A4","5A00A4","5B00A3","5B00A3","5C00A2","5C00A2","5D00A1","5D00A1",
    "5D00A1","5E00A0","5E00A0","5F009F","5F009F","60009E","60009E","61009D","61009D","62009C","62009C","63009B","63009B","64009A","64009A",
    "650099","650099","660098","660098","670097","670097","680096","680096","690095","690095","6A0094","6A0094","6B0093","6B0093","6B0093",
    "6C0092","6C0092","6D0091","6D0091","6E0090","6E0090","6F008F","6F008F","70008E","70008E","71008D","71008D","72008C","72008C","73008B",
    "73008B","74008A","74008A","750089","750089","760088","760088","770087","770087","780086","780086","780086","790085","790085","7A0084",
    "7A0084","7B0083","7B0083","7C0082","7C0082","7D0081","7D0081","7E0080","7E0080","7F007F","7F007F","80007E","80007E","81007D","81007D",
    "82007C","82007C","83007B","83007B","84007A","84007A","850079","850079","860078","860078","860078","870077","870077","880076","880076",
    "890075","890075","8A0074","8A0074","8B0073","8B0073","8C0072","8C0072","8D0071","8D0071","8E0070","8E0070","8F006F","8F006F","90006E",
    "90006E","91006D","91006D","92006C","92006C","93006B","93006B","93006B","94006A","94006A","950069","950069","960068","960068","970067",
    "970067","980066","980066","990065","990065","9A0064","9A0064","9B0063","9B0063","9C0062","9C0062","9D0061","9D0061","9E0060","9E0060",
    "9F005F","9F005F","A0005E","A0005E","A1005D","A1005D","A1005D","A2005C","A2005C","A3005B","A3005B","A4005A","A4005A","A50059","A50059",
    "A60058","A60058","A70057","A70057","A80056","A80056","A90055","A90055","AA0054","AA0054","AB0053","AB0053","AC0052","AC0052","AD0051",
    "AD0051","AE0050","AE0050","AE0050","AF004F","AF004F","B0004E","B0004E","B1004D","B1004D","B2004C","B2004C","B3004B","B3004B","B4004A",
    "B4004A","B50049","B50049","B60048","B60048","B70047","B70047","B80046","B80046","B90045","B90045","BA0044","BA0044","BB0043","BB0043",
    "BB0043","BC0042","BC0042","BD0041","BD0041","BE0040","BE0040","BF003F","BF003F","C0003E","C0003E","C1003D","C1003D","C2003C","C2003C",
    "C3003B","C3003B","C4003A","C4003A","C50039","C50039","C60038","C60038","C70037","C70037","C80036","C80036","C90035","C90035","C90035",
    "CA0034","CA0034","CB0033","CB0033","CC0032","CC0032","CD0031","CD0031","CE0030","CE0030","CF002F","CF002F","D0002E","D0002E","D1002D",
    "D1002D","D2002C","D2002C","D3002B","D3002B","D4002A","D4002A","D50029","D50029","D60028","D60028","D60028","D70027","D70027","D80026",
    "D80026","D90025","D90025","DA0024","DA0024","DB0023","DB0023","DC0022","DC0022","DD0021","DD0021","DE0020","DE0020","DF001F","DF001F",
    "E0001E","E0001E","E1001D","E1001D","E2001C","E2001C","E3001B","E3001B","E4001A","E4001A","E4001A","E50019","E50019","E60018","E60018",
    "E70017","E70017","E80016","E80016","E90015","E90015","EA0014","EA0014","EB0013","EB0013","EC0012","EC0012","ED0011","ED0011","EE0010",
    "EE0010","EF000F","EF000F","F0000E","F0000E","F1000D","F1000D","F1000D","F2000C","F2000C","F3000B","F3000B","F4000A","F4000A","F50009",
    "F50009","F60008","F60008","F70007","F70007","F80006","F80006","F90005","F90005","FA0004","FA0004","FB0003","FB0003","FC0002","FC0002",
    "FD0001","FD0001","FE0000","FE0000","FF0000"
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
          cell.innerHTML = lista[idx]+ "mm<br/> ";
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

  Wire.begin();           //This resets to 100kHz I2C
  Wire.setClock(400000);  //Sensor has max I2C freq of 400kHz

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

}

void loop() {
  webSocket.loop();
  server.handleClient();
  if (Serial.available() > 0) {
    char c[] = { (char)Serial.read() };
    webSocket.broadcastTXT(c, sizeof(c));
  }
    delay(100);  //Small delay between polling
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

      String thermalFrame = "[";

      for (int i = 0; i <= (imageResolution - 1); i++) {
        thermalFrame += measurementData.distance_mm[i];
        if (i != (imageResolution - 1)) {
          thermalFrame += ", ";
        }
      }
      thermalFrame += "]";

      // Serial.println(thermalFrame);
      // Serial.println();

      char tf[thermalFrame.length() + 1];
      thermalFrame.toCharArray(tf, thermalFrame.length() + 1);
      webSocket.broadcastTXT(tf, sizeof(tf) - 1);
    }
  }
}