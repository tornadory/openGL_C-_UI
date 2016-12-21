//
// Created by cheng on 2016/12/7.
//

#pragma once

#include "transform_mat.h"
#include "animation.h"

struct RenderContext
{
public:

    struct RectProgram : GLProgram
    {
    public:

        RectProgram()= default;
        RectProgram(const GLShader& i_vertextShader,const GLShader & i_fragmentShader)noexcept;

        void load()noexcept ;
        void bindPos(uint32_t i_stride,uint32_t i_offset)noexcept;
        void setColor(const array<float,4> & i_color)noexcept ;
        void setTransform(const Matrix3X2 & i_mat)noexcept ;

    private:
        GLuint a_pos;
        GLuint u_color;
        GLuint u_transform;
    };


    RenderContext();

    void createSurface(NativeWindowType i_type,float i_width,float i_height)noexcept ;
    bool hasSurface()noexcept ;
    void destorySurface()noexcept ;
    void createContext()noexcept;
    bool isHasContext()noexcept;
    void CreateRectProgram()noexcept;
    void makeCurrent() const noexcept;
    void unbindThread()noexcept ;
    void initRectBuffer()noexcept;
    void initRectBuffer2()noexcept;
    void draw()noexcept;
    void setAnimation()noexcept;
    void updateAnimation() noexcept ;

private:

    GLDisplay _display;
    GLConfig _config;
    GLSurface _surface;
    GLContext _context;
    RectProgram _rectProgram;

    GLBuffer _RectBuffer;
    GLBuffer _RectBuffer2;

    Matrix3X2 _mat;
    Matrix3X2 _mat2;
    AnimationManager _animationManger;

};
