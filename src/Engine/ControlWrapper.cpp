#ifndef CGPROJECT_2022_23_CONTROLWRAPER_H
#define CGPROJECT_2022_23_CONTROLWRAPER_H

#include "Starter.hpp"
#include "../../View/PlaneView.hpp"
#include "../../Model/Include/Partita.h"

enum DIR
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};


enum Planes
{
    Left = 0,
    Right,
    Bottom,
    Top,
    Near,
    Far,
};


glm::mat4 generateYRotation(float degree)
{
    return glm::mat4(glm::cos(degree), 0, -glm::sin(degree), 0,
                     0, 1, 0, 0,
                     glm::sin(degree), 0, glm::cos(degree), 0,
                     0, 0, 0, 1);
}

glm::mat4 generateXRotation(float degree)
{
    return glm::mat4(1, 0, 0, 0,
                     0, glm::cos(degree), glm::sin(degree), 0,
                     0, -glm::sin(degree), glm::cos(degree), 0,
                     0, 0, 0, 1);
}

/** Creates a view projection matrix, with near plane at 0.1, and far plane at 150.0, and
 aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
 vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
 defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
 elevation (Pitch), and <Rho> the roll.*/
glm::mat4 updateCam(float Ar, float deltaT, glm::vec3 m, glm::vec3 r, bool model,PlaneView* planes,Partita* partita){

    const float FOVy = glm::radians(105.0f);
    const float nearPlane = 0.1f;
    const float farPlane = 150.f;
    const float camHeight = 2.5;
    const float camDist = 7;
    const float ROT_SPEED = glm::radians(120.0f);
    const float MOVE_SPEED = 4.0f;

    static float CamYaw=0.0f , CamPitch=0.0f, CamRoll=0.0f;
    static glm::vec3 CamPos = glm::vec3(1.0f);

    CamYaw = CamYaw - ROT_SPEED * deltaT * r.y;
    CamPitch  = CamPitch - ROT_SPEED * deltaT * r.x;
    CamPitch  = CamPitch < glm::radians(-90.0f) ? glm::radians(-90.0f) :
                (CamPitch > glm::radians(90.0f) ? glm::radians(90.0f) : CamPitch);
    CamRoll   = CamRoll - ROT_SPEED * deltaT * r.z;
    CamRoll   = CamRoll < glm::radians(-180.0f) ? glm::radians(-180.0f) :
                (CamRoll > glm::radians(180.0f) ? glm::radians(180.0f) : CamRoll);

    glm::mat3 CamDir = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) *
                       glm::rotate(glm::mat4(1.0f), CamPitch, glm::vec3(1, 0, 0)) *
                       glm::rotate(glm::mat4(1.0f), CamRoll, glm::vec3(0, 0, 1));

    glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
    glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

    CamPos += MOVE_SPEED * m.x * ux * deltaT;
    CamPos += MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
    CamPos += MOVE_SPEED * m.z * uz * deltaT;


    if(model)
    {
        glm::vec3 centerCam = planes->playerInfo->ubo.model * glm::vec4(0.0f,15.0f,-20.0f,1.0f);
        glm::vec3 up = glm::normalize(planes->playerInfo->ubo.model * glm::vec4(0.0f,1.0f,0.0f,0.0f));
        return glm::scale(glm::mat4(1),glm::vec3(1,-1,1)) *
                glm::frustum(-Ar*nearPlane*tan(FOVy/2),Ar*nearPlane*tan(FOVy/2),-nearPlane*tan(FOVy/2),nearPlane*tan(FOVy/2),nearPlane,farPlane) *
                glm::lookAt(centerCam,glm::vec3(partita->getPlayer()->getPosition().origin), up);
    }
    else
    {
        //LookIn model
        return glm::scale(glm::mat4(1),glm::vec3(1,-1,1)) *
               glm::frustum(-Ar*nearPlane*tan(FOVy/2),Ar*nearPlane*tan(FOVy/2),-nearPlane*tan(FOVy/2),nearPlane*tan(FOVy/2),nearPlane,farPlane) *
               glm::rotate(glm::mat4(1), -CamRoll, glm::vec3(0, 0, 1)) *
               glm::rotate(glm::mat4(1), -CamPitch, glm::vec3(1, 0, 0)) *
               glm::rotate(glm::mat4(1), -CamYaw, glm::vec3(0, 1, 0)) *
               glm::translate(glm::mat4(1),-CamPos) *
               glm::translate(glm::mat4(1),glm::vec3(0,-camHeight,-camDist));

    }
}

bool sphereInFrustum(glm::vec4* m_planes, glm::vec3 point, float radius)
{
    for (uint32_t i = 0; i < 6; i++) {
        if (m_planes[i][0] * point.x + m_planes[i][1] * point.y + m_planes[i][2] * point.z + m_planes[i][3] <= -radius) {
            return false;
        }
    }
    return true;
}

void extractFrustumPlanes(glm::vec4* m_planes, glm::mat4 m)
{
    m = glm::transpose(m);
    m_planes[Left]   = m[3] + m[0];
    m_planes[Right]  = m[3] - m[0];
    m_planes[Bottom] = m[3] + m[1];
    m_planes[Top]    = m[3] - m[1];
    m_planes[Near]   = m[3] + m[2];
    m_planes[Far]    = m[3] - m[2];
}





#endif