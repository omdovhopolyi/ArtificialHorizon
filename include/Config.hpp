#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

struct Config
{
    static constexpr unsigned long UpdateInterval = 100L;

    static constexpr uint8_t PinRX = 18;
    static constexpr uint8_t PinTX = 17;
    static constexpr unsigned long UARTBaud = 115200;

    static constexpr uint8_t PinSDA = 8;
    static constexpr uint8_t PinSCL = 9;

    static constexpr uint16_t PixPer10Degrees = 10;
    static constexpr float PitchMax = 45.f;
    static constexpr float SpriteBackScale = 1.5f;
    
    static constexpr uint8_t PinSCLK = 12;
    static constexpr uint8_t PinMOSI = 11;
    static constexpr uint8_t PinDC = 7;

    static constexpr uint8_t PinCS = 10;
    static constexpr uint8_t PinRST = 6;

    static constexpr uint16_t ScreenWidth = 128;
    static constexpr uint16_t ScreenHeight = 128;
    static constexpr uint8_t ScreenColorDepth = 16;
    static constexpr uint8_t ScreenOrientation = 0;

    static constexpr uint16_t BackgroundSpriteWidth = 196;
    static constexpr uint16_t BackgroundSpriteHeight = 197;
    static constexpr uint8_t BackgroundSpriteColorDepth = 16;
    static constexpr uint8_t TargetSpriteColorDepth = 16;

    static constexpr uint8_t MPU9250Address = 0x68;
    static constexpr uint8_t FilterIterations = 15;
};

#endif