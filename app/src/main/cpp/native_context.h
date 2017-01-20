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

     void onPointerDown(int i_point_id, float i_x, float i_y) noexcept;
     void onPointerUp(int i_point_id, float i_x, float i_y) noexcept;
     void onPointerMoved(int i_point_id, float i_x, float i_y) noexcept;

     void draw()noexcept;
     void initRect()noexcept;
     void addAnimation(shared_ptr<Rect> _rect,shared_ptr<Rect> _rect1,shared_ptr<Rect> _rect2)noexcept ;

     virtual void touchMove(int i_point_id, float i_dx, float i_dy) noexcept override final;
     virtual void touchDown(int i_point_id, float i_dx, float i_dy) noexcept override final;
     virtual void touchUp(int i_point_id, float i_dx, float i_dy) noexcept override final;

     void addPoint(int i_pointId,float i_x,float i_y)noexcept ;
     void removePoint(int i_pointId,float i_x,float i_y)noexcept ;



private:
    RenderContext _rc;
    CommandQueue _commandQueue;
    Signal _signal;

    Rect * _rectTouch;

    shared_ptr<Rect> _rect=make_shared<Rect>();
    shared_ptr<Rect> _rect_1=make_shared<Rect>();
    shared_ptr<Rect> _rect_2=make_shared<Rect>();

    float _down_x=0;
    float _down_y=0;

    AnimationManager _animationManager;
    TimerOur _timerOur;

    bool _isPause=false;

    int _windowWidth;
    int _windowHeight;

    map<int,array<float,2>> _point_map;
    map<int,array<float,2>> _move_map;

};