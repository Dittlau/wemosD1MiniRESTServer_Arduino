#include "restServer.h"

/**
 * @description: Processes the incoming url 
 * @parameters: WifiClient client - the active WiFi connection
 */

void process(WiFiClient client) {

  //////////////////////////////////////////////////////////////////////////////////
  // Determine GET, POST, PUT or DELETE request, then process type, eg. "lights"  //
  //                                                                              //
  //////////////////////////////////////////////////////////////////////////////////

  String command = client.readStringUntil('/');

  if (command == "GET ") {
    Serial.println("\"GET\" received");
    String type = client.readStringUntil('/');
    Serial.print("Type ");
    Serial.print(type);
    Serial.println(" received");
    if (type == "lights") {
      lightControlGet(client);
    }
    else {
      Serial.print("Wrong type received: ");
      Serial.println(type);
      errorResponse(client, "Wrong type received: ", type);
    }
  }
  else if (command == "PUT ") {
    Serial.println("\"PUT\" received");
    String type = client.readStringUntil('/');
    Serial.print("Type ");
    Serial.print(type);
    Serial.println(" received");
    if (type == "lights") {
      lightControlPut(client);
    }
    else {
      Serial.print("Wrong type received: ");
      Serial.println(type);
      errorResponse(client, "Wrong type received: ", type);
    }
  }
  else {
    Serial.print("Wrong command received: ");
    Serial.println(command);
    errorResponse(client, "Wrong command received: ", command);
  }
}

/**
 * @description:  Called if the URL is of type GET and the first part of url is "lights". 
 *                This function prints the selected light's status, or all available lights, 
 *                if no light-ID is present in the URL (note that a character e.g. 'a' will 
 *                also be registered as no value). 
 * @parameters: WifiClient client - the active WiFi connection
 */

void lightControlGet(WiFiClient client) {

  String lightNoTemp;
  int lightNo;
  int lightId;
  lightNo = (client.readStringUntil('/').toInt());

  if (lightNo == NULL) {
    client.print("HTTP/1.1 200 OK");
    sendHTMLHeader(client);
    client.print("Sending back all active lights: ");
    for (int i = 0; i < (sizeof lights / sizeof lights[0]); i++) {
      if (lights[i].id != 0) {
        client.print(lights[i].id);
      }
    }
    client.print("</body>");
    client.print("</html>");
  }
  else {
    
    // if a number comes after, check that it matches
    lightId = -1;
    for (int i = 0; i < (sizeof lights / sizeof lights[0]); i++) {
      if (lights[i].id == lightNo) {
        lightId = lightNo;
        
        // is the wanted parameter specified
        String parameter = client.readStringUntil('/');
        if (parameter == "onStatus"){
          client.print("HTTP/1.1 200 OK");
          sendHTMLHeader(client);
          client.print("Status: ");
          client.print(lights[lightNo - 1].onStatus);
          client.print("<br />");
          client.print("</body>");
          client.print("</html>");
        }
        else if (parameter == "intensity"){
          client.print("HTTP/1.1 200 OK");
          sendHTMLHeader(client);
          client.print("Intensity: ");
          client.print(lights[lightNo - 1].intensity);
          client.print("<br />");
          client.print("</body>");
          client.print("</html>");
        }
        else {
          lightStatus(client, lightNo);
        }      
      }
    }
    
    // if lightId has'nt been updated, the light no is invalid
    if (lightId == -1) {
      client.print("HTTP/1.1 200 OK");
      sendHTMLHeader(client);
      client.print("Light ");
      client.print(lightNo);
      client.print(" does not exist!");
      client.print("</body>");
      client.print("</html>");
    }
  }
}

/**
 * @description:  Called if the URL is of type PUT and the first part of url is "lights". 
 *                This function prints the selected light's status, or all available lights, 
 *                if no light-ID is present in the URL (note that a character e.g. 'a' will 
 *                also be registered as no value). 
 * @parameters: WifiClient client - the active WiFi connection
 */
 
void lightControlPut(WiFiClient client) {

  String lightNoTemp;
  int lightNo;
  int lightId;
  lightNo = (client.readStringUntil('/').toInt());
  
  if (lightNo == NULL) {
    sendHTMLHeader(client);
    client.print("No light selected");
    client.print("</body>");
    client.print("</html>");
  }
  else {

    // if a number comes after, check that it matches
    lightId = -1;
    for (int i = 0; i < (sizeof lights / sizeof lights[0]); i++) {
      if (lights[i].id == lightNo) {
        lightId = lightNo;
        
        
        String parameter = client.readStringUntil('/');
        String value = client.readStringUntil('/');
        
        if (parameter == "onStatus"){
          // Begin the HTML
          client.print("HTTP/1.1 200 OK");
          sendHTMLHeader(client);
          
          
          if (value == "on"){
            client.print("onStatus is set to On");
          }
          else if (value == "off"){
            client.print("onStatus is set to Off");
          }
          else {
            client.print("Invalid value!");
          }
          // End the HTML
          client.print("</body>");
          client.print("</html>");
        }
        else if (parameter == "intensity"){

          int intensity = value.toInt();
          
          // Begin the HTML
          client.print("HTTP/1.1 200 OK");
          sendHTMLHeader(client);
          
          
          if ((intensity >= 0) && (intensity <= 100)){
            client.print("Intensity is set to ");
            client.print(intensity);
          }
          else{
            client.print("Intensity is out of range!");
          }
          
          // End the HTML
          client.print("</body>");
          client.print("</html>");
        }
        else {
          // No valid parameter found
          client.print("HTTP/1.1 200 OK");
          sendHTMLHeader(client);
          client.print("Unknown parameter!");
          client.print("</body>");
          client.print("</html>");
        }      
      }
    }
    
    // if lightId has'nt been updated, the light no is invalid
    if (lightId == -1) {
      client.println("HTTP/1.1 200 OK");
      sendHTMLHeader(client);
      client.println("Light ");
      client.println(lightNo);
      client.println(" does not exist!");
      client.println("</body>");
      client.println("</html>");
    }
  }
}

//////////////////////////////////////
//                                  //
// Helper functions for handling    //
// responses to requests:           //
//                                  //
//////////////////////////////////////

void lightStatus(WiFiClient client, int lightNo) {
  client.println("HTTP/1.1 200 OK");
  sendHTMLHeader(client);
  client.print("ID: ");
  client.print(lights[lightNo - 1].id);
  client.print("<br />");
  client.print("Status: ");
  client.print(lights[lightNo - 1].onStatus);
  client.print("<br />");
  client.println("</body>");
  client.println("</html>");
}


void lightResponse(WiFiClient client, int lightNo, String value) {
  client.println("HTTP/1.1 200 OK");
  sendHTMLHeader(client);
  client.print("Light: ");
  client.print(lightNo);
  client.print(" turned ");
  client.print(value);
  client.println("</body>");
  client.println("</html>");
}

void errorResponse(WiFiClient client, String text, String value) {
  client.println("HTTP/1.1 400 Bad Request");
  sendHTMLHeader(client);
  client.print(text);
  client.println(value);
  client.println("</body>");
  client.println("</html>");
}

void errorResponseLight(WiFiClient client, int lightNo, String action) {
  client.println("HTTP/1.1 400 Bad Request");
  sendHTMLHeader(client);
  client.print("Unknown light: ");
  client.print(lightNo);
  client.print(" or unknown action: ");
  client.println(action);
  client.println("</body>");
  client.println("</html>");
}

void sendHTMLHeader(WiFiClient client) {
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Arduino Web Page</title>");
  client.println("</head>");
  client.println("<body>");
}



