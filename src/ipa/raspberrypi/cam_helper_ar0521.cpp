/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2021, Raspberry Pi (Trading) Limited
 *
 * cam_helper_ar0521.cpp - camera information for ar0521 sensor
 */

#include <assert.h>

#include "cam_helper.hpp"

using namespace RPiController;

class CamHelperAr0521 : public CamHelper
{
public:
	CamHelperAr0521();
	uint32_t GainCode(double gain) const override;
	double Gain(uint32_t gain_code) const override;
	void GetDelays(int &exposure_delay, int &gain_delay,
		       int &vblank_delay) const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	static constexpr int frameIntegrationDiff = 4;
};

/*
 * Ar0521 doesn't output metadata, so we have to use the "unicam parser" which
 * works by counting frames.
 */

CamHelperAr0521::CamHelperAr0521()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperAr0521::GainCode(double gain) const
{
	return static_cast<uint32_t>(gain * 16.0);
}

double CamHelperAr0521::Gain(uint32_t gain_code) const
{
	return static_cast<double>(gain_code) / 16.0;
}

void CamHelperAr0521::GetDelays(int &exposure_delay, int &gain_delay,
				int &vblank_delay) const
{
	/* The driver appears to behave as follows: */
	exposure_delay = 2;
	gain_delay = 2;
	vblank_delay = 2;
}

static CamHelper *Create()
{
	return new CamHelperAr0521();
}

static RegisterCamHelper reg("ar0521", &Create);
