#include "Scene.h"
#include "matrix.h"

float _max(float a, float b, float c) { return a>b?(a>c? a: c):(b>c? b: c);}
bool operator<(const Figure l, const Figure r){return l.t<r.t;}
bool operator>(const Figure l, const Figure r){return l.t>r.t;}

void PixelMatrix::setColor(uint c, uint r, fl3 Color)
{
    int i = r*width + c;
    pixels[3*i  ] = Color.m[0];
    pixels[3*i+1] = Color.m[1];
    pixels[3*i+2] = Color.m[2];
}
void CScene::Draw()
{
    glColor3f(0,0,0);
    for(auto it = light .begin(); it != light .end(); it++) it->Draw();
    for(auto it = sphere.begin(); it != sphere.end(); it++) it->Draw();
    for(auto it = cone  .begin(); it != cone  .end(); it++) it->Draw();
}
void CScene::RayTracing()
{
    static bool b = false;
    if(b == false)
    {
        int n = pixelMatrix.height*pixelMatrix.width;
        delete[] pixelMatrix.pixels;
        pixelMatrix.pixels = new float[3*n];
        b = true;
    }
    ///////////////////////////////////////////////////////////////////////////

    fl3     Eye   = camera.Position();
    fl3     w     = normalize(camera.View() - camera.Position());
    float   theta = camera.Perspective;
    uint     nCols = pixelMatrix.width;
    uint     nRows = pixelMatrix.height;
    float   aspect = (float)nCols/(float)nRows;
    float   N     = 1;
    float   H     = N*tan(theta*0.5f);
    float   W     = H*aspect;
    fl3     v     = norm_ort(camera.Strafe(),w);
    fl3     u     = camera.Strafe();

    ///////////////////////////////////////////////////////////////////////////
    
    for(uint c = 0; c < nCols; c++)
    for(uint r = 0; r < nRows; r++)
    {
        if( c == nCols>>1 && r == nRows>>1)
        {
            c = nCols>>1;
        }
        fl3   color;
        float uc = - W + W*(2*(float)c/nCols),
              vr = - H + H*(2*(float)r/nRows);
        fl3   Dir_rc = N*w+uc*u+vr*v;

        CRay ray = CRay(Eye,Dir_rc);

        //ищем всевозможные точки пересечения луча с фигурами
        CreateFigures(ray);

        if(Figures.size() > 0)
        {
            vector<Figure>::iterator it;
            //нашли первую точку пересечения луча с некоторой фигурой (it).
            bool b = SearchMinInFigures(it);
            if(CLight::light_model != CLight::LM_NO_LIGHT)
            {
                
                //осуществляем расчет нормали в этой точке
                fl3 ObjectNormal;
                switch(it->type)
                {
                case Figure::FT_SPHERE: 
                    ObjectNormal = sphere[it->index].getNormal(ray._1); break;
                case Figure::FT_CONE:
                    ObjectNormal = cone  [it->index].getNormal(ray._1); break;
                }

                //рассчитываем источники света,
                //    свет которых попадает в эту точку
                //(с учетом других объектов на сцене)
                fl3 Point = ray.getPoint(it->t);
                CreateNewLightVector(Point, ObjectNormal);
            }
            int LightCount = light.size();
            //рассчитываем цвет в точке
            if(b)
            switch(it->type)
            {
            case Figure::FT_SPHERE: 
                color = sphere[it->index].getColor(NewLights,LightCount);
                break;
            case Figure::FT_CONE:
                color = cone[it->index].getColor(NewLights,LightCount);  
                break;
            }
        }
        else
        {
            color = BackColor;
        }
        pixelMatrix.setColor(c,r,color);
    }
}
void CScene::CreateFigures(CRay ray)
{
    Figures.clear();
    int i = 0;
    float t;
    for(auto it = sphere.begin(); it != sphere.end(); it++, i++) 
    {
        if(it->trace(ray, t))
        {
            Figure a;
            a.index = i;
            a.t = t;
            a.type = a.FT_SPHERE;
            Figures.push_back(a);
        }
    }
    i = 0;
    for(auto it = cone.begin(); it != cone  .end(); it++, i++)
    {
        if(it->trace(ray, t))
        {
            Figure a;
            a.index = i;
            a.t = t;
            a.type = a.FT_CONE;
            Figures.push_back(a);
        }
    }
}
bool CScene::SearchMinInFigures(vector<Figure>::iterator& it)
{
    if(Figures.size() > 0)
    {
        float t_min = Figures[0].t;
        it = begin(Figures);
        int i = 0;
        for(int j = 0; it != end(Figures); it++, j++)
        {
            if(it->t < t_min) 
            {
                t_min = it->t;
                i = j;
            }
        }
        it = begin(Figures); advance(it,i);
        return true;
    }
    else
    {
        return false;
    }
    
}
void CScene::CreateNewLightVector(fl3 Point, fl3 Normal)
{
    NewLights.clear();
    for(auto it : light)
    {
        bool b = false;
        //Посылаю луч из точки на сфере в направлении источника света.
        //если он пересекается с каким нибудь объектом до достижения 
        //источника, то источник исключается.
        CRay ray = CRay(Point, normalize(it.pos - Point));
        {
            for(auto it2 = sphere.begin(); it2 != sphere.end() && !b; it2++) 
                if(it2->trace(ray)) b = true;
            for(auto it2 = cone  .begin(); it2 != cone.end()   && !b; it2++)
                if(it2->trace(ray)) b = true;
        }
        if(b == false) 
            NewLights.push_back(it);
    }
}