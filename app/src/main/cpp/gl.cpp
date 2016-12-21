//
// Created by cheng on 2016/11/23.
//

#include "common.h"

//GLDisplay=======
GLDisplay::GLDisplay(EGLNativeDisplayType i_display_id)noexcept
{
    _display=eglGetDisplay(nullptr);

    if(_display != EGL_NO_DISPLAY)
    {
        GLint majorVer,minorVer;

        eglInitialize(_display,&majorVer,&minorVer);


        if(majorVer<1||majorVer==1&&minorVer<3)
        {
            _display=EGL_NO_DISPLAY;

            return;
        }
    }
}
GLDisplay& GLDisplay::operator=(GLDisplay &&i_display)
{
    this->_display=i_display._display;
    return *this;

}

 GLDisplay::operator EGLDisplay() const noexcept
{
    return _display;
}

//GLConfig=======

GLConfig::operator EGLConfig() const noexcept
{
    return _config;
}

//GLSurface========
GLSurface::GLSurface(GLDisplay& i_display, GLConfig& i_config, NativeWindowType i_window) noexcept :_display(i_display)
{
    _surface=eglCreateWindowSurface(i_display,i_config,i_window, nullptr);
}
GLSurface& GLSurface::operator=(GLSurface &&i_right)noexcept
{
    this->~GLSurface();

    return *new(this) GLSurface(std::move(i_right));
}

GLSurface::GLSurface(GLSurface &&i_right)noexcept :_display(i_right._display),_surface(i_right._surface)
{
    i_right._surface=EGL_NO_SURFACE;
}

GLSurface::~GLSurface()
{
    if(_surface!=EGL_NO_SURFACE)
    {
        eglDestroySurface(_display,_surface);
    }
}

bool GLSurface::swap() const noexcept
{
    return eglSwapBuffers(_display,_surface)==EGL_TRUE;
}

GLSurface::operator EGLSurface() const noexcept
{
    return _surface;
}

bool GLSurface::isHasSurfurface()noexcept
{
    return !(_surface==EGL_NO_SURFACE);
}


//GLContext======
GLContext::GLContext(const GLDisplay &i_display, const GLConfig &i_config,
                     const GLContext &i_glContext)noexcept :_display(i_display)
{
    GLint attr[]={EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};

    _context = eglCreateContext(i_display,i_config,i_glContext,attr);
}

GLContext::~GLContext()
{
    eglDestroyContext(_display,_context);
}

GLContext::GLContext(GLContext &&i_right)noexcept :_display(i_right._display),_context(i_right._context)
{
    i_right._context=EGL_NO_CONTEXT;
}


GLContext& GLContext::operator=(GLContext &&i_right)noexcept
{
    this->~GLContext();

    return *new(this)GLContext(std::move(i_right));
}

GLContext::operator EGLContext() const noexcept
{

    return _context;
}

bool GLContext::hasContext() noexcept
{
    return !(_context==EGL_NO_CONTEXT);
}


//GLShader=======
GLShader::GLShader(const GLenum i_type, const std::string &i_source)noexcept :_shader(glCreateShader(i_type))
{
    const GLchar *p=&i_source[0];
    GLint size=(GLint)i_source.size();

    glShaderSource(_shader,1,&p,&size);
    glCompileShader(_shader);

    if(compileStatus()==GL_FALSE)
    {
        compileLog();
    }

}

GLShader::GLShader(GLShader &&i_right)noexcept :_shader(i_right._shader)
{
    i_right._shader=0;
}

GLShader& GLShader::operator=(GLShader &&i_right)noexcept
{
    this->~GLShader();

    return *new(this) GLShader(std::move(i_right));
}

GLShader::~GLShader()
{
    glDeleteShader(_shader);
}

GLShader:: operator GLuint () const noexcept
{
    return _shader;
}

GLuint GLShader::compileStatus() const noexcept
{
    GLint status;

    glGetShaderiv(_shader,GL_COMPILE_STATUS,&status);

    return status;
}

std::string GLShader::compileLog() const noexcept
{
    GLint  length;

    glGetShaderiv(_shader,GL_INFO_LOG_LENGTH,&length);

    if(length==0)
    {
        return string();
    }

    auto buf=make_unique<GLchar []>(length);

    glGetShaderInfoLog(_shader,length,&length,buf.get());



    return std::string(buf.get(),length);
}
//GLProgram

GLProgram::GLProgram(const GLShader &i_vertextShader, const GLShader &i_fragmentShader)noexcept :_program(glCreateProgram())
{
    glAttachShader(_program,i_vertextShader);
    glAttachShader(_program,i_fragmentShader);

    glLinkProgram(_program);

    glDetachShader(_program,i_vertextShader);
    glDetachShader(_program,i_fragmentShader);
}

GLProgram::GLProgram(GLProgram &&i_right)noexcept :_program(i_right._program)
{
    i_right._program=0;
}
GLProgram& GLProgram::operator=(GLProgram &&i_right)noexcept
{
    this->~GLProgram();

    return * new(this) GLProgram(std::move(i_right));
}

GLProgram::~GLProgram()
{
    glDeleteProgram(_program);
}

GLint GLProgram::getAttributeLocation(const std::string & i_name) const noexcept
{
    return glGetAttribLocation(_program,i_name.c_str());
}

GLint GLProgram::getUniformLocation(const std::string &i_name) const noexcept
{
    return glGetUniformLocation(_program,i_name.c_str());
}

void GLProgram::use() const  noexcept
{

    glUseProgram(_program);
}

GLint GLProgram::linkStatus() const noexcept
{

    GLint status;

    glGetProgramiv(_program,GL_LINK_STATUS,&status);

    return status;
}

std::string GLProgram::linkLog() const noexcept
{

    GLint length;

    glGetProgramiv(_program,GL_INFO_LOG_LENGTH,&length);

    if(length==0)
    {
        return std::string();
    }

    auto buf=make_unique<GLchar []>(length);
    glGetProgramInfoLog(_program,length,&length,buf.get());

    return std::string(buf.get(),length);
}

//GLBuffer

GLBuffer::GLBuffer(GLBuffer &&i_right)noexcept :_buffer(i_right)
{
    i_right._buffer=0;
}

GLBuffer &GLBuffer::operator=(GLBuffer &&i_right) noexcept
{
    this->~GLBuffer();

    return *new(this) GLBuffer(std::move(i_right));
}

GLBuffer::~GLBuffer() {

    glDeleteBuffers(1,&_buffer);
}

GLBuffer::operator GLuint() noexcept
{
    return _buffer;
}




void GLBuffer::bind(GLenum i_attachment) const noexcept
{

    glBindBuffer(i_attachment,_buffer);
}

//GLTexture
GLTexture::GLTexture(GLTexture &&i_right) noexcept :_texture(i_right._texture)
{
    i_right._texture=0;
}

GLTexture &GLTexture::operator=(GLTexture &&i_right) noexcept
{

    this->~GLTexture();

    return *(new(this) GLTexture(move(i_right)) );
}

GLTexture::operator GLuint () noexcept {
    return _texture;
}
GLTexture::~GLTexture()
{
    glDeleteTextures(1,&_texture);
}

void GLTexture::initialize(uint32_t i_width, uint32_t i_height, GLenum i_format,
                           const void *i_pixel, GLenum i_srcFormat) const noexcept {

    glBindTexture(GL_TEXTURE_2D,_texture);
    glTexImage2D(GL_TEXTURE_2D,0,i_format,i_width,i_height,0,i_srcFormat,GL_UNSIGNED_BYTE,i_pixel);

}

GLTexture GLTexture::create(uint32_t i_width, uint32_t i_height, GLenum i_format,
                            const void *i_pixel, GLenum i_srcFormat)noexcept {
    GLTexture _glTexture;

    glGenTextures(1,(GLuint *)&_glTexture);
    _glTexture.initialize(i_width,i_height,i_format,i_pixel,i_srcFormat);

    return std::move(_glTexture);
}

void GLTexture::selectSlot(uint32_t i_slot) noexcept {

    glActiveTexture(GL_TEXTURE0+i_slot);
}

void GLTexture::bind() noexcept {

    glBindTexture(GL_TEXTURE_2D,_texture);
}

void GLTexture::bind(uint32_t i_slot) noexcept {

    glActiveTexture(GL_TEXTURE0+i_slot);
    glBindTexture(GL_TEXTURE_2D,_texture);

}

void GLTexture::setFilters(GLenum i_min, GLenum i_mag) const noexcept
{
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,i_min);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,i_mag);
}

void GLTexture::setWarp(GLenum i_h, GLenum i_v) const noexcept
{
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,i_h);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,i_v);
}

void GLTexture::generateMipmap() const noexcept
{
    glGenerateMipmap(GL_TEXTURE_2D);
}
























