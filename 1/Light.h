#ifndef __LIGHT_H
#define __LIGHT_H

#include "fl3.h"
#include "Material.h"
#pragma once

static const fl3 BackColor = fl3(0.4,0.4,0.4);

class CLight
{
public:
    static enum LIGHT_MODEL
    {
        LM_NO_LIGHT,
        LM_PHONG,
        LM_COOK_TORRENS
    }light_model;

	fl3	pos;	//расположение света
	fl3	clr;	//цвет света
    fl3 Ia,Id,Is; //фонова€, диффузна€ и зеркальна€ составл€ющие света
    CLight( );
    CLight(fl3 pos): pos(pos),
                     clr(fl3(1,1,1)),
                     Ia(fl3(1,1,1)),
                     Id(fl3(1,1,1)),
                     Is(fl3(1,1,1))
    {}
    CLight(fl3 pos, fl3 clr): pos(pos),
                              clr(clr),
                              Ia(fl3(1,1,1)), 
                              Id(fl3(1,1,1)), 
                              Is(fl3(1,1,1))
    {}
    void Draw();
};

class CObjectLightParam
{
public:
    //коэф. фоновой, диффузной и зеркальной составл€ющих.
    fl3 ka,
        kd,
        ks;
    float F;
    fl3 F0;
    CObjectLightParam()
        :ka(m_ambient[bridge[METALS_IRON]])
        ,kd(m_diffuse[bridge[METALS_IRON]])
        ,ks(m_specular[bridge[METALS_IRON]])
        ,F(m_shininess[bridge[METALS_IRON]])
        ,F0(::F0[METALS_IRON])
    {}
    CObjectLightParam(METALS_REFRACTION_COEFFICIENT a)
        :ka(m_ambient[bridge[a]])
        ,kd(m_diffuse[bridge[a]])
        ,ks(m_specular[bridge[a]])
        ,F(m_shininess[bridge[a]])
        ,F0(::F0[a])
    {}
    CObjectLightParam(MATERIALS_COEFFICIENT a)
        :ka(m_ambient[a])
        ,kd(m_diffuse[a])
        ,ks(m_specular[a])
        ,F(m_shininess[a])
        ,F0(fl3(1,1,1))
    {}
    void set(METALS_REFRACTION_COEFFICIENT a);
    void set(MATERIALS_COEFFICIENT a);
};

#endif /*__LIGHT_H*/