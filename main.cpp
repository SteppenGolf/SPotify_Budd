#include <Arduino.h>
//The JPEG Decoder library for thumbnails
#include <TJpg_Decoder.h>
//For SPIFFS
#include <FS.h> 
#include "user_settings.h"
//Include JSON
#include <ArduinoJson.h>


#include <base64.h>

// Libraries for wifi and internet communcaiton.
#include <esp_wifi.h>
#include <esp_http_client.h>
#include <esp_http_server.h>
#include <WiFiClientSecure.h> //for ssl

//Some custom headers here

//TFT library & SPI

#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
int imageOffset_X = 25;
int imageOffset_Y = 20;

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){

    // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);
  // Return 1 to decode next block
  return 1;

}


/*=========================
|User modifiable variables|
=========================*/
// WiFi credentials
#define WIFI_SSID "YOUR_WIFI_NAME"
#define PASSWORD "YOUR_WIFI_PASSWORD"

// Spotify API credentials
#define CLIENT_ID "YOUR_CLIENT_ID"
#define CLIENT_SECRET "YOUR_CLIENT_SECRET"
#define REDIRECT_URI "http://YOUR_ESP_IP/callback"

String getValue(esp_http_client &http, String key) {

  bool found = false, look = false, seek = true;
  int ind = 0;
  String ret_str = "";

  int len = http.getSize();
  char char_buff[1];
Rover Defender old

  WiFiClient *stream = http.getStreamPtr();

  while(http.connected()&&(len > 0 || len == -1 )){
    size_t size = stream -> available();

    if(size){
      int c = stream -> readBytes(char_buff, ((size> sizeof(char_buff))? sizeof(char_buff):size ));
      if(found){
        if(seek && char_buff[0] != ':'){
          continue;

        }else if (char_buff[0] != '\n'){
          seek = false;
          int c= stream -> readBytes(char_buff, 1);

        }

        else break;



      }
      else if ((!look)&& (char_buff[0]== key[0])){
        look = true;
        ind = 1;
      }
      else if(look && (char_buff[0]==key[ind])){
        ind++;

        if (ind == key.length()) found = true;

      }
      else if (look && (char_buff[0]!=key[ind])){
        ind = 0;
        look = false;

      }
    }
  }

  if(*(ret_str.end()-1)==','){
    ret_str = ret_str.substring(0,ret_str.length()-1);   
  }
  return ret_str;
}

/*HTTP response Structure*/

struct httpResponse{

  int responseCode;
  String responseMessage;

};

struct songDetails{

  int durationMs;
  String album;
  String artist;
  String song;
  String Id;
  bool isLiked;
};

char *parts[10];


void printSplitString(String text,int maxLineSize, int yPos)
{
    int currentWordStart = 0;
    int spacedCounter = 0;
    int spaceIndex = text.indexOf(" ");
    
    while(spaceIndex != -1){
        char *part = parts[spacedCounter]; 
        sprintf(part,text.substring(currentWordStart,spaceIndex).c_str());
        currentWordStart = spaceIndex;
        spacedCounter++;
        spaceIndex = text.indexOf(" ",spaceIndex+1);
    }

    char *part = parts[spacedCounter]; 
    sprintf(part,text.substring(currentWordStart,text.length()).c_str());
 
    currentWordStart = spaceIndex;
    size_t counter = 0;
    currentWordStart = 0;
    while(counter <= spacedCounter){
        char printable[maxLineSize];
        char* printablePointer = printable;
        sprintf(printablePointer,parts[counter]);
        int currentLen = 0;
        while(parts[counter][currentLen] != '\0'){
            currentLen++;
            printablePointer++;
        }
        counter++;
        while(counter <= spacedCounter){
            int nextLen = 0;
            while(parts[counter][nextLen] != '\0'){
                nextLen++;
            }
            if(currentLen + nextLen > maxLineSize)
                break;
            sprintf(printablePointer, parts[counter]);
            currentLen += nextLen;
            printablePointer += nextLen;
            counter++;
        }
        String output = String(printable);
        if(output[0] == ' ')
            output = output.substring(1);
        tft.setCursor((int)(tft.width()/2 - tft.textWidth(output) / 2),tft.getCursorY());
        tft.println(output);

    }

}

class Spot_Conn{
  public:
  Spot_Conn(){
    client = 
    client-> setInsecu
  }


}








// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}