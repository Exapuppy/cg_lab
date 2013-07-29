#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "pch.h"
#include "fl3.h"

enum MATERIALS_COEFFICIENT
{
	MATERIAL_BLACK_PLASTIC,
	MATERIAL_BRASS,
	MATERIAL_BRONZE,
	MATERIAL_CHROME,
	MATERIAL_COPPER,
	MATERIAL_GOLD,
	MATERIAL_TIN,
	MATERIAL_SILVER,
	MATERIAL_POLISHED_SILVER,
    MATERIAL_INDEX
};

enum METALS_REFRACTION_COEFFICIENT
{
    METALS_GOLD,
    METALS_SILVER,
    METALS_COPPER,
    METALS_IRON,//TODO: I don't no! bronze!!
    METALS_INDEX
};

#pragma region Material parameter
const fl3 m_ambient[MATERIAL_INDEX] = { fl3(0.0000,0.0000,0.0000),
								        fl3(0.3294,0.2235,0.0275),
								        fl3(0.2125,0.1275,0.0540),
								        fl3(0.2500,0.2500,0.2500),
								        fl3(0.1913,0.0735,0.0225),
								        fl3(0.2473,0.1995,0.0745),
								        fl3(0.1059,0.0588,0.1137),
								        fl3(0.1923,0.1923,0.1923),
								        fl3(0.2313,0.2313,0.2313)};
const fl3 m_diffuse[MATERIAL_INDEX] = { fl3(0.0100,0.0100,0.0100),
								        fl3(0.7804,0.5687,0.1137),
								        fl3(0.7140,0.4284,0.1814),
								        fl3(0.4000,0.4000,0.4000),
								        fl3(0.7038,0.2705,0.0828),
								        fl3(0.7516,0.6065,0.2264),
								        fl3(0.4275,0.4706,0.5412),
								        fl3(0.5075,0.5075,0.5075),
								        fl3(0.2775,0.2775,0.2775)};
const fl3 m_specular[MATERIAL_INDEX] = {fl3(0.5000,0.5000,0.5000),
						                fl3(0.9922,0.9412,0.8087),
						                fl3(0.3935,0.2719,0.1667),
					                    fl3(0.7746,0.7746,0.7746),
					                    fl3(0.2568,0.1376,0.0860),
					                    fl3(0.6283,0.5558,0.3661),
				                        fl3(0.3333,0.3333,0.5216),
			                            fl3(0.5083,0.5083,0.5083),
	                                    fl3(0.7739,0.7739,0.7739)};
const double m_shininess[MATERIAL_INDEX] = {32,28,26,77,13,51,10,51,90};
#pragma endregion Material parameters
#pragma region Refraction coefficients
const fl3 F0[METALS_INDEX] = {fl3(0.989,0.876,0.349),
                              fl3(0.950,0.930,0.880),
                              fl3(0.755,0.490,0.095),
                              fl3(0.530,0.505,0.480)};
const int bridge[METALS_INDEX] = {5,7,4,2};
#pragma endregion Refraction coefficients

#endif /*__MATERIAL_H*/