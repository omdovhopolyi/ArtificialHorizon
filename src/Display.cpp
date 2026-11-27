#include "Display.hpp"
#include "Config.hpp"
#include "ErrorHandler.hpp"
#include "TelemetryData.hpp"

namespace ah
{
    LGFX::LGFX()
    {
        auto bus_cfg = _bus.config();

        bus_cfg.freq_write = 27000000;

        bus_cfg.pin_sclk = Config::PinSCLK;
        bus_cfg.pin_mosi = Config::PinMOSI;
        bus_cfg.pin_dc = Config::PinDC;

        _bus.config(bus_cfg);
        _panel.setBus(&_bus);

        auto panel_cfg = _panel.config();

        panel_cfg.pin_cs = Config::PinCS;
        panel_cfg.pin_rst = Config::PinRST;

        panel_cfg.panel_width = Config::ScreenWidth;
        panel_cfg.panel_height = Config::ScreenHeight;

        _panel.config(panel_cfg);
        setPanel(&_panel);
    }

    Display::Display()
        : _spriteBackground(&_lcd)
        , _spriteTarget(&_lcd)
        , _buffer(&_lcd)
    {}

    void Display::Init()
    {
        if (!LittleFS.begin(false)) {
            ErrorHandler("Failed to init little fs");
        }
        else {
            Serial.println("Little fs initialized successfuly");
        }

        _lcd.init();
        _lcd.setRotation(Config::ScreenOrientation);
        _lcd.setColorDepth(Config::ScreenColorDepth);

        if (!_spriteBackground.createSprite(Config::BackgroundSpriteWidth, Config::BackgroundSpriteHeight)) {
            ErrorHandler("Sprite allocation failed!");
        }
        if (!_spriteTarget.createSprite(Config::ScreenWidth, Config::ScreenHeight)) {
            ErrorHandler("Target sprite allocation failed");
        }
        if (!_buffer.createSprite(Config::ScreenWidth, Config::ScreenHeight)) {
            ErrorHandler("Buffer sprite allocation failed");
        }

        _spriteBackground.setColorDepth(Config::BackgroundSpriteColorDepth);
        _spriteTarget.setColorDepth(Config::TargetSpriteColorDepth);

        bool result = _spriteBackground.drawPngFile(LittleFS, "/ah_back_20.png");
        if (!result) {
            ErrorHandler("Can not create 'ah_back_20.png' sprite");
        }

        result = _spriteTarget.drawPngFile(LittleFS, "/ah_target.png");
        if (!result) {
            ErrorHandler("Can not create 'ah_target.png' sprite");
        }

        _spriteBackground.setPivot(_spriteBackground.width() / 2, _spriteBackground.height() / 2);
    }

    void Display::HandleTelemetry(TelemetryData& data)
    {
        UpdateScreen(data.pitch, data.roll);
    }

    void Display::UpdateScreen(float pitch, float roll)
    {
        if (!_lcd.dmaBusy()) {
            _lcd.startWrite();
            pitch = pitch / 10.f * Config::PixPer10Degrees;
            pitch = std::max(std::min(pitch, Config::PitchMax), -Config::PitchMax);
            _spriteBackground.setPivot(_spriteBackground.getPivotX(), _spriteBackground.height() / 2 + pitch);
            _spriteBackground.pushRotateZoom(&_buffer, Config::ScreenWidth / 2, Config::ScreenHeight / 2, roll, Config::SpriteBackScale, Config::SpriteBackScale);
            _spriteTarget.pushSprite(&_buffer, 0, 0, TFT_BLACK);
            _buffer.pushSprite(0, 0);
            _lcd.endWrite();
        }
    }
}
