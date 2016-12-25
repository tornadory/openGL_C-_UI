//
// Created by cheng on 2016/11/23.
//
#include "common.h"
#include "render_context.h"
#include "transform_mat.h"

RenderContext::RectProgram::RectProgram(const GLShader &i_vertextShader,
                                        const GLShader &i_fragmentShader) noexcept :GLProgram(i_vertextShader,i_fragmentShader)
{

}

void RenderContext::RectProgram::load() noexcept
{
    new (this) RectProgram(GLShader(GL_VERTEX_SHADER,
                                    "precision highp float; attribute vec2 a_pos;  uniform vec2 u_transform[3];"
                                        "vec2 mul(vec2 v, vec2 trans[3])"
                                        "{"
                                        "    return trans[0] * v.x + trans[1] * v.y + trans[2];"
                                        "}"
            "void main()"
            "{"
            "    gl_Position = vec4(mul(a_pos,u_transform), 0.0f, 1.0f);"
            "}"),GLShader(GL_FRAGMENT_SHADER,"precision mediump float;"
            "uniform vec4 u_color;"
            "void main()"
            "{"
            "gl_FragColor = u_color;"
            "}"));

#define get__(name) a_##name=getAttributeLocation("a_"#name);

    get__(pos);

#undef  get__

#define get__(name) u_##name=getUniformLocation("u_"#name);

    get__(color);
    get__(transform);

#undef get__

}

void RenderContext::RectProgram::bindPos(uint32_t i_stride, uint32_t i_offset) noexcept
{
    glVertexAttribPointer(a_pos,2,GL_FLOAT,GL_FALSE, sizeof(float)*i_stride,(void *) (i_offset* sizeof(float)));
    glEnableVertexAttribArray(a_pos);
}

void RenderContext::RectProgram::setColor(const array<float, 4> & i_color) noexcept
{
    glUniform4fv(u_color,1,&i_color[0]);
}

void RenderContext::RectProgram::setTransform(const Matrix3X2 &i_mat)noexcept
{
    glUniform2fv(u_transform,3,i_mat.getElement());
}


RenderContext::RenderContext() {

    _display=GLDisplay(EGL_DEFAULT_DISPLAY);

    auto config=GLConfig::config(_display,
                                 EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
                                 EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
                                 EGL_RED_SIZE,8,
                                 EGL_BLUE_SIZE,8,
                                 EGL_GREEN_SIZE,8);

    if(!config.empty())
    {
        _config=config[0];
    }
}

void RenderContext::createSurface(NativeWindowType i_type,float i_width,float i_height)noexcept
{
    unbindThread();

    _surface=GLSurface(_display,_config,i_type);

    if(_context)
    {
        makeCurrent();
    }
}

void RenderContext::createContext()noexcept
{
    _context=GLContext(_display,_config);

    if(_surface)
    {
        makeCurrent();
    }
}

void RenderContext::CreateRectProgram()noexcept
{
    _rectProgram.load();
}

void RenderContext::initRectBuffer()noexcept
{
    array<float,8> v= {

            -1.0f,1.0f,
            -1.0f,-1.0f,
            1.0f,1.0f,
            1.0f,-1.0f

    };
    _RectBuffer=_RectBuffer2.create(&v[0],8);
}

void RenderContext::initRectBuffer2()noexcept
{
    array<float,8> v= {

           0.0f,0.0f,
           1.0f,0.0f,
           0.0f,1.0f,
           1.0f,1.0f

    };

    _RectBuffer2=_RectBuffer2.create(&v[0],8);
}

void RenderContext::makeCurrent() const noexcept
{
    eglMakeCurrent(_display, _surface, _surface, _context);
}

bool RenderContext::hasSurface() noexcept
{
    return _surface.isHasSurfurface();
}

bool RenderContext::isHasContext() noexcept
{
    return _context.hasContext();
}

void RenderContext::destorySurface() noexcept
{
    unbindThread();

    _surface = GLSurface();
}

void RenderContext::unbindThread()noexcept
{
    eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

RenderContext::RectProgram &RenderContext::getGLProgam() noexcept
{
    return _rectProgram;
}

GLBuffer &RenderContext::getBuffer1()noexcept
{
    return _RectBuffer;
}

GLBuffer &RenderContext::getBuffer2()noexcept
{
    return _RectBuffer2;
}

GLSurface &RenderContext::getSurface() noexcept
{
    return _surface;
}



































