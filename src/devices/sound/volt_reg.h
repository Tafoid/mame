// license:BSD-3-Clause
// copyright-holders:smf
/***************************************************************************

    volt_reg.h

    Voltage Regulator

***************************************************************************/

#ifndef __VOLTAGE_REGULATOR_H__
#define __VOLTAGE_REGULATOR_H__

#include "emu.h"

#define MCFG_VOLTAGE_REGULATOR_OUTPUT(_output) \
	voltage_regulator_device::set_output(*device, _output);

class voltage_regulator_device : public device_t,
	public device_sound_interface
{
public:
	static void set_output(device_t &device, double analogue_dc) { downcast<voltage_regulator_device &>(device).m_output = (analogue_dc * 32768) / 5.0f; }

	voltage_regulator_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	~voltage_regulator_device() { }

	virtual bool issound() override { return false; }

protected:
	// device-level overrides
	virtual void device_start() override;

	// sound stream update overrides
	virtual void sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples) override;

private:
	sound_stream* m_stream;
	stream_sample_t m_output;
};

extern const device_type VOLTAGE_REGULATOR;

#endif
