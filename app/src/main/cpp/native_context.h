//
// Created by cheng on 2016/11/23.
//
#pragma once

#include "render_context.h"
#include "command_queue.h"
#include "rect_1.h"
#include "animation.h"

struct NativeContext:TouchListener
{
public:
     void changeSurface(JNIEnv *env,jobject i_surface,int i_width,int i_height)noexcept ;
     void renderThread()noexcept ;
     void onPauseActivity()noexcept ;
     void onResumeAcitity()noexcept ;
     void onDestorySurface()noexcept ;

//     void onPointerDown(int i_point_id) noexcept;
//     void onPointerUp(int i_point_id) noexcept;
//     void onPointerMoved() noexcept;
     void setPoint(int i_point_id,float i_x,float i_y)noexcept ;

     void draw()noexcept;
     void initRect()noexcept;
     void addAnimation(shared_ptr<Rect> _rect,shared_ptr<Rect> _rect1,shared_ptr<Rect> _rect2)noexcept ;

//     virtual void touchMove() noexcept override final;
//     virtual void touchDown(int i_point_id) noexcept override final;
//     virtual void touchUp(int i_point_id) noexcept override final;

     virtual bool touchEvent(int i_event_type, int i_point_id, Rect *i_rect)noexcept override final ;
     void getRectAt(int i_point_id, Rect *i_lastTouch)noexcept ;
     void depatchTouchEvent(int i_event_type, int i_point_id)noexcept ;
     void handleTouch(int i_event_type, int i_point_id)noexcept ;

     void addPoint(int i_pointId,float i_x,float i_y)noexcept ;
     void removePoint(int i_pointId)noexcept ;
     float getCurrentDistance()noexcept ;
     array<float,4> getcurrentPoints()noexcept ;
     float sinAB(float i_cosa,float i_sina,float i_cosb,float i_sinb)noexcept ;
     float cosAB(float i_cosa,float i_sina,float i_cosb,float i_sinb)noexcept ;
     float getCosa(array<float,4> i_start,array<float,4> i_end)noexcept ;
     float getSina(array<float,4> i_start,array<float,4> i_end)noexcept ;


private:
    RenderContext _rc;
    CommandQueue _commandQueue;
    Signal _signal;

    Rect * _rectTouch;

    shared_ptr<Rect> _rect=make_shared<Rect>();
    shared_ptr<Rect> _rect_1=make_shared<Rect>();
    shared_ptr<Rect> _rect_2=make_shared<Rect>();
    shared_ptr<Rect> _rect_3=make_shared<Rect>();

    Rect * _lastTouch= _rect.get();
    vector<shared_ptr<Rect>> _route;
    bool isFirstDown= true;

    AnimationManager _animationManager;
    TimerOur _timerOur;

    bool _isPause=false;

    int _windowWidth;
    int _windowHeight;

    map<int,array<float,4>> _point_map;

    int _status=-1;
    int _pen_handler_id=-1;
    array<float,2> _scale_handler_ids={-1,-1};

    float _scaleRadio=1.0f;
    float _distance=0.0f;

    float _down_x=0.0f;
    float _down_y=0.0f;

    float _cosA=1;
    float _sinA=0;

    array<float,4> _startPoints;

};