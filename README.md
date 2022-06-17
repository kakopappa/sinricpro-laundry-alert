# Sinric Pro laundry alert

Laundry alert push notification using ESP8266/ESP32 + SW-420 vibration sensor from Sinric Pro.

![esp-32-esp8266-laundry-alert](https://github.com/kakopappa/sinricpro-laundry-alert/blob/main/images/Testing.png?raw=true)


**Parts**

1. ESP8266 or ESP32. (Tested on ESP8266)
2. SW-420 vibration sensor. (connected to D2 PIN)

**How does it work?**

Vibration sensor detects vibrations and adds to samples list. When there's enough possitive/negative samples (because sensor goes 111001111) sketch send a push notification to my mobile via Sinric Pro IOT platform that laundry is done and it's time to pickup. 

![sinricpro-push-laundry-alert](https://github.com/kakopappa/sinricpro-laundry-alert/blob/main/images/Screenshot_1655484018.png?raw=true)
  
During my testing I found out it's only possible to detect only the spin cycle. My LG washing machine does not make large enough vibrations to detect Wash/Rinse cycle (well done LG).

You can use the following template to create a custom device in Sinric Pro
 
 ![sinricpro-push-laundry-alert](https://github.com/kakopappa/sinricpro-laundry-alert/blob/main/images/Screenshot%202022-06-17%20234729.png?raw=true)

{
  "name": "WashingMachine",
  "description": "WashingMachine",
  "deviceTypeId": "5ff0b112994fd31b7d5e8065",
  "capabilities": [
    {
      "id": "62a707bd9e2f39090569e2d3"
    },
    {
      "id": "5ff0b41b994fd31b7d5e8961",
      "mode": {
        "nonControllable": false,
        "instanceId": "modeInstance1",
        "modeValues": [
          "idle",
          "spinning"
        ],
        "locale": "en-US",
        "modeName": "state"
      }
    }
  ]
}

