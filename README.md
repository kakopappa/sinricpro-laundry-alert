# sinricpro-laundry-alert
Laundry alert push notification using ESP8266/ESP32 + SW-420 vibration sensor from Sinric Pro.

SW-420 vibration sensor is connected to D2 PIN.  

How it works:
    Vibration sensor detects vibration and adds to samples. When there's enough possitive samples (because sensor goes 111001111) sketch send a push notification to my mobile via Sinric Pro IOT platform. 
    
During my testing i found out it's only possible to detect the spin cycle. My LG washing machine does not make large enough vibrations to detect Wash/Rinse cycle (well done LG).

You can use the following template to create a custom device in Sinric Pro

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

