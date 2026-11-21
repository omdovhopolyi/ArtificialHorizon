#include <Arduino.h>
#include <MPU9250.h>
#include <Wire.h>
#include <LittleFS.h>
#include <LovyanGFX.hpp>

#define MPU9250_IMU_ADDRESS 0x68 
#define MAGNETIC_DECLINATION 1.63 // To be defined by user 
#define INTERVAL_MS_PRINT 100
#define RX_PIN 18
#define TX_PIN 17
#define SDA_PIN 8
#define SCL_PIN 9

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7735S _panel;
    lgfx::Bus_SPI _bus;

public:
    LGFX()
    {
        auto bus_cfg = _bus.config();

        bus_cfg.spi_host = SPI2_HOST;
        bus_cfg.spi_mode = 0;
        bus_cfg.freq_write = 27000000;
        bus_cfg.spi_3wire = true;

        bus_cfg.pin_sclk = 12;
        bus_cfg.pin_mosi = 11;
        bus_cfg.pin_miso = -1;
        bus_cfg.pin_dc = 7;

        _bus.config(bus_cfg);
        _panel.setBus(&_bus);

        auto panel_cfg = _panel.config();

        panel_cfg.pin_cs = 10;
        panel_cfg.pin_rst = 6;

        panel_cfg.panel_width = 128;
        panel_cfg.panel_height = 128;

        panel_cfg.offset_x = 0;
        panel_cfg.offset_y = 0;

        panel_cfg.invert = false;
        panel_cfg.rgb_order = false;

        _panel.config(panel_cfg);
        setPanel(&_panel);
    }
};

LGFX lcd;
LGFX_Sprite sprite(&lcd);

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
	

	if (!LittleFS.begin(false)) {
		Serial.println("Failed to init little fs");
		while (true) {};
	}
	else {
		Serial.println("Little fs initialized successfuly");
	}

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
	//mpu.calibrateAccelGyro();
	Serial.println("Ready!");

	lcd.init();
    lcd.setRotation(0);
	lcd.setColorDepth(8);

	

    if (!sprite.createSprite(196, 197))
    {
        Serial.println("Sprite allocation failed!");
        while (true) {};
    }

	sprite.setColorDepth(8);

    // sprite.fillScreen(TFT_BLACK);

    // sprite.setTextColor(TFT_WHITE);
    // sprite.setTextSize(2);
    // sprite.drawString("Hello!", 20, 50);

// lcd.drawPngFile(
//     LittleFS,
//     "/ah_back_20.png",
//     0,
//     0,
//     0,
//     0,
//     1.0f,
//     1.0f,
//     0,
//     100
// );

// sprite.setColorDepth(16);
// sprite.createSprite(100, 100);

// sprite.fillScreen(TFT_RED);
// sprite.fillRect(0, 0, 50, 50, TFT_GREEN);
// sprite.fillRect(50, 50, 50, 50, TFT_BLUE);

// sprite.pushSprite(0, 0);

//prite.drawPngFile("/ah_back_2.png");

	const bool result = sprite.drawPngFile(
		LittleFS,
		"/ah_back_20.png",
		0,
		0
	);

// 	Serial.printf("PNG result: %d\n", result);

// 	sprite.pushRotateZoom(
// 		0,     // Destination center X
// 		0,     // Destination center Y
// 		0.f,  // Rotation angle
// 		0.3f,   // Scale X
// 		0.3f    // Scale Y
// )	;	

    sprite.pushSprite(0, 0);

	// lcd.drawPngFile(
    //     LittleFS,
    //     "/ah_back_20.png",
    //     0,
    //     0,
	// 	128,
	// 	128
    // );

    // lcd.fillScreen(TFT_GREEN);
	// lcd.setTextColor(TFT_WHITE);
    // lcd.setTextSize(2);
    // lcd.drawString("Hello!", 20, 50);
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

	//sprite.pushSprite(0, 0);

}
