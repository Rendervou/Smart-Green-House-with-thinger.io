#include <ESP8266WiFi.h> //Library untuk konek ke Wi-Fi
#include <ThingerESP8266.h> //Library untuk terhubung ke Thinger
#include <NewPing.h> //Library untuk HC-SR04

#define USERNAME " Al fathi" //Username Thinger IO
#define DEVICE_ID "MCUNR" //Device ID Thinger IO
#define DEVICE_CREDENTIAL "uyJM8A2oL7OZu&8n" //Device Credential
#define SSID "idnboardingschool.net" //SSID Wi-Fi
#define SSID_PASSWORD "123" //Password Wi-Fi

#define TRIGGER_PIN D5 //Pin Trigger HC-SR04 pada NodeMCU
#define ECHO_PIN D6 //Pin Echo HC-SR04 pada NodeMCU
#define MAX_DISTANCE 250 //Maksimum Pembacaan Jarak (cm)
#define pinYL A0 //pin Analog YL-69
#define Lampu D2 //Pin LED
 
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL); //Aktifkan Fungsi Thinger IO
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //Aktifkan Fungsi HCSR-04
int outputValue; //Variabel untuk Output YL-69
 
void setup() {
pinMode(Lampu, OUTPUT); //Set LED sebagai Output
digitalWrite(Lampu, LOW); //Set nilai LED = 0
 
thing.add_wifi(SSID, SSID_PASSWORD); //Mencoba terhubung ke Wi-Fi dan Thinger IO
 
//Fungsi untuk Kontrol Lampu dengan Topik Lampu
thing["Lampu"] << [](pson& in){
if(in.is_empty()){
in = (bool) digitalRead(Lampu);
}
else{
digitalWrite(Lampu, in ? HIGH : LOW);
}
};
 
//Mengirim Pembacaan Sensor HCSR-04 ke Thinger IO
thing["HCSR04"] >> [](pson& out){
out["Jarak"] = sonar.ping_cm();
};
 
//Mengirim Pembacaan Sensor YL-69 ke Thinger IO
thing["Soil_Moisture"] >> [](pson& out){
outputValue = analogRead(pinYL);
outputValue = map(outputValue, 1024, 250, 0, 100);
out["Moisture"] = outputValue;
};
}
 
void loop() {
//Menjalankan fungsi Thinger IO
thing.handle();
}
