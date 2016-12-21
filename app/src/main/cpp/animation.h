//
// Created by cheng on 2016/12/16.
//

#include <ctime>
#include "transform_mat.h"

struct Animation
{

public:

    Animation(double i_time,double i_postpone=0.0)noexcept ;
    virtual void updateAnimation()= 0;

    bool _isEnd=false;

protected:

    double  _time_start=0.0;
    double  _time_end=0.0;
    double  _time_run=0.0;
    double  _postpone=0.0;
};

/**
 * 旋转动画
 */
struct RotateAnimaton:Animation
{
public:
    RotateAnimaton(double i_time,float i_angle,Matrix3X2* i_matrix,double i_postpone=0.0f)noexcept;
    virtual  void updateAnimation()noexcept override  final ;

private :
    float _angle;
    float _lastAngle=0.0f;

    Matrix3X2 *  _mat;
};

/**
 * 缩放动画
 */
struct ScaleAnimation:Animation
{
public :
    ScaleAnimation(double i_time,float i_k ,Matrix3X2* i_matrix,double i_postpone=0.0f)noexcept ;
    virtual  void updateAnimation()noexcept override  final ;
private :

    float _k;
    float _lastK=1.0f;

    Matrix3X2 * _mat;
};

/**
 * 平移动画
 */

struct TransformAnimation:Animation
{
public:
    TransformAnimation(double i_time,float i_x,float i_y ,Matrix3X2* i_matrix,double i_postpone=0.0f)noexcept;
    virtual  void updateAnimation()noexcept override  final ;
private:
    float _x;
    float _y;
    float _lastX=0.0f;
    float _lastY=0.0f;

    Matrix3X2 *_mat;
};

/**
 * AnimationManger 只能有一个
 */
struct AnimationManager
{
public :
    void addAnimation(const std::shared_ptr<Animation> &animation)noexcept;
    void updateAnimations()noexcept;
    void removeAnimations()noexcept ;

private :
    std::list<std::shared_ptr<Animation>> _animations;
};



