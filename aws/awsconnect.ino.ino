#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
 // This should contain your WiFi credentials and AWS IoT credentials

#include <pgmspace.h>
 
#define SECRET
 
const char WIFI_SSID[] = "Sudharsanan";               //TAMIM2.4G
const char WIFI_PASSWORD[] = "55555555";           //0544287380
 
#define THINGNAME "ESP8266"
 
int8_t TIME_ZONE = -5; //NYC(USA): -5 UTC
 
const char MQTT_HOST[] = "a1bojsxwypdjt3-ats.iot.eu-north-1.amazonaws.com";
 
 
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
 
// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAN+Qz5eT3Cov5NGGRuxTdTOz40lZMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA5MjcxNDMy
MzZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDTkkf666oiBd4nayjP
xpXeqNmmuhJkc5lwoC2L41TtCwSkWocTY/F/T1BsB06+oD17twS3M5xT2upm/XVQ
7MUsVfs6bZTUdisqfDWEUytsiXiGLEFQq0Um1ExZYdm8L/5lWCDHYVBqZsDemA8K
xid77xbHv317GwNM0eQBJVR6Oz25Ih5GSb/mIV2KQKsR4+1P11nSHBBXvdCy6xnH
obAxuPb0Vs4LgoDNQRtBvTblPFKAWIwD91lncbuHPTLFhPMuf+ugplC+EUziap0b
KBt8Cn9+tSzGoi/slghg4Sy1gJuzkitqAVYQcyNsDtLCov6Yz2uybPmA+Q73eo2d
XbKnAgMBAAGjYDBeMB8GA1UdIwQYMBaAFJK63i8eT1BM8BmxSV/cDCIQewrgMB0G
A1UdDgQWBBQIDlPkKIBJdgEyaVrXa9WhBgDO1TAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAshpawRhRdY/+XU+CYuxrCRPx
N5DPOLvUeuhfJdFaFRUf6q3n/MRZInl2lDtxZQqtqKrYj7pwpQPGFvUBPOkvdCsZ
dZWh0PAzIMv4Kb71agqrMCuBGo4pIb5aHlh/wb1GUWmoIebwzJhBZSNoaSFpSTZV
5/d+AVI1DXMLn24SwmllYUMSqW1NKMaLJApQ1smerCcOz1RRJwf88EQD64buWyfo
jTTGdVVjpQfEBuPrgnelj233y0543bD8LQVPJZOYiUwgD+eB/RhCMVs4aL09gsdo
qnsizXsPJYqdHKoSH/mou+iDxUFociagbviprQ8E21ovIQYnDRUtGemU5ND48Q==
-----END CERTIFICATE-----
)KEY";
 
 
// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA05JH+uuqIgXeJ2soz8aV3qjZproSZHOZcKAti+NU7QsEpFqH
E2Pxf09QbAdOvqA9e7cEtzOcU9rqZv11UOzFLFX7Om2U1HYrKnw1hFMrbIl4hixB
UKtFJtRMWWHZvC/+ZVggx2FQambA3pgPCsYne+8Wx799exsDTNHkASVUejs9uSIe
Rkm/5iFdikCrEePtT9dZ0hwQV73QsusZx6GwMbj29FbOC4KAzUEbQb025TxSgFiM
A/dZZ3G7hz0yxYTzLn/roKZQvhFM4mqdGygbfAp/frUsxqIv7JYIYOEstYCbs5Ir
agFWEHMjbA7SwqL+mM9rsmz5gPkO93qNnV2ypwIDAQABAoIBAQDCrLQc12H54mpS
3RB1duYKNlXnSwmHcrL6Zfji4fAQGXZRAJePqQPawBFhBaxYTTulrXbiV9P9eM78
qzWam/Kl5AvB7Hjosj7v3dCkjJ7FRQ0ocGo9HwsHOotyJ05hEcxEVj37F7kV3qyT
ZVlZ8ZD9Qbc1XybH8sgrlGn2//FmcZXv5OHFNqZsIueQt6aZjlVf6Pd4W5Cu4fte
VA2wq7FkYT+k7kkhj00i6TPdjbITBxtakh19zCDF1Z2VUwtImJyBmcnyl9BH2ANJ
nhtb8HVxjPRvM+ZsJNBps44inTf+GgdDwH63Y73OLgo5m+n1SVdiW9qPhY7b7+de
XYSA+L+5AoGBAO1O7koodVFFQ0eupG9L2pbDchT3yq+O02Z9k/8nz79qirE4M79k
3OL4DqhmApUyOwWhue4Wn64UkSuZFd+9PNspHlSj2QBWTnOS+JxMhL113qtvq8dE
hk5juZnJjvt7UonemnBSSIPzaQQYaEycEbxFfJ5AT1WN/ZAM9VhTz3pLAoGBAOQ8
ZJlkUe5eQ0NfX22AVZhswjmghfAk78nL/j0SPXaiT/39Yur94EUYCWggnkMp1jSR
KdWr7NPATx1cPhFUmk+MNnBgIxopR1Ms6KHE4Q/CUFQyL473ihbI2Xpe8noPsHtE
b4iO1j1GrFOanyKKLjCI9f1L38+xsjAYAz/hn2+VAoGBAK32fdLZQ993AfJLgXmh
RrXcwwXB/NCjyC9LAbI70hzFkrSFudGltBOiusQSO/R5ZTEktLNn3Fj19NHcIF/r
TvBufTCZCFwp6Mrqw9t4cJqfDMPqUif7AOX4TzgAvRN3/TLaf6a1ezlXt/VrmxIG
Gy5EVi2AgWf7dhUimrzMe71vAoGBALXkw6/T2dAE48pfyee07YKKuse+fMAET+4d
W0NEtCNF/EZJmHn7Vq76FxUbWcXhI8vFHPgjCp0/mE2n4/df25MmBYTNuWvyFdvd
4JB8HXm3vNk9HdXsxeqeq2PxI4HP2zCCO/kzDN7M4i3G/qhCQ8l+fb0eqQIXdhEK
MhzJLY15AoGAf/Jt/0gOfY1QosvjHQtqDTyZ2UaJDEyVyFVKSKQWzoFy+YDJTiN9
D9y3ezjZT+d/7PSVbzpSwA4ZjPBf3XXdh+rlWFYMM6n0AgMYUSEJLJpwzICp9XPo
c6s09BOlGjeGt6A5NoE/aSVlPE0dHJOT3CQrrKw4Y3GOyctBXwf4Yqs=
-----END RSA PRIVATE KEY-----

)KEY";


// Ultrasonic Sensor Pins
#define trigPin D1  // Trigger pin for ultrasonic sensor
#define echoPin D2  // Echo pin for ultrasonic sensor

float distance;  // Measured distance from the ultrasonic sensor
unsigned long lastMillis = 0;

#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"

WiFiClientSecure net;
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);
time_t now;
time_t nowish = 1510592825;

void NTPConnect(void) {
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void messageReceived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connectAWS() {
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(1000);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

// Ultrasonic sensor reading function
float getDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance (speed of sound is 34300 cm/s)
  float distance = duration * 0.034 / 2;
  return distance;
}

// Publish sensor data to AWS IoT Core
void publishMessage() {
  StaticJsonDocument<200> doc;
  
  doc["spaceID"] = "1";
  doc["status"] = distance < 20 ? "occupied" : "available";  // Example threshold: if distance < 20 cm, parking is occupied
  doc["time"] = millis();
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // Serialize JSON payload

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);  // Publish to AWS IoT
}

void setup() {
  Serial.begin(115200);

  // Set up ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  connectAWS();
}

void loop() {
  distance = getDistance();  // Get distance from the ultrasonic sensor

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Reconnect to AWS IoT if disconnected
  if (!client.connected()) {
    connectAWS();
  } else {
    client.loop();  // Maintain MQTT connection
    if (millis() - lastMillis > 5000) {  // Publish every 5 seconds
      lastMillis = millis();
      publishMessage();
    }
  }

  delay(2000);  // Wait 2 seconds between readings
}