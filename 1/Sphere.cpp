#include "Sphere.h"

CSphere::CSphere() : r(0)
{
    b_create = false;
    obj.resize(VERTEX_NUM);
    for(int i = 0; i < VERTEX_NUM; i++) obj[i].resize(VERTEX_NUM);
    c = fl3(0,0,0);
    LightCoef = new CObjectLightParam(MATERIAL_BLACK_PLASTIC);
}
CSphere::CSphere(float _r)
{
    b_create = false;
    r = _r;
    obj.resize(VERTEX_NUM);
    for(int i = 0; i < VERTEX_NUM; i++) obj[i].resize(VERTEX_NUM);
    c = fl3(0,0,0);
    LightCoef = new CObjectLightParam();
}
CSphere::CSphere(float _r, fl3 _c): r(_r)
{
    b_create = false;
    c = _c;
    obj.resize(VERTEX_NUM);
    for(int i = 0; i < VERTEX_NUM; i++) obj[i].resize(VERTEX_NUM);
    LightCoef = new CObjectLightParam();
}
void CSphere::create()
{
    //for OGL drawing
    float phi = 0, phi_end = 2*M_PI, d_phi = (2*M_PI / (float)VERTEX_NUM);
    for(int i = 0; phi < phi_end && i < VERTEX_NUM; phi += d_phi, i++)
    {
        float psi = 0, psi_end = M_PI, d_psi = (M_PI / (float)VERTEX_NUM);
        for(int j = 0; psi < psi_end && j < VERTEX_NUM; psi += d_psi, j++)
        {
            obj[i][j] = c + fl3(r*cos(phi)*sin(psi),
                                r*cos(psi),
                                r*sin(phi)*sin(psi));
        }
    }

    //create matrix for ray tracing
    transpose += c;
    scale *= r;
    rotate &= fl3(0,1,0);
}
void CSphere::Draw()
{
    if(!b_create){create(); b_create = true;}

    glBegin(GL_LINES);
    for(uint i = 0; i < obj.size(); i++)
    {
        for(uint j = 0, j1 = 1; j1 < obj[i].size(); j++,j1++)
        {
            glVertex3fv(obj[i][j].m);
            glVertex3fv(obj[i][j1].m);
            glVertex3fv(obj[j][i].m);
            glVertex3fv(obj[j1][i].m);
        }
    }
    for(uint j = 0, n = obj.size() - 1; j < obj[0].size(); j++)
    {
        glVertex3fv(obj[0][j].m);
        glVertex3fv(obj[n][j].m);
    }
    glEnd();
}

fl3  CSphere::TransferToCanonical(fl3 point)
{
    return scale*(rotate*(transpose*point));
}
CRay CSphere::TransferToCanonical(CRay ray)
{
    return rotate*(scale*(transpose*ray));
}
bool CSphere::trace(CRay ray, float& t)
{
    CRay Ray = TransferToCanonical(ray);
    SavedEyeRay = Ray;
    Ray._2 = normalize(Ray._2);
    float nEye = norm(Ray._1);
    float _2ED = 2*(Ray._1*Ray._2);
    float nDir = norm(Ray._2);
    float A = nDir*nDir;
    float B = _2ED;
    float C = nEye*nEye - 1;
    float d = _2ED*_2ED - 4*nDir*nDir*(nEye*nEye-1);
    
    if(d < 0) return false;

    float t1 = (-B+sqrt(d))/2*A,
          t2 = (-B-sqrt(d))/2*A;
          t  = t1>=1 ? (t2>=1 ? min(t1,t2) : t1) : (t2>=1 ? t2 : -1);

    if(t < 0) return false;
          
    t *= r;
          
    SavedPoint = TransferToCanonical(ray.getPoint(t));
    SavedNormal = normalize(SavedPoint);
    return true;
}
void CSphere::calculatePointColor()
{
    SavedColor = fl3(0.,0.3,0.8);
}
void CSphere::calculateIntensity(vector<CLight> Lights)
{
    // рассчитываютс€ диффузные, спекул€рные и зеркальные интенсивности
    // по модели ‘онга.
    SavedIntensity.clear();
    int n = Lights.size();
    SavedIntensity.resize(n);
    for(int i = 0; i < n; i++)
    {
        CLight l = Lights[i];
        fl3 pos = TransferToCanonical(l.pos);
        fl3 _a = l.Ia ^ LightCoef->ka,
            _d = l.Id ^ LightCoef->kd,
            _s = l.Is ^ LightCoef->ks;
        SavedIntensity[i] = (CLight::light_model == CLight::LM_PHONG) 
            ? calculateIntensity_Phong       (_a,_d,_s,pos)
            : calculateIntensity_Cook_Torrens(_a,_d,_s,pos);
    }
}
fl3 CSphere::calculateIntensity_Phong(fl3 a, fl3 d, fl3 s, fl3 LightPos)
{
    fl3 n = SavedNormal,
        cp = SavedPoint,
        S = normalize(LightPos - cp),
        r = fl3_rotate(M_PI,n,S),
        v = normalize(TransferToCanonical(SavedEyeRay._1) - cp);
    return a + d*(n*S) + s*pow((r*v),LightCoef->F);
}
fl3 CSphere::calculateIntensity_Cook_Torrens(fl3 a, fl3 d, fl3 s, fl3 LightPos)
{
    return SavedNormal;
}
bool CSphere::trace(CRay ray)
{
    float t;
    return trace(ray,t);
}
fl3  CSphere::getNormal(fl3 Eye)
{
    return SavedNormal;
}
fl3  CSphere::getColor(vector<CLight> Lights, int SceneLightCount)
{

    //здесь интенсивность света от разных источников суммируетс€. 
    if(CLight::light_model != CLight::LM_NO_LIGHT)
    {
        int n = Lights.size();
        if(n == 0) return fl3(0,0,0);
        calculatePointColor();
        calculateIntensity(Lights);
        fl3 c = fl3(0,0,0);
        for(int i = 0; i < n; i++)
        {
            fl3 clr = Lights[i].clr;
            float r = clr.m[0],
                   g = clr.m[1],
                   b = clr.m[2];
            fl3 intensity = SavedIntensity[i];
            float ir = intensity.m[0],
                   ig = intensity.m[1],
                   ib = intensity.m[2];
           c += fl3(r*ir,g*ig,b*ib);
        }
        c = SavedColor ^ c;
        //c /= (float)SceneLightCount;
        if(c > fl3(0,1,0))
        {
            c = fl3(1,1,1);
        }
        return c;
    }
    return SavedNormal;
}