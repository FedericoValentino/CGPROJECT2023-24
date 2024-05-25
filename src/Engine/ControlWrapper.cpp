#ifndef CGPROJECT_2022_23_CONTROLWRAPER_H
#define CGPROJECT_2022_23_CONTROLWRAPER_H

#include "Starter.hpp"

enum DIR
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
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

/** Creates a view projection matrix, with near plane at 0.1, and far plane at 50.0, and
 aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
 vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
 defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
 elevation (Pitch), and <Rho> the roll.*/
glm::mat4 updateCam(float Ar, float deltaT, glm::vec3 m, glm::vec3 r, bool model){
    const float FOVy = glm::radians(105.0f);
    const float nearPlane = 0.1f;
    const float farPlane = 100.f;
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
        return glm::scale(glm::mat4(1),glm::vec3(1,-1,1)) *
                glm::frustum(-Ar*nearPlane*tan(FOVy/2),Ar*nearPlane*tan(FOVy/2),-nearPlane*tan(FOVy/2),nearPlane*tan(FOVy/2),nearPlane,farPlane) *
                //generateXRotation(glm::radians(30.0f)) *
                //generateYRotation(glm::radians(60.0f)) *
                glm::rotate(glm::mat4(1.0), -CamRoll, glm::vec3(0,0,1)) *
                glm::lookAt(CamPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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

int getNewSquare(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    if(glfwGetKey(window, GLFW_KEY_I))
    {
        return UP;
    }
    if(glfwGetKey(window, GLFW_KEY_K))
    {
        return DOWN;
    }
    if(glfwGetKey(window, GLFW_KEY_J))
    {
        return LEFT;
    }
    if(glfwGetKey(window, GLFW_KEY_L))
    {
        return RIGHT;
    }

    return NONE;
}






#endif