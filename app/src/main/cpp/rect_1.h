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
    void draw(RenderContext &i_rendContext, Matrix3X2 i_matix)noexcept ;
    void addRectChild(shared_ptr<Rect> i_rect)noexcept ;

    //设置初始数据
    void setWidth(float i_width)noexcept ;
    void setHeight(float i_height)noexcept ;
    void setInitVertex(float i_x,float i_y)noexcept;
    void setCenter(float i_x,float i_y)noexcept ;
    void setColor(array<float,4> i_color)noexcept ;
    array<float, 2> transformPosition(float i_x, float i_y) noexcept;
    float getWidth()noexcept ;
    float getHeight()noexcept ;
    float getCenterX()noexcept ;
    float getCenterY()noexcept ;

    bool onPointerDown(float i_x, float i_y, Matrix3X2 i_parent_counter_matrix)noexcept ;
    void onPointerUp(float i_x, float i_y) noexcept;
    void onPointerMoved(float i_x, float i_y) noexcept;

    void pointerDown(float i_x, float i_y)noexcept ;
    void pointerUp(float i_x, float i_y) noexcept;
    void pointerMoved(float i_x, float i_y) noexcept;

private:
    float _width;
    float _height;
    array<float,2> _center;  //比例

    array<float,2> _translate={0.0f,0.0f};
    array<float,2> _rotate={1.0f,0.0f};
    array<float,2> _scale={1.0f,1.0f};
    array<float,4> _color={1.0,1.0f,1.0f,1.0f};

    list<shared_ptr<Rect>> _rectChildren;

};


