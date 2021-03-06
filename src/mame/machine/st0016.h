// license:BSD-3-Clause
// copyright-holders:Tomasz Slanina, David Haywood
/* ST0016 - CPU (z80) + Sound + Video */

#pragma once

#ifndef __ST0016_CPU__
#define __ST0016_CPU__

#include "emu.h"
#include "cpu/z80/z80.h"
#include "sound/st0016.h"

#define ISMACS  (m_game_flag & 0x80)
#define ISMACS1 (((m_game_flag & 0x180) == 0x180))
#define ISMACS2 (((m_game_flag & 0x180) == 0x080))


#define ST0016_MAX_SPR_BANK   0x10
#define ST0016_MAX_CHAR_BANK  0x10000
#define ST0016_MAX_PAL_BANK   4

#define ST0016_SPR_BANK_SIZE  0x1000
#define ST0016_CHAR_BANK_SIZE 0x20
#define ST0016_PAL_BANK_SIZE  0x200

#define UNUSED_PEN 1024

#define ST0016_SPR_BANK_MASK  (ST0016_MAX_SPR_BANK-1)
#define ST0016_CHAR_BANK_MASK (ST0016_MAX_CHAR_BANK-1)
#define ST0016_PAL_BANK_MASK  (ST0016_MAX_PAL_BANK-1)


typedef device_delegate<uint8_t (void)> st0016_dma_offs_delegate;
#define ST0016_DMA_OFFS_CB(name)  uint8_t name(void)

#define MCFG_ST0016_DMA_OFFS_CB(_class, _method) \
	st0016_cpu_device::set_dma_offs_callback(*device, st0016_dma_offs_delegate(&_class::_method, #_class "::" #_method, downcast<_class *>(owner)));


class st0016_cpu_device : public z80_device, public device_gfx_interface
{
public:
	st0016_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t);

	static void set_dma_offs_callback(device_t &device, st0016_dma_offs_delegate callback) { downcast<st0016_cpu_device &>(device).m_dma_offs_cb = callback; }

	DECLARE_WRITE8_MEMBER(st0016_sprite_bank_w);
	DECLARE_WRITE8_MEMBER(st0016_palette_bank_w);
	DECLARE_WRITE8_MEMBER(st0016_character_bank_w);
	DECLARE_READ8_MEMBER(st0016_sprite_ram_r);
	DECLARE_WRITE8_MEMBER(st0016_sprite_ram_w);
	DECLARE_READ8_MEMBER(st0016_sprite2_ram_r);
	DECLARE_WRITE8_MEMBER(st0016_sprite2_ram_w);
	DECLARE_READ8_MEMBER(st0016_palette_ram_r);
	DECLARE_WRITE8_MEMBER(st0016_palette_ram_w);
	DECLARE_READ8_MEMBER(st0016_character_ram_r);
	DECLARE_WRITE8_MEMBER(st0016_character_ram_w);
	DECLARE_READ8_MEMBER(st0016_vregs_r);
	DECLARE_READ8_MEMBER(st0016_dma_r);
	DECLARE_WRITE8_MEMBER(st0016_vregs_w);
	DECLARE_READ8_MEMBER(soundram_read);

	void set_st0016_game_flag(uint32_t flag) { m_game_flag = flag; }

	void draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void st0016_save_init();
	void draw_bgmap(bitmap_ind16 &bitmap,const rectangle &cliprect, int priority);

	void startup();
	uint32_t update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void st0016_draw_screen(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	std::unique_ptr<uint8_t[]> st0016_spriteram;
	std::unique_ptr<uint8_t[]> st0016_paletteram;


	int32_t st0016_spr_bank,st0016_spr2_bank,st0016_pal_bank,st0016_char_bank;
	int spr_dx,spr_dy;

	uint8_t st0016_vregs[0xc0];
	int st0016_ramgfx;
	std::unique_ptr<uint8_t[]> m_charram;

protected:
	// device-level overrides
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual void device_start() override;
	virtual void device_reset() override;

	const address_space_config m_io_space_config;
	const address_space_config m_space_config;


	const address_space_config *memory_space_config(address_spacenum spacenum) const override
	{
		switch (spacenum)
		{
			case AS_IO: return &m_io_space_config;
			case AS_PROGRAM: return &m_space_config;
			default: return z80_device::memory_space_config(spacenum);
		}
	};
	required_device<screen_device> m_screen;

private:
	uint8_t m_dma_offset;
	st0016_dma_offs_delegate m_dma_offs_cb;
	uint32_t m_game_flag;
};


// device type definition
extern const device_type ST0016_CPU;


#endif /// __ST0016_CPU__
