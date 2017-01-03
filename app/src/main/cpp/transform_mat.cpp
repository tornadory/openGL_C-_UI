//
// Created by cheng on 2016/12/9.
//

#include "common.h"
#include "transform_mat.h"

Matrix3X2::Matrix3X2(float i_a,float i_c, float i_b, float i_d,  float i_tx, float i_ty) noexcept
{
    _m={
       i_a,i_c,
       i_b,i_d,
       i_tx,i_ty
    };
}

Matrix3X2 Matrix3X2::translation(float i_tx, float i_ty)noexcept
{

  return
          {
                  1.0f,0.0f,
                  0.0f,1.0f,
                  i_tx,i_ty
          };

}

const float * Matrix3X2::getElement() const noexcept
{
    return &_m[0][0];
}

Matrix3X2 Matrix3X2::translate(float i_tx, float i_ty)noexcept
{
    return {

            _m[0][0],_m[0][1],
            _m[1][0],_m[1][1],
            _m[2][0]+i_tx,_m[2][1]+i_ty

    };
}

Matrix3X2 Matrix3X2::preTranslate(float i_tx, float i_ty)noexcept
{
    return {

            _m[0][0],_m[0][1],
            _m[1][0],_m[1][1],
            _m[0][0]*i_tx+_m[1][0]*i_ty+_m[2][0],_m[0][1]*i_tx+_m[1][1]*i_ty+_m[2][1]

    };
}

Matrix3X2 Matrix3X2::scaling(float i_sx, float i_sy)noexcept
{
    return
            {
                    i_sx,0.0f,
                    0.0f,i_sy,
                    0.0f,0.0f
    };
}

Matrix3X2 Matrix3X2::scaling(float i_s)noexcept
{
    return scaling(i_s,i_s);
}

Matrix3X2 Matrix3X2::scale(float i_s) noexcept
{
    return scale(i_s,i_s);
}

Matrix3X2 Matrix3X2::scale(float i_sx, float i_sy) noexcept
{
    return {
            _m[0][0]*i_sx,_m[0][1]*i_sy,
            _m[1][0]*i_sx,_m[1][1]*i_sy,
            _m[2][0]*i_sx,_m[2][1]*i_sy
    };
}

Matrix3X2 Matrix3X2::preScale(float i_s) noexcept
{
    return preScale(i_s,i_s);
}

Matrix3X2 Matrix3X2::preScale(float i_sx, float i_sy) noexcept
{
    return {

            i_sx*_m[0][0],i_sx*_m[0][1],
            i_sy*_m[1][0],i_sy*_m[1][1],
            _m[2][0],_m[2][1]
    };
}

Matrix3X2 Matrix3X2::rotation(float i_cos, float i_sin) noexcept
{
    return
            {
                i_cos,i_sin,
                -i_sin,i_cos,
                0,0
            };
}

Matrix3X2 Matrix3X2::rotation(float i_angle) noexcept
{
    return rotation(cosf(i_angle), sinf(i_angle));
}

Matrix3X2 Matrix3X2::rotate(float i_cos, float i_sin) noexcept
{
    return {

            _m[0][0]*i_cos-_m[0][1]*i_sin,_m[0][0]*i_sin+_m[0][1]*i_cos,

            _m[1][0]*i_cos-_m[1][1]*i_sin,_m[1][0]*i_sin+_m[1][1]*i_cos,

            _m[2][0]*i_cos-_m[2][1]*i_sin,_m[2][0]*i_sin+_m[2][1]*i_cos
    };
}

Matrix3X2 Matrix3X2::rotate(float i_angle) noexcept
{
    return rotate(cosf(i_angle), sinf(i_angle));
}

Matrix3X2 Matrix3X2::preRotate(float i_cos, float i_sin) noexcept
{
    return {

            i_cos*_m[0][0]+i_sin*_m[1][0],i_cos*_m[0][1]+i_sin*_m[1][1],
            -i_sin*_m[0][0]+i_cos*_m[1][0],-i_sin*_m[0][1]+i_cos*_m[1][1],
            _m[2][0],_m[2][1]

    };
}

Matrix3X2 Matrix3X2::preRotate(float i_angle)noexcept
{
    return preRotate(cosf(i_angle), sinf(i_angle));
}

Matrix3X2 operator*(const Matrix3X2 &i_a, const Matrix3X2 &i_b) noexcept
{
    return i_a.concat(i_b);
}

Matrix3X2 Matrix3X2::concat(const Matrix3X2 &i_left) const noexcept
{
    return {

            _m[0][0] * i_left._m[0][0] + _m[0][1] * i_left._m[1][0], _m[0][0] * i_left._m[0][1] + _m[0][1] * i_left._m[1][1],
            _m[1][0] * i_left._m[0][0] + _m[1][1] * i_left._m[1][0], _m[1][0] * i_left._m[0][1] + _m[1][1] * i_left._m[1][1],
            _m[2][0] * i_left._m[0][0] + _m[2][1] * i_left._m[1][0] + i_left._m[2][0], _m[2][0] * i_left._m[0][1] + _m[2][1] * i_left._m[1][1] + i_left._m[2][1],

    };
}

float Matrix3X2::getElement(int i_x, int i_y)noexcept
{
    return _m[i_x][i_y];
}




































