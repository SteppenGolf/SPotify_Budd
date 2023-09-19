#define CORE_DEBUG_LEVEL ARDUHAL_LOG_LEVEL_VERBOSE

#include<HTTPClient.h>
#include"Spotfiy_Client.h"
#include<base64.h>

Spotify_Client::Spotify_Client(String client_Id, String client_Secret,  String device_Name, String refresh_Token ){
    this->client_Id = client_Id;
    this->client_Secret = client_Secret;
    this-> device_Name = device_Name;
    this->refresh_Token = refresh_Token;

    client.setCACert(digicert_root_ca);

}

void Spotify_Client::fetch_Token(){

    HTTPClient http;

    String body = "grant_type=refresh_token&refresh_token=" + refresh_Token;
    String authorization_Raw = client_Id + ":" + client_Secret;
    String authorization = base64::encode(authorization_Raw);
    http.begin(client, "https://accounts.spotify.com/api/token"); 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Basic " + authorization);

    int http_Code = http.POST(body);
    if(http_Code>0){
        String returned_Payload = http.getString();
        if(http_Code == 200){
            access_Token = parse_Json("access_token",returned_Payload);
            Serial.println("Got the new access token");


        }
        else {
            Serial.println("failed to fetch token");
            Serial.println(http_Code);
            Serial.println(returned_Payload);

        }

    }
    else{
        Serial.println("failed to connect to the https://accounts.spotify.com/api/token");
    }
    http.end();

}

int Spotify_Client::Shuffle(){
    Serial.println("Shuffle() called");
    Http_Result result = CallAPI( "PUT", "https://api.spotify.com/v1/me/player/shuffle?state=true&device_id=" + device_Id, "");
    return result.http_Code;
}

int Spotify_Client::skip_Forward(){
    Serial.println("skip_Forward() called");
    Http_Result result = CallAPI( "POST", "https://api.spotify.com/v1/me/player/next?device_id=" + device_Id, "");
    return result.http_Code;
}

int Spotify_Client::skip_Back(){
    Serial.println("skip_Back() called");
    Http_Result result = CallAPI( "POST", "https://api.spotify.com/v1/me/player/previous?device_id=" + device_Id, "");
    return result.http_Code;
}

int Spotify_Client::toggle_Play(String context_uri){
    Serial.println("toggle_Play() called");
    String body = "{\"context_uri\":\"" + context_uri + "\",\"offset\":{\"position\":0,\"position_ms\":0}}";
    String url = "https://api.spotify.com/v1/me/player/play?device_id=" + device_Id;
    Http_Result result = CallAPI( "PUT", url, body );
    return result.http_Code;
}   

int Spotify_Client::vol_Adjust(int vol){
    Serial.println("vol_Adjust() called");
    String url = "https://api.spotify.com/v1/me/player/volume?volume_percent=" + String(vol);
    Http_Result result = CallAPI("PUT",url,"");
     return result.http_Code;
}

/*

Code for gettign song info coming here soon 

*/
/*Parse JSON Function*/


Http_Result Spotify_Client::CallAPI(String method, String url, String body){

    Http_Result result;
    result.http_Code = 0;
    Serial.println(url);
    Serial.print("Returned");


    HTTPClient http;
    http.begin(client,url);

    String auth = "Bearer" + access_Token;
    http.addHeader(F("Content-Type"),"application/json");
    http.addHeader(F("Authorization"),auth);

    if(body.length()==0) http.addHeader(F("Content-Length"),String(0));
    if(method == "PUT")         result.http_Code = http.PUT(body);
    else if (method == "POST")  result.http_Code = http.POST(body);
    else if (method =="GET")    result.http_Code = http.GET(body);

    if(result.http_Code> 0){
        Serial.println(result.http_Code);
        if(http.getSize()>0) result.payload = http.getString();

    }
    else{
        Serial.print("Failed to connect to ");
        Serial.println(url);

    }
http.end();

return result;

}

String Spotify_Client::get_Devices() {

    Http_Result result  = CallAPI("GET","https://api.spotify.com/v1/me/player/devices", "" );
    device_Id = GetDeviceId(result.payload);
    Serial.println("Device ID : ");
    Serial.println(device_Id);
    return device_Id;
}


String Spotify_Client::GetDeviceId(String json) {
    
    
}





