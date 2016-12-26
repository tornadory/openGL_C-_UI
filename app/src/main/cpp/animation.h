//
// Created by cheng on 2016/12/16.
//
#pragma once

#include <ctime>
#include "transform_mat.h"
#include "rect_1.h"

/**
 * AnimationInterface
 */
struct AnimationInterface
{
    virtual void updateAnimation(double i_time)= 0;
    virtual bool isEnd()=0;

};

/**
 * RectAnimation
 */
struct RectAnimation:AnimationInterface
{
public:
    RectAnimation(shared_ptr<Rect> i_rect,double i_time,double i_start_time)noexcept;
    virtual void updateAnimation( double i_time )=0;
    virtual bool isEnd() noexcept override final ;

protected:

    shared_ptr<Rect> _rect;

    double  _time_start=0.0;
    double  _time_end=0.0;
    double  _time_run=0.0;
    bool _isEnd=false;
};

/**
* 旋转动画 ========ok====2016年12月24日
*/
struct RotateAnimaton:RectAnimation
{
public:
    RotateAnimaton(shared_ptr<Rect> i_rect,double i_time,double i_start_time,float i_start_angle,float i_end_angle)noexcept;
    virtual  void updateAnimation( double i_time )noexcept override  final ;

private :
    float _startAngle; //-1~1 值的范围
    float _endAngle;
};

/**
 * 缩放动画 ========ok====2016年12月25日
 */
struct ScaleAnimation:RectAnimation
{
public :
    ScaleAnimation(shared_ptr<Rect> i_rect, double i_time,double i_start_time, float i_startyKx, float i_startKy,float i_endKx, float i_endKy)noexcept ;
    ScaleAnimation(shared_ptr<Rect> i_rect,double i_time,double i_start_time,float i_startK,float i_endK)noexcept ;
    virtual  void updateAnimation( double i_time )noexcept override  final ;
private :

    float _startKx;
    float _startKy;

    float _endKx;
    float _endKy;
};

/**
 * 平移动画  ========ok====2016年12月24日
 */

struct TranslateAnimation:RectAnimation
{
public:
    TranslateAnimation(shared_ptr<Rect> i_rect,double i_time,double i_start_time,float i_startX,float i_startY,float i_endX,float i_endY)noexcept;
    virtual  void updateAnimation( double i_time )noexcept override  final ;
private:

    float _startX;
    float _startY;
    float _endX;
    float _endY;
};



/**
 * AnimationManger 只能有一个
 */
struct AnimationManager
{
public :
    void addAnimation(const std::shared_ptr<AnimationInterface> &animation)noexcept;
    void updateAnimations(double i_timerOur)noexcept;
    void removeAnimations()noexcept ;

private :
    std::list<std::shared_ptr<AnimationInterface>> _animations;
};

