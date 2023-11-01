//
// Created by lucas on 22/10/23.
//

#ifndef LITHO_3D___SHADOWMAP_H
#define LITHO_3D___SHADOWMAP_H

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap();

    virtual bool  Init(GLuint width, GLuint height);

    virtual void Write();

    virtual void Read(GLenum textureUnit);

    ~ShadowMap();

private:
    GLuint FBO, shadowMap;
    GLuint shadowWidth, shadowHeight;
};



#endif //LITHO_3D___SHADOWMAP_H
