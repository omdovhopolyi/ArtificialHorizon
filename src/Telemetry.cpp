#include "Telemetry.hpp"
#include "Config.hpp"
#include "TelemetryData.hpp"
#include <Wire.h>

namespace ah
{
    void Telemetry::Init()
    {
        Wire.begin(Config::PinSDA, Config::PinSCL);

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

        _mpu.setup(Config::MPU9250Address, setting);
        _mpu.selectFilter(QuatFilterSel::MADGWICK);
        _mpu.setFilterIterations(Config::FilterIterations);

        Serial.println("Please leave the device still on the flat plane."); 
        Serial.println("Calibrating...");
        _mpu.calibrateAccelGyro();
        Serial.println("Ready!");
    }

    void Telemetry::Update()
    {
        _mpu.update();
    }

    void Telemetry::HandleTelemetry(TelemetryData& data)
    {
        const float pitch = _mpu.getPitch();
        const float roll = _mpu.getRoll();

        Serial.print(pitch);
        Serial.print(' ');
        Serial.println(roll);

        data.pitch = pitch;
        data.roll = roll;
        data.timestamp = millis();
    }
}
