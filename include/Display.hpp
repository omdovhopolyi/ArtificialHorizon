#ifndef DISPLAY_H
#define DISPLAY_H

#include <LittleFS.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
public:
    LGFX();

private:
    lgfx::Panel_ST7735S _panel;
    lgfx::Bus_SPI _bus;
};

class Display
{
public:
    Display();

    void Init();

    void Update(float pitch, float roll);

private:
    LGFX _lcd;
    LGFX_Sprite _spriteBackground;
    LGFX_Sprite _spriteTarget;
    LGFX_Sprite _buffer;
};

#endif