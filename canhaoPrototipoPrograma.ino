#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "Pepper";
const char* password = "vaidereto89";

WiFiServer server(80);

Servo servoX;
Servo servoY;
Servo servoPush;

int unitsValue, xPosition, yPosition, pushBulletPosition;
String header;

const int projectileThrottle = D7;
const int timeoutDefault = 2000;

void setup() {
  Serial.begin(115200);

  servoX.attach(D1);
  servoY.attach(D2);
  servoPush.attach(D5);

  xPosition = 105;
  yPosition = 90;
  pushBulletPosition = 160;

  servoX.write(xPosition);
  servoY.write(yPosition);
  servoPush.write(pushBulletPosition);

  unitsValue=10;

  pinMode(projectileThrottle, OUTPUT);
  digitalWrite(projectileThrottle, LOW);
  
  initWifi();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    
    processRequest(client);
    
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}

void initWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void processRequest(WiFiClient client) {
  String currentLine = "";

  unsigned long currentTime = millis();
  unsigned long previousTime = currentTime;

  while (client.connected() && currentTime - previousTime <= timeoutDefault) {
    currentTime = millis();

    if (client.available()) {
      
      char c = client.read();
      Serial.write(c);
      header += c;

      if (c == '\n') {
        if (currentLine.length() == 0) {
          handleFrontEnd(client);
          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }

  }
}

void handleFrontEnd(WiFiClient client) {
  createHeader(client);
  handleParams(client);
  createBody(client);
}

void handleParams(WiFiClient client) {
  if (header.indexOf("GET /set_units") >= 0) {
    int unitsIndex = header.indexOf("units=");
    unitsValue = header.substring(unitsIndex + 6).toInt();
  } else {
    if (header.indexOf("GET /up") >= 0) {
      yPosition = yPosition - unitsValue;
      servoY.write(yPosition);
    } else if (header.indexOf("GET /down") >= 0) {
      yPosition = yPosition + unitsValue;
      servoY.write(yPosition);
    } else if (header.indexOf("GET /left") >= 0) {
      xPosition = xPosition - unitsValue;
      servoX.write(xPosition);
    } else if (header.indexOf("GET /right") >= 0) {
      xPosition = xPosition + unitsValue;
      servoX.write(xPosition);
    } else if (header.indexOf("GET /fire") >= 0) {
      fire();
    }
  }  
}

void fire() {
  digitalWrite(D7, HIGH);
  servoPush.write(0);
  delay(300);
  servoPush.write(160);
  delay(300);
  digitalWrite(D7, LOW);
}

void createHeader(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
}

void createBody(WiFiClient client) {
  client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head>");
    client.println("  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("  <link rel=\"icon\" href=\"data:,\">");
    client.println("  <style>");
    client.println("    html { ");
    client.println("      font-family: Helvetica; ");
    client.println("      display: inline-block; ");
    client.println("      margin: 0px auto; ");
    client.println("      text-align: center;");
    client.println("      background-color: #333333;");
    client.println("      color: white;");
    client.println("    }");
    client.println("    .button { ");
    client.println("      background-color: #ff8c00; ");
    client.println("      border: none; ");
    client.println("      color: white; ");
    client.println("      padding: 10px 20px;");
    client.println("      text-decoration: none; ");
    client.println("      font-size: 16px; ");
    client.println("      margin: 2px; ");
    client.println("      cursor: pointer;");
    client.println("      border-radius: 5px;");
    client.println("    }");
    client.println("    .button:hover { ");
    client.println("      background-color: #e67e00;");
    client.println("    }");
    client.println("    .dpad { ");
    client.println("      display: grid; ");
    client.println("      grid-template-columns: repeat(3, 1fr); ");
    client.println("      grid-template-rows: repeat(3, 1fr); ");
    client.println("      grid-gap: 10px; ");
    client.println("      width: auto; ");
    client.println("      margin: 20px auto;");
    client.println("    }");
    client.println("    .dpad > div { ");
    client.println("      display: flex; ");
    client.println("      justify-content: center; ");
    client.println("      align-items: center; ");
    client.println("    }");
    client.println("    .dpad .up { ");
    client.println("      grid-column: 2; ");
    client.println("      grid-row: 1; ");
    client.println("    }");
    client.println("    .dpad .down { ");
    client.println("      grid-column: 2; ");
    client.println("      grid-row: 3; ");
    client.println("    }");
    client.println("    .dpad .left { ");
    client.println("      grid-column: 1; ");
    client.println("      grid-row: 2; ");
    client.println("    }");
    client.println("    .dpad .right { ");
    client.println("      grid-column: 3; ");
    client.println("      grid-row: 2; ");
    client.println("    }");
    client.println("    .fire {");
    client.println("      margin-top: 20px;");
    client.println("      font-size: 36px;");
    client.println("    }");
    client.println("  </style>");
    client.println("</head>");
    client.println("<body>");
    client.println("  <div style=\"text-align: center;\"><h1>NERF CANNON</h1></div>");
    client.println("  <div class=\"dpad\">");
    client.println("    <div class=\"up\">");
    client.println("      <a href=\"/up\">");
    client.println("        <button class=\"button\">UP</button>");
    client.println("      </a>");
    client.println("    </div>");
    client.println("    <div class=\"left\">");
    client.println("      <a href=\"/left\">");
    client.println("        <button class=\"button\">LEFT</button>");
    client.println("      </a>");
    client.println("    </div>");
    client.println("    <div class=\"right\">");
    client.println("      <a href=\"/right\">");
    client.println("        <button class=\"button\">RIGHT</button>");
    client.println("      </a>");
    client.println("    </div>");
    client.println("    <div class=\"down\">");
    client.println("      <a href=\"/down\">");
    client.println("        <button class=\"button\">DOWN</button>");
    client.println("      </a>");
    client.println("    </div>");
    client.println("  </div>");
    client.println("  <div class=\"fire\">");
    client.println("    <a href=\"/fire\">");
    client.println("      <button class=\"button\">FIRE</button>");
    client.println("    </a>");
    client.println("  </div>");
    client.println("  <div style=\"margin-top: 30px;\">");
    client.println("    <form action=\"/set_units\" method=\"GET\">");
    client.println("      <label for=\"units\">Units:</label>");
    client.println("      <input type=\"number\" id=\"units\" name=\"units\" value=\"" + String(unitsValue) + "\" min=\"1\">");
    client.println("      <input type=\"submit\" value=\"Set Units\">");
    client.println("    </form>");
    client.println("  </div>");
    client.println("</body>");
    client.println("</html>");
}
