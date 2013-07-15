/**
 * @file Matrix.cpp
 * Implements RotationMatrix
 *
 * @author <a href="mailto:martin.kallnik@gmx.de">Martin Kallnik</a>
 * @author Max Risler
 */

#include "Matrix.h"
#include "MVTools.h"
#include "Range.h"
//#include "Common.h"

RotationMatrix::RotationMatrix(
  const Vector3<double>& c0,
  const Vector3<double>& c1,
  const Vector3<double>& c2) : Matrix3x3<double>(c0,c1,c2) {}

RotationMatrix::RotationMatrix(const double& yaw, const double& pitch, const double& roll)
{
  const double cy=cos(yaw);
  const double sy=sin(yaw);
  const double cp=cos(pitch);
  const double sp=sin(pitch);
  const double cr=cos(roll);
  const double sr=sin(roll);

  c[0].x=cr*cp ;
  c[0].y=sr*cp ;
  c[0].z=-sp ;
  c[1].x=-sr*cy+cr*sp*sy ;
  c[1].y=cr*cy+sr*sp*sy ;
  c[1].z=cp*sy ;
  c[2].x=sr*sy+cr*sp*cy ;
  c[2].y=-cr*sy+sr*sp*cy ;
  c[2].z=cp*cy ;
}

RotationMatrix::RotationMatrix(const Vector3<double>& a, const double& angle)
{
  Vector3<double> axis(a);
  //rotation is only possible with unit vectors
  axis.normalize();
  const double &x = axis.x, &y = axis.y, &z = axis.z;
  //compute sine and cosine of angle because it is needed quite often for complete matrix
  const double si = sin(angle), co = cos(angle);
  //compute all components needed more than once for complete matrix
  const double v = 1 - co;
  const double xyv = x * y * v;
  const double xzv = x * z * v;
  const double yzv = y * z * v;
  const double xs = x * si;
  const double ys = y * si;
  const double zs = z * si;
  //compute matrix
  c[0].x = x * x * v + co; c[1].x = xyv - zs;      c[2].x = xzv + ys;
  c[0].y = xyv + zs;      c[1].y = y * y * v + co; c[2].y = yzv - xs;
  c[0].z = xzv - ys;      c[1].z = yzv + xs;      c[2].z = z * z * v + co;
}

RotationMatrix::RotationMatrix(const Vector3<double>& a)
{
  Vector3<double> axis(a);
  const double& angle(a.abs());
  //rotation is only possible with unit vectors
  axis.normalize();
  const double &x = axis.x, &y = axis.y, &z = axis.z;
  //compute sine and cosine of angle because it is needed quite often for complete matrix
  const double si = sin(angle), co = cos(angle);
  //compute all components needed more than once for complete matrix
  const double v = 1 - co;
  const double xyv = x * y * v;
  const double xzv = x * z * v;
  const double yzv = y * z * v;
  const double xs = x * si;
  const double ys = y * si;
  const double zs = z * si;
  //compute matrix
  c[0].x = x * x * v + co; c[1].x = xyv - zs;      c[2].x = xzv + ys;
  c[0].y = xyv + zs;      c[1].y = y * y * v + co; c[2].y = yzv - xs;
  c[0].z = xzv - ys;      c[1].z = yzv + xs;      c[2].z = z * z * v + co;
}

RotationMatrix& RotationMatrix::rotateX(const double angle)
{
  double c = cos(angle),
         s = sin(angle);
  *this = RotationMatrix(this->c[0], this->c[1] * c + this->c[2] * s, this->c[2] * c - this->c[1] * s);
  return *this;
}

RotationMatrix& RotationMatrix::rotateY(const double angle)
{
  double c = cos(angle),
         s = sin(angle);
  *this = RotationMatrix(this->c[0] * c - this->c[2] * s, this->c[1], this->c[2] * c + this->c[0] * s);
  return *this;
}

RotationMatrix& RotationMatrix::rotateZ(const double angle)
{
  double c = cos(angle),
         s = sin(angle);
  *this = RotationMatrix(this->c[0] * c + this->c[1] * s, this->c[1] * c - this->c[0] * s, this->c[2]);
  return *this;
}

double RotationMatrix::getXAngle() const
{
  double h = sqrt(c[2].y * c[2].y + c[2].z * c[2].z);
  return h ? acos(c[2].z / h) * (c[2].y > 0 ? -1 : 1) : 0;
}

double RotationMatrix::getYAngle() const
{
  double h = sqrt(c[0].x * c[0].x + c[0].z * c[0].z);
  return h ? acos(c[0].x / h) * (c[0].z > 0 ? -1 : 1) : 0;
}

double RotationMatrix::getZAngle() const
{
  double h = sqrt(c[0].x * c[0].x + c[0].y * c[0].y);
  return h ? acos(c[0].x / h) * (c[0].y < 0 ? -1 : 1) : 0;
}

void RotationMatrix::getAngleAxis(Vector3<double>& axis, double& angle) const
{
  const double cosAngle = ((c[0].x + c[1].y + c[2].z) - 1.0) / 2.0;
  static const Range<double> clipping(-1.0, 1.0);
  angle = acos(clipping.limit(cosAngle));
  if(MVTools::isNearZero(angle))
  {
    axis = Vector3<double>(1, 0, 0);
  }
  else
  {
    axis.x = c[1].z - c[2].y;
    axis.y = c[2].x - c[0].z;
    axis.z = c[0].y - c[1].x;
    axis.normalize();
  }
}

Vector3<> RotationMatrix::getAngleAxis() const
{
  float co = (c[0].x + c[1].y + c[2].z - 1.f) * 0.5f;
  if(co > 1.f)
    co = 1.f;
  else if(co < -1.f)
    co = 1.f;
  const float angle = acos(co);
  if(angle == 0.f)
    return Vector3<>();
  Vector3<> result(
    c[1].z - c[2].y,
    c[2].x - c[0].z,
    c[0].y - c[1].x);
  result *= angle / (2.f * sin(angle));
  return result;
}
