# ttn-tp-proxy-wemos

The http integration of the things network is limited in the sence that it does not allow to only forward the `payload_fieds` (  https://www.thethingsnetwork.org/docs/applications/http/#uplink), so you can pass it directly to [Thingspeak](https://thingspeak.com/). In order to fix this, I used a Wemos D1 to do this filtering/proxy between the things network and thingspeak. 

This code can be compiled using https://platform.io/

Install the `ArduinoJSon` libary first:

```
pio lib install 64
```

Then compile and upload the code to a Wemos D1. You can test the functionality by navigating with your browser to http://$wemosip/

The code forwards only the `payload_fields` part of incomming json posts to `/ttn` to Thingspeak. 

A simple test is to send some data to the Wemos. Make sure you have created a channel on Thingspeak first and have the *Write API Key* around. 

```
read -p "Wemos D1 ip: " wemos_ip
read -p "Thingspeak API key: "  api_key
read -p "Value to post: " value

DATA=$(cat <<__EOT__
{ "foo" : "bar ",
  "payload_fields" : {
    "api_key" : "${api_key}",
    "field1" : "${value}"
  } }
__EOT__
)

curl \
  --header "Content-Type: application/json" \
  --request POST \
  --data "${DATA}" \
  http://${wemos_ip}/ttn

```

If this works, configure your home router to forward packages a port of your choise to port 80 of the Wemos. Last, configure the things network http integration to point to http://$yourip:$yourport/ttn
