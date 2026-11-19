#include <Arduino.h>
#include <MPU9250.h>
#include <Wire.h>

#define MPU9250_IMU_ADDRESS 0x68 
#define MAGNETIC_DECLINATION 1.63 // To be defined by user 
#define INTERVAL_MS_PRINT 100
#define RX_PIN 18
#define TX_PIN 17
#define SDA_PIN 8
#define SCL_PIN 9

unsigned long lastPrintMillis = 0; 

struct TelemetryPacket {
	float pitch = 0.f;
	float roll = 0.f;
	unsigned long timestamp = 0L;
};

MPU9250 mpu;

void setup() {
  	Serial.begin(115200);
  	Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  	Wire.begin(SDA_PIN, SCL_PIN);

	MPU9250Setting setting;
	// Sample rate must be at least 2x DLPF rate 
	setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
	setting.gyro_fs_sel = GYRO_FS_SEL::G1000DPS;
	setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
	setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_250HZ;
	setting.gyro_fchoice = 0x03;
	setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
	setting.accel_fchoice = 0x01;
	setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;
	mpu.setup(MPU9250_IMU_ADDRESS, setting);
	mpu.setMagneticDeclination(MAGNETIC_DECLINATION);
	mpu.selectFilter(QuatFilterSel::MADGWICK);
	mpu.setFilterIterations(15);
	Serial.println("Please leave the device still on the flat plane."); 
	Serial.println("Calibrating...");
	mpu.calibrateAccelGyro();
	Serial.println("Ready!");
}

void loop() {
	mpu.update();
	unsigned long now = millis(); 
	if (mpu.update() && now - lastPrintMillis > INTERVAL_MS_PRINT) { 
		Serial.print(mpu.getPitch());
		Serial.print(' ');
		Serial.println(mpu.getRoll());   

		TelemetryPacket packet;
		packet.pitch = mpu.getPitch();
		packet.roll = mpu.getRoll();
		packet.timestamp = now;

		Serial1.write(reinterpret_cast<const uint8_t*>(&packet), sizeof(packet));

		lastPrintMillis = now; 
	}
}
