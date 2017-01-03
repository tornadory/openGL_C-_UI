//
// Created by cheng on 2016/12/9.
//
#pragma once

struct Matrix3X2
{

public:

    Matrix3X2()= default;
    Matrix3X2(float i_a,float i_b,float i_tx,float i_c,float i_d,float i_ty)noexcept ;


    static Matrix3X2 translation(float i_tx,float i_ty)noexcept ;

    static Matrix3X2 scaling(float i_sx,float i_sy)noexcept ;
    static Matrix3X2 scaling(float i_s)noexcept ;

    static Matrix3X2 rotation(float i_cos,float i_sin)noexcept ;
    static Matrix3X2 rotation(float i_angle)noexcept ;

    Matrix3X2 translate(float i_tx,float i_ty)noexcept ;
    Matrix3X2 preTranslate(float i_tx,float i_ty)noexcept ;

    Matrix3X2 scale(float i_s)noexcept ;
    Matrix3X2 scale(float i_sx,float i_sy)noexcept ;
    Matrix3X2 preScale(float i_s)noexcept ;
    Matrix3X2 preScale(float i_sx,float i_sy)noexcept ;

    Matrix3X2 rotate(float i_cos,float i_sin)noexcept ;
    Matrix3X2 rotate(float i_angle)noexcept ;

    Matrix3X2 preRotate(float i_cos,float i_sin)noexcept ;
    Matrix3X2 preRotate(float i_angle)noexcept ;

    const float * getElement() const noexcept ;
    float getElement(int i_x,int i_y)noexcept ;

    Matrix3X2 concat(const Matrix3X2 &i_left) const noexcept;

private:

    std::array<std::array<float,2>,3> _m=
            {
              1.0f,0.0f,
              0.0f,1.0f,
              0.0f,0.0f
            };


};

Matrix3X2 operator*(const Matrix3X2 &i_a, const Matrix3X2 &i_b) noexcept;
