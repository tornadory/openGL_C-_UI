//
// Created by cheng on 2016/11/23.
//
#pragma once

struct GLDisplay
{
    GLDisplay()= default;
    GLDisplay(EGLNativeDisplayType i_display_id) noexcept ;
    GLDisplay& operator =(GLDisplay &&i_display);
    operator EGLDisplay() const noexcept ;

private:
    EGLDisplay _display=EGL_NO_DISPLAY;
};

struct GLConfig
{
    GLConfig()= default;

    template <class ...Attributes>
    static std::vector<GLConfig> config(GLDisplay &i_display,Attributes ...i_attributes)noexcept ;

    operator EGLConfig () const noexcept ;

private:
    EGLConfig  _config= nullptr;
};

struct GLSurface
{
public:
    GLSurface()= default;
    GLSurface(GLDisplay& i_display,GLConfig& i_config,NativeWindowType i_window) noexcept ;
    ~GLSurface() ;
    GLSurface& operator =(GLSurface &&i_right)noexcept ;
    GLSurface(GLSurface &&i_right)noexcept ;
    bool swap() const noexcept ;
    operator EGLSurface () const noexcept ;
    bool isHasSurfurface()noexcept ;

private :

    EGLDisplay _display;
    EGLSurface _surface=EGL_NO_SURFACE;

};

struct GLContext
{
public:
    GLContext()= default;
    GLContext(const GLDisplay &i_display,const GLConfig &i_config,const GLContext &i_glContext=GLContext() )noexcept;
    GLContext(GLContext &&i_right)noexcept;
    GLContext& operator =(GLContext&& i_right)noexcept ;
    operator EGLContext () const noexcept ;
    bool hasContext()noexcept ;
    ~GLContext();
private :
    EGLDisplay  _display;
    EGLContext _context=EGL_NO_CONTEXT;
};

struct GLShader
{

public:
    GLShader()= default;
    GLShader(const GLenum i_type,const std::string & i_source)noexcept ;


    GLShader(GLShader && i_right)noexcept ;
    GLShader& operator=(GLShader && i_right) noexcept;

    ~GLShader();
    operator GLuint () const noexcept ;

    GLuint compileStatus() const noexcept ;
    std::string compileLog() const noexcept ;

private:
    GLuint _shader=0;
};

struct GLProgram
{
public:
    GLProgram()= default;
    GLProgram(const GLShader& i_vertextShader,const GLShader & i_fragmentShader)noexcept;

    GLProgram(GLProgram && i_right) noexcept ;
    GLProgram & operator=(GLProgram && i_right) noexcept ;

    ~GLProgram();

    GLint linkStatus()const noexcept ;
    std::string linkLog() const noexcept ;

    GLint getAttributeLocation(const std::string & i_name) const noexcept ;
    GLint getUniformLocation(const std::string & i_name) const noexcept ;

    void use() const noexcept ;

private:
    GLuint _program=0;
};

struct GLBuffer
{
public:

    GLBuffer()= default;
    GLBuffer(GLBuffer&& i_right)noexcept ;
    GLBuffer &operator =(GLBuffer &&i_right)noexcept ;

    ~GLBuffer();

    operator GLuint() noexcept;

    template <class T>
    static GLBuffer create(const T *i_t,size_t i_count,GLenum i_usage=GL_STATIC_DRAW,GLenum i_attachment=GL_ARRAY_BUFFER)noexcept ;

    template<class T>
    void initialize(const T *i_t,size_t i_count,GLenum i_usage=GL_STATIC_DRAW,GLenum i_attachment=GL_ARRAY_BUFFER)noexcept ;

    void bind(GLenum i_attachment=GL_ARRAY_BUFFER) const noexcept ;

    template <class T>
    void upload(const T *i_t,size_t i_count,size_t i_offset,GLenum i_usage=GL_STATIC_DRAW,GLenum i_attachment=GL_ARRAY_BUFFER) noexcept ;

private:

    GLuint _buffer;

};

struct GLTexture
{
public:
    GLTexture()= default;
    GLTexture(GLTexture &&i_right)noexcept ;
    GLTexture & operator = (GLTexture && i_right) noexcept ;
    ~GLTexture();
    operator GLuint() noexcept ;

    void initialize(uint32_t i_width,uint32_t i_height,GLenum i_format,const void * i_pixel ,GLenum i_srcFormat) const noexcept ;
    static GLTexture create(uint32_t i_width,uint32_t i_height,GLenum i_format,const void * i_pixel,GLenum i_srcFormat)noexcept ;

    void selectSlot(uint32_t i_slot)noexcept ;
    void bind()noexcept ;
    void bind(uint32_t i_slot) noexcept;

    void setFilters(GLenum i_min,GLenum i_mag)const noexcept;
    void setWarp(GLenum i_h,GLenum i_v)const noexcept;

    void generateMipmap() const noexcept ;

private:
    GLuint _texture=0;
};

template <class ...Attributes>
std::vector<GLConfig> GLConfig::config(GLDisplay &i_display, Attributes ...i_attributes) noexcept
{
    std::array<EGLint,sizeof...(i_attributes)+1> arr={EGLint(i_attributes)...,EGL_NONE};

    EGLint n;

    if(eglChooseConfig(i_display,&arr[0], nullptr,0,&n))
    {
        std::vector<GLConfig> r(n);

        eglChooseConfig(i_display,&arr[0],(EGLConfig *)&r[0],n,&n);

        return r;
    }

    return std::vector<GLConfig>();
}

template<class T>
void GLBuffer::initialize(const T *i_t, size_t i_count, GLenum i_usage, GLenum i_attachment) noexcept
{

    glBindBuffer(i_attachment,_buffer);
    glBufferData(i_attachment, (GLsizeiptr)(i_count*sizeof(T)),i_t,i_usage);
}

template<class T>
void GLBuffer::upload(const T *i_t, size_t i_count, size_t i_offset,GLenum i_usage, GLenum i_attachment)noexcept
{

    glBufferSubData(i_attachment,i_offset,(GLsizeiptr)(sizeof(T)*i_count),i_t);
}



template<class T>
GLBuffer GLBuffer::create(const T *i_t, size_t i_count, GLenum i_usage, GLenum i_attachment) noexcept
{

    GLBuffer r;

    glGenBuffers(1,&r._buffer);

    r.initialize(i_t,i_count);

    return std::move(r);

}