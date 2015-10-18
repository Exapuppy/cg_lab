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
const fl3 m_ambient[MATERIAL_INDEX] = { fl3(0.0000f,0.0000f,0.0000f),
								        fl3(0.3294f,0.2235f,0.0275f),
								        fl3(0.2125f,0.1275f,0.0540f),
								        fl3(0.2500f,0.2500f,0.2500f),
								        fl3(0.1913f,0.0735f,0.0225f),
								        fl3(0.2473f,0.1995f,0.0745f),
								        fl3(0.1059f,0.0588f,0.1137f),
								        fl3(0.1923f,0.1923f,0.1923f),
								        fl3(0.2313f,0.2313f,0.2313f)};
const fl3 m_diffuse[MATERIAL_INDEX] = { fl3(0.0100f,0.0100f,0.0100f),
								        fl3(0.7804f,0.5687f,0.1137f),
								        fl3(0.7140f,0.4284f,0.1814f),
								        fl3(0.4000f,0.4000f,0.4000f),
								        fl3(0.7038f,0.2705f,0.0828f),
								        fl3(0.7516f,0.6065f,0.2264f),
								        fl3(0.4275f,0.4706f,0.5412f),
								        fl3(0.5075f,0.5075f,0.5075f),
								        fl3(0.2775f,0.2775f,0.2775f)};
const fl3 m_specular[MATERIAL_INDEX] = {
                        fl3(0.5000f,0.5000f,0.5000f),
						            fl3(0.9922f,0.9412f,0.8087f),
						            fl3(0.3935f,0.2719f,0.1667f),
					              fl3(0.7746f,0.7746f,0.7746f),
					              fl3(0.2568f,0.1376f,0.0860f),
					              fl3(0.6283f,0.5558f,0.3661f),
				                fl3(0.3333f,0.3333f,0.5216f),
			                  fl3(0.5083f,0.5083f,0.5083f),
	                      fl3(0.7739f,0.7739f,0.7739f)};
const float m_shininess[MATERIAL_INDEX] = {32,28,26,77,13,51,10,51,90};
#pragma endregion Material parameters
#pragma region Refraction coefficients
const fl3 F0[METALS_INDEX] = {fl3(0.989f,0.876f,0.349f),
                              fl3(0.950f,0.930f,0.880f),
                              fl3(0.755f,0.490f,0.095f),
                              fl3(0.530f,0.505f,0.480f)};
const int bridge[METALS_INDEX] = {5,7,4,2};
#pragma endregion Refraction coefficients

#endif /*__MATERIAL_H*/