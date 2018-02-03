# ttn-tp-proxy-wemos
Use a Wemos to proxy between the things network and thingspeak. 

This code can be compiled using https://platform.io/

Install the `ArduinoJSon` libary first:

```
pio lib install 64
```

Then compile and upload the code to a Wemos D1. 

You can test it locally by navigating with your browser to http://$wemosip/

A simple test is to send some data to thingspeak:

```
read -p "Wemos D1 ip: " wemos_ip
read -p "Thingspeak API key: "  api_key
curl -d '{ "api_key": "${api_key}", "field1": 22 }' \
  -H "Content-Type: application/json" -X POST \
  http://${wemos_ip}/ttn

```

Last, configure your home router to forward packages a port of your choise to port 80 of the Wemos. Last, configure the things network http integration to point to http://$yourip:$yourport/ttn
