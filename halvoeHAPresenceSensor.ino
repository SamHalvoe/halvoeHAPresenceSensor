#include <SoftwareSerial.h>
#include <mmwave_for_xiao.h>

SoftwareSerial sensorSerial(D2, D3);
Seeed_HSP24 presenceSensor(sensorSerial, Serial);
Seeed_HSP24::RadarStatus radarStatus;

void setup()
{
  Serial.begin(9600);
  while (not Serial) { delay(100); }
  Serial.println("Serial is ready.");

  Serial.println("Begin sensor serial...");
  sensorSerial.begin(9600);
  while (not sensorSerial) { delay(100); }
  Serial.println("Sensor Serial is ready.");
  presenceSensor.disableEngineeringModel();

  Serial.println("Leaving setup.");
}

int retryCount = 0;
const int MAX_RETRIES = 10; // Maximum number of retries to prevent infinite loops

// Parsing the acquired radar status
const char* targetStatusToString(Seeed_HSP24::TargetStatus status)
{
  switch (status) {
    case Seeed_HSP24::TargetStatus::NoTarget:
      return "NoTarget";
    case Seeed_HSP24::TargetStatus::MovingTarget:
      return "MovingTarget";
    case Seeed_HSP24::TargetStatus::StaticTarget:
      return "StaticTarget";
    case Seeed_HSP24::TargetStatus::BothTargets:
      return "BothTargets";
    default:
      return "Unknown";
  }
}

void loop()
{
  do {
    radarStatus = presenceSensor.getStatus();
    retryCount++;
  } while (radarStatus.targetStatus == Seeed_HSP24::TargetStatus::ErrorFrame && retryCount < MAX_RETRIES);

  // Parses radar status and prints results from debug serial port
  if (radarStatus.targetStatus != Seeed_HSP24::TargetStatus::ErrorFrame)
  {
    Serial.print("Status: " + String(targetStatusToString(radarStatus.targetStatus)) + " ---- ");
    Serial.println("Distance: " + String(radarStatus.distance) + "  Mode: " + String(radarStatus.radarMode));
  }

  delay(200);
}
