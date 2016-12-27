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
    void draw(RenderContext & i_rendContext)noexcept ;

    //设置初始数据
    void setWidth(float i_width)noexcept ;
    void setHeight(float i_height)noexcept ;
    void setInitVertex(float i_x,float i_y)noexcept;
    void setCenter(float i_x,float i_y)noexcept ;

    float getWidth()noexcept ;
    float getHeight()noexcept ;
    float getCenterX()noexcept ;
    float getCenterY()noexcept ;

private:
    float _width;
    float _height;
    array<float,2> _center;  //比例

    array<float,2> _translate={0.0f,0.0f};
    array<float,2> _rotate={1.0f,0.0f};
    array<float,2> _scale={1.0f,1.0f};

};
