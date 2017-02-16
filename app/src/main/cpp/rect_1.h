//
// Created by cheng on 2016/12/22.
//
#pragma  once

#include "render_context.h"
#include "touch_listener.h"

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
    float getTranslateX()noexcept ;
    float getTranslateY()noexcept ;
    float getScaleX()noexcept ;
    float getScaleY()noexcept ;
    float getCos()noexcept ;
    float getSin()noexcept ;
    Matrix3X2 getInverseMatrix()noexcept ;
    void setInverseMatrix(Matrix3X2 i_matrix)noexcept ;


//    Rect *onPointerDown(map<int,array<float,4>> &i_point_map, int i_point_id, float i_x, float i_y)noexcept ;
//    void onPointerUp(int i_point_id) noexcept;
//    void onPointerMoved(map<int,array<float,4>> &i_point_map) noexcept;

    void setTouchListener(TouchListener *i_touchListener)noexcept ;
    TouchListener * getTouchListener()noexcept ;

    //4-4-6 事件分发
    bool onInterceptTouchEvent(map<int, array<float, 4>> &i_point_map, int i_point_id,
                               int i_event_type)noexcept ;
    bool touchEvent(map<int,array<float,4>> &i_point_map, int i_event_type, int i_point_id) noexcept;
    bool getRectAt(shared_ptr<Rect> &i_rect, float i_x, float i_y,
                   vector<shared_ptr<Rect>> &i_route)noexcept;

private:
    float _width;
    float _height;
    array<float,2> _center;  //比例

    array<float,2> _translate={0.0f,0.0f};
    array<float,2> _rotate={1.0f,0.0f};
    array<float,2> _scale={1.0f,1.0f};
    array<float,4> _color={1.0,1.0f,1.0f,1.0f};

    list<shared_ptr<Rect>> _rectChildren;
    Matrix3X2 _inverseMatrix;

    TouchListener * _touchListener= nullptr;

};

