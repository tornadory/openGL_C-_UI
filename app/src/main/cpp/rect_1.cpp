//
// Created by cheng on 2016/12/22.
//

#include "common.h"
#include "rect_1.h"
#include "transform_mat.h"

void Rect::setRotate(float i_x, float i_y)noexcept
{
    _rotate[0]=i_x;
    _rotate[1]=i_y;
}

void Rect::setScale(float i_kx, float i_ky)noexcept
{
    _scale[0]=i_kx;
    _scale[1]=i_ky;
}

void Rect::setTranslate(float i_x, float i_y) noexcept
{
    _translate[0]=i_x;
    _translate[1]=i_y;
}

void Rect::draw(GLBuffer & i_buffer,RenderContext::RectProgram& i_program) noexcept
{
    /**
     * buffer
     * {
     *  0.0f,0.0f,
     *  0.0f,1.0f,
     *  1.0f,0.0f,
     *  1.0f,1.0f
     * }
     */

    i_buffer.bind();

    i_program.bindPos(2,0);
    i_program.setColor({1.0f,1.0f,1.0f,1.0f});

    Matrix3X2 mat  //buffer *mat 变成 四个buffer对用的屏幕坐标系的坐标
            {
                    _width,0.0f,
                    0.0f,_heigt,
                    0, 0

            };

    mat=mat.translate(-_center[0],-_center[1]);

    mat=mat.scale(_scale[0],_scale[1]); //缩放动画
    mat=mat.rotate(_rotate[0],_rotate[1]); //旋转动画

    mat=mat.translate(_center[0],_center[1]);

    mat=mat.translate(_translate[0],_translate[1]);//位移动画

    //屏幕坐标系 -> openGL坐标系
    float kx=1/_parent_width*2;
    float ky=1/_parent_height*2;
    mat=mat.scale(kx,ky);
    mat=mat.scale(1,-1);
    mat=mat.translate(-1,1);

    i_program.setTransform(mat);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    dbglog("===draw=====");

}

void Rect::setWidth(float i_width) noexcept
{
    _width=i_width;
}

void Rect::setHeight(float i_height) noexcept
{
    _heigt=i_height;
}

void Rect::setInitVertex(float i_width, float i_height) noexcept
{
    _init[0]=i_width;
    _init[1]=i_height;
}

void Rect::setCenter(float i_x, float i_y)noexcept
{
    _center[0]=i_x;
    _center[1]=i_y;
}

void Rect::setParentWidthHeight(float i_width, float i_height)noexcept
{
    _parent_width=i_width;
    _parent_height=i_height;
}

float Rect::getWidth() noexcept
{
    return _width;
}

float Rect::getHeight()noexcept
{
    return _heigt;
}

float Rect::getInitVertexX() noexcept
{
    return _init[0];
}

float Rect::getInitVertexY()noexcept
{
    return _init[1];
}





















