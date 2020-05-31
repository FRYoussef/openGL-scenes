#ifndef _H_Light_H_
#define _H_Light_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include <vector>
#include <gtc/type_ptr.hpp>

class Light {
    protected:
        static GLuint cont; // Atributo para poder generar un nuevo id cada vez
        GLuint id = GL_LIGHT0 + GL_MAX_LIGHTS; // Primer id no valido
        glm::fvec4 ambient = {0.1, 0.1, 0.1, 1};
        glm::fvec4 diffuse = {0.5, 0.5, 0.5, 1};
        glm::fvec4 specular = {0.5, 0.5, 0.5, 1};
        glm::fvec4 posDir= {0, 0, 1, 0};

    public:
        Light();
        virtual ~Light() { disable(); cont--; }
        void uploadL(); // Metodo abstracto
        virtual void upload(glm::dmat4 const& modelViewMat) = 0;
        void disable() {if(id < GL_LIGHT0 + GL_MAX_LIGHTS) glDisable(id);};
        void enable() {if(id < GL_LIGHT0 + GL_MAX_LIGHTS) glEnable(id);};
        void setAmbient(glm::fvec4 amb) { ambient = amb; uploadL();};
        void setDiffuse(glm::fvec4 diff) { diffuse = diff; uploadL();};
        void setSpecular(glm::fvec4 spec) { specular = spec; uploadL();};
};

class DirLight: public Light { 
    public: 
        virtual void upload(glm::dmat4 const& modelViewMat);
        // Ojo al 0.0 que determina que la luz sea remota
        void setPosDir(glm::fvec3 dir) { posDir= glm::fvec4(dir, 0.0); };
};

class PosLight: public Light {
    protected:
        // Factores de atenuación   
        GLfloat kc = 1, kl = 0, kq= 0;  
    public:
        virtual void upload(glm::dmat4 const& modelViewMat);
        void setPosDir(glm::fvec3 dir) { posDir= glm::fvec4(dir, 1.0); };
        void setAtte(GLfloat kc, GLfloat kl, GLfloat kq);
};

class SpotLight: public PosLight{
    protected:
        // Atributos del foco
        glm::fvec4 direction = { 0, 0, -1, 0 };
        GLfloat cutoff= 180;
        GLfloat exp= 0;
    public: 
        SpotLight(glm::fvec3 pos = { 0, 0, 0 }) : PosLight() {posDir = glm::fvec4(pos, 1.0);};
        virtual void upload(glm::dmat4 const& modelViewMat);
        void setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e);
};

class Material {
    protected:
        // Coeficientes de reflexion
        glm::fvec4 ambient = {0.2, 0.2, 0.2, 1.0};
        glm::fvec4 diffuse = {0.8, 0.8, 0.8, 1.0};
        glm::fvec4 specular = {0.0, 0.0, 0.0, 1.0};
        GLfloat expF = 0;  
        GLuint face = GL_FRONT_AND_BACK; // Exponente para la reflexion especular
        GLuint sh = GL_SMOOTH; // Tipo de matizado 
    public:
        Material() {};
        virtual ~Material() {};
        virtual void upload();
        void setCopper();
        void setGold();
};

#endif