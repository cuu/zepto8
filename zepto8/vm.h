//
//  ZEPTO-8 — Fantasy console emulator
//
//  Copyright © 2016 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <lol/engine.h>

#include "zepto8.h"
#include "cart.h"
#include "code-fixer.h"

namespace z8
{

using lol::array;
using lol::u8vec4;

class vm : public lol::LuaLoader,
           public lol::LuaObject,
           public lol::WorldEntity
{
public:
    vm();
    virtual ~vm();

    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, lol::Scene &scene);

    void load(char const *name)
    {
        // FIXME: we only know PNG for now
        m_cart.load_png(name);

        // Copy everything into memory up to the code
        memcpy(m_memory.data(), m_cart.get_data().data(), OFFSET_CODE);
    }

    void run()
    {
        // Fix code
        code_fixer fixer(m_cart.get_code());
        lol::String new_code = fixer.fix();

        //msg::info("Fixed cartridge code:\n%s\n", m_code.C());
        //printf("%s", m_code.C());
        // FIXME: not required yet because we inherit from LuaLoader
        //lol::LuaLoader lua;

        // Execute cartridge code
        ExecLuaCode(new_code.C());

        // Initialize cartridge code
        ExecLuaCode("_init()");
    }

    static const lol::LuaObjectLib* GetLib();
    static vm* New(lol::LuaState* l, int arg_nb);

private:
    // System
    static int cartdata(lol::LuaState *l);
    static int reload(lol::LuaState *l);
    static int peek(lol::LuaState *l);
    static int poke(lol::LuaState *l);

    // I/O
    static int btn(lol::LuaState *l);
    static int btnp(lol::LuaState *l);

    // Text
    static int cursor(lol::LuaState *l);
    static int print(lol::LuaState *l);

    // Maths
    static int max(lol::LuaState *l);
    static int min(lol::LuaState *l);
    static int mid(lol::LuaState *l);
    static int flr(lol::LuaState *l);
    static int cos(lol::LuaState *l);
    static int sin(lol::LuaState *l);
    static int atan2(lol::LuaState *l);
    static int sqrt(lol::LuaState *l);
    static int abs(lol::LuaState *l);
    static int sgn(lol::LuaState *l);

    static int rnd(lol::LuaState *l);
    static int srand(lol::LuaState *l);

    static int band(lol::LuaState *l);
    static int bor(lol::LuaState *l);
    static int bxor(lol::LuaState *l);
    static int bnot(lol::LuaState *l);
    static int shl(lol::LuaState *l);
    static int shr(lol::LuaState *l);

    // Graphics
    static int camera(lol::LuaState *l);
    static int circ(lol::LuaState *l);
    static int circfill(lol::LuaState *l);
    static int clip(lol::LuaState *l);
    static int cls(lol::LuaState *l);
    static int color(lol::LuaState *l);
    static int line(lol::LuaState *l);
    static int map(lol::LuaState *l);
    static int mget(lol::LuaState *l);
    static int mset(lol::LuaState *l);
    static int pal(lol::LuaState *l);
    static int pget(lol::LuaState *l);
    static int pset(lol::LuaState *l);
    static int rect(lol::LuaState *l);
    static int rectfill(lol::LuaState *l);
    static int sget(lol::LuaState *l);
    static int sset(lol::LuaState *l);
    static int spr(lol::LuaState *l);
    static int sspr(lol::LuaState *l);

    // Sound
    static int music(lol::LuaState *l);

private:
    int getpixel(int x, int y);
    void setpixel(int x, int y, int color);

    int getspixel(int x, int y);
    void setspixel(int x, int y, int color);

private:
    array<uint8_t> m_memory;
    array<u8vec4> m_screen;
    lol::Image m_font;
    cart m_cart;

    uint8_t m_color;
    lol::ivec2 m_camera, m_cursor;
    uint8_t m_palette[2][16];

    lol::Camera *m_scenecam;
    lol::TileSet *m_tile;
};

} // namespace z8

