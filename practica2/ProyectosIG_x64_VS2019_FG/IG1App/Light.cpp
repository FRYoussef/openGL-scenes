#include "Light.h"
#include <gtc/type_ptr.hpp>

GLuint Light::cont = 0;

Light::Light() { 
    if(cont < GL_MAX_LIGHTS) { 
        id = GL_LIGHT0 + cont; 
        ++cont;
        glEnable(id);
    }
}

void Light::uploadL() { 
    // Transfiere las características de la luz a la GPU
    glLightfv(id, GL_AMBIENT, value_ptr(ambient));
    glLightfv(id, GL_DIFFUSE, value_ptr(diffuse));
    glLightfv(id, GL_SPECULAR, value_ptr(specular));
}

void DirLight::upload(glm::dmat4 const& modelViewMat) const{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(value_ptr(modelViewMat));
    glLightfv(id, GL_POSITION, value_ptr(posDir));
    Light::uploadL();
}

void PosLight::upload(glm::dmat4 const& modelViewMat) const{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(value_ptr(modelViewMat));
    glLightfv(id, GL_POSITION, value_ptr(posDir));
    glLightf(id, GL_CONSTANT_ATTENUATION, kc);
    glLightf(id, GL_LINEAR_ATTENUATION, kl);
    glLightf(id, GL_QUADRATIC_ATTENUATION, kq);
    Light::uploadL();
}

void PosLight::setAtte(GLfloat kc, GLfloat kl, GLfloat kq){
    this->kc = kc;
    this->kl = kl;
    this->kq = kq;
}

void SpotLight::upload(glm::dmat4 const& modelViewMat) const{
    PosLight::upload(modelViewMat);
    glLightfv(id, GL_SPOT_DIRECTION, value_ptr(direction));
    glLightf(id, GL_SPOT_CUTOFF, cutoff);
    glLightf(id, GL_SPOT_EXPONENT, exp);
}

// Ojo al 0.0: la direccion de emision del foco es vector
void SpotLight::setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e) {
    direction = glm::fvec4(dir, 0.0);
    cutoff = cf;
    exp = e;
}

void Material::upload() {
    glMaterialfv(face, GL_AMBIENT, value_ptr(ambient));
    glMaterialfv(face, GL_DIFFUSE, value_ptr(diffuse));
    glMaterialfv(face, GL_SPECULAR, value_ptr(specular));
    glMaterialf(face, GL_SHININESS, expF);
    glShadeModel(sh);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);  // Defecto
}

void Material::setCopper() {
    ambient = {0.19125, 0.0735, 0.0225, 1};
    diffuse = {0.7038, 0.27048, 0.0828, 1};
    specular = {0.256777, 0.137622, 0.086014, 1};
    expF = 12.8;
}

void Material::setGold() {
    ambient = {0.24725, 0.1995, 0.0745, 1.0};
    diffuse = {0.75164, 0.60648, 0.22648, 1.0};
    specular = {0.628281, 0.555802, 0.366065, 1.0};
    expF = 51.2;
}