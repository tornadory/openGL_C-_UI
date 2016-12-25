//
// Created by cheng on 2016/12/22.
//
#pragma  once

#include "render_context.h"

struct Rect
{
public:

    //更新完 translate rotate scale 数据，并显示
    void setTranslate(float i_x, float i_y)noexcept;
    void setRotate(float i_x,float i_y)noexcept;
    void setScale(float i_kx,float i_ky)noexcept ;
    void draw(GLBuffer & i_buffer,RenderContext::RectProgram& i_program)noexcept ;

    //设置初始数据
    void setWidth(float i_width)noexcept ;
    void setHeight(float i_height)noexcept ;
    void setInitVertex(float i_width,float i_height)noexcept;
    void setCenter(float i_x,float i_y)noexcept ;

    void setParentWidthHeight(float i_width,float i_height)noexcept;

    float getWidth()noexcept ;
    float getHeight()noexcept ;

    float getInitVertexX()noexcept ;
    float getInitVertexY()noexcept ;

private:
    float _width;
    float _heigt;
    array<float,2> _init={0.0f,0.0f};
    array<float,2> _center;

    float _parent_width;
    float _parent_height;

    array<float,2> _translate={0.0f,0.0f};
    array<float,2> _rotate={1.0f,0.0f}; ///????????
    array<float,2> _scale={1.0f,1.0f};

};

