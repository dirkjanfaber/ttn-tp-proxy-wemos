# ttn-tp-proxy-wemos
Use a Wemos to proxy between the things network and thingspeak. 

This code can be compiled using https://platform.io/

Install the `ArduinoJSon` libary first:

```
pio lib install 64
```

Then compile and upload the code to a Wemos D1. 

Next, configure your home router to forward packages a port of your choise to port 80 of the wemos. Last, configure the things network http integration to point to http://$yourip:$yourport/ttn
