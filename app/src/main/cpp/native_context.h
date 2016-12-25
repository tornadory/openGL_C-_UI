//
// Created by cheng on 2016/11/23.
//
#pragma once

#include "render_context.h"
#include "command_queue.h"
#include "rect_1.h"
#include "animation.h"

struct NativeContext
{
public:
     void changeSurface(JNIEnv *env,jobject i_surface,int i_width,int i_height)noexcept ;
     void renderThread()noexcept ;
     void onPauseActivity()noexcept ;
     void onResumeAcitity()noexcept ;
     void onDestorySurface()noexcept ;
     void draw()noexcept;
     void initRect()noexcept;
     void addAnimation(shared_ptr<Rect> _rect)noexcept ;


private:
    RenderContext _rc;
    CommandQueue _commandQueue;
    Signal _signal;

    shared_ptr<Rect> _rect=make_shared<Rect>();
    AnimationManager _animationManager;

    bool _isPause=false;

    int _windowWidth;
    int _windowHeight;

};