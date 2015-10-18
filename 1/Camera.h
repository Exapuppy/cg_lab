#include "fl3.h"
#pragma once

class CCamera
{
private:
  fl3 m_vPosition;
  fl3 m_vView;
  fl3 m_vUpVector;
  fl3 m_vStrafe;
public:
  float kSpeed;
  bool MouseFlag;
  float Perspective;

  CCamera();
  fl3 Position() { return m_vPosition; }
  fl3 View() { return m_vView; }
  fl3 UpVector() { return m_vUpVector; }
  fl3 Strafe() { return m_vStrafe; }
  void PositionCamera(float PositionX, float PositionY, float PositionZ,
    float ViewX, float ViewY, float ViewZ,
    float upVectorX, float upVectorY, float upVectorZ);

  void StrafeCamera(float speed);
  void MoveCamera(float speed);
  void RotateView(float angle, float x, float y, float z);
  void Keyboard();
  void Update(int w, int h);
  void Look();
  void Mouse();
};