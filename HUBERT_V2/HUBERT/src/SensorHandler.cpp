#include "SensorHandler.h"

SensorHandler::SensorHandler()
    : filteredRoll(0), filteredPitch(0), filteredYaw(0),
      complementaryEnabled(false), alpha(0.98),
      lowPassEnabled(false), lowPassCutoff(5.0),
      highPassEnabled(false), highPassCutoff(0.1) {}

void SensorHandler::begin(DatabaseTool* db) {
    mpu6050.begin(db);

    // Lade Filterparameter aus der JSON
    complementaryEnabled = db->get<bool>("sensors/mpu6050/filters/complementary/enabled", false);
    alpha = db->get<float>("sensors/mpu6050/filters/complementary/alpha", 0.98);

    lowPassEnabled = db->get<bool>("sensors/mpu6050/filters/lowpass/enabled", false);
    lowPassCutoff = db->get<float>("sensors/mpu6050/filters/lowpass/cutoffFrequency", 5.0);

    highPassEnabled = db->get<bool>("sensors/mpu6050/filters/highpass/enabled", false);
    highPassCutoff = db->get<float>("sensors/mpu6050/filters/highpass/cutoffFrequency", 0.1);
}

void SensorHandler::update() {
    sensors_event_t accel, gyro, temp;
    mpu6050.getEvent(&accel, &gyro, &temp);

    float dt = 0.01;  
    
    // Komplementärfilter anwenden
    if (complementaryEnabled) {
        applyComplementaryFilter(filteredRoll, filteredPitch, filteredYaw, accel, gyro, dt);
        // Serial.println("Komplementärfilter angewendet.");
    }
    // Low-Pass-Filter anwenden
    else if (lowPassEnabled) {
        applyLowPassFilter(filteredRoll, filteredRoll, lowPassCutoff, dt);
        applyLowPassFilter(filteredPitch, filteredPitch, lowPassCutoff, dt);
        applyLowPassFilter(filteredYaw, filteredYaw, lowPassCutoff, dt);
        // Serial.println("Low-Pass-Filter angewendet.");
    }
    // High-Pass-Filter anwenden
    else if (highPassEnabled) {
        applyHighPassFilter(filteredRoll, filteredRoll, highPassCutoff, dt);
        applyHighPassFilter(filteredPitch, filteredPitch, highPassCutoff, dt);
        applyHighPassFilter(filteredYaw, filteredYaw, highPassCutoff, dt);
        // Serial.println("High-Pass-Filter angewendet.");
    }
    // Fallback: Rohdaten direkt verwenden, falls kein Filter aktiv ist
    else {
        filteredRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180 / PI;
        filteredPitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180 / PI;
        filteredYaw += gyro.gyro.z * dt; // Nur Gyro-Daten für Yaw
        // Serial.println("Rohdaten direkt verwendet (kein Filter aktiv).");
    }

    // Debugging der gefilterten Werte
    // Serial.printf("Filtered Data -> Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", filteredRoll, filteredPitch, filteredYaw);
}

void SensorHandler::getFilteredData(float& roll, float& pitch, float& yaw) {
    roll = filteredRoll;
    pitch = filteredPitch;
    yaw = filteredYaw;
    // Serial.printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", roll, pitch, yaw);

}

void SensorHandler::applyComplementaryFilter(float& roll, float& pitch, float& yaw, const sensors_event_t& accel, const sensors_event_t& gyro, float dt) {
    // Accelerometer-Winkel berechnen
    float accelRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180 / PI;
    float accelPitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180 / PI;

    // Gyroskop-Winkelintegration
    float gyroRoll = roll + gyro.gyro.x * dt;
    float gyroPitch = pitch + gyro.gyro.y * dt;
    float gyroYaw = yaw + gyro.gyro.z * dt;

    // Komplementärfilter
    roll = alpha * gyroRoll + (1 - alpha) * accelRoll;
    pitch = alpha * gyroPitch + (1 - alpha) * accelPitch;
    yaw = gyroYaw;  // Nur Gyroskop für Yaw
}

void SensorHandler::applyLowPassFilter(float& value, float& previousValue, float cutoffFrequency, float dt) {
    float rc = 1.0 / (2 * PI * cutoffFrequency);
    float alpha = dt / (rc + dt);
    value = previousValue + alpha * (value - previousValue);
    previousValue = value;
}

void SensorHandler::applyHighPassFilter(float& value, float& previousValue, float cutoffFrequency, float dt) {
    float rc = 1.0 / (2 * PI * cutoffFrequency);
    float alpha = rc / (rc + dt);
    value = alpha * (previousValue + value - previousValue);
    previousValue = value;
}
