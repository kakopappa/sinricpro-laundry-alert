#ifndef _WASHINGMACHINE_H_
#define _WASHINGMACHINE_H_

#include <SinricProDevice.h>
#include <Capabilities/PushNotification.h>
#include <Capabilities/ModeController.h>

class WashingMachine 
: public SinricProDevice
, public PushNotification<WashingMachine>
, public ModeController<WashingMachine> {
  friend class PushNotification<WashingMachine>;
  friend class ModeController<WashingMachine>;
public:
  WashingMachine(const String &deviceId) : SinricProDevice(deviceId, "WashingMachine") {};
};

#endif
