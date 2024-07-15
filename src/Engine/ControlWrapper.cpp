#ifndef CGPROJECT_2022_23_CONTROLWRAPER_H
#define CGPROJECT_2022_23_CONTROLWRAPER_H

#include "Starter.hpp"
#include "../../View/PlaneView.hpp"
#include "../../Model/Include/Partita.h"
#include <tuple>

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

//update Plane position & orientation
void updatePlaneMatrix(Position3D& pl_pos,float deltaT, float time, const glm::vec3 &m, const glm::vec3 &r)
{
    // Update Euler angles
    //pl_pos.rotation.x += ROT_SPEED/2.0f * r.x * deltaT; // pitch
    pl_pos.rotation.y += ROT_SPEED * r.y * deltaT; // yaw
    if(pl_pos.rotation.z + ROT_SPEED/2.0f * -r.y * deltaT <= M_PI/4 && pl_pos.rotation.z + ROT_SPEED/2.0f * -r.y * deltaT >= -M_PI/4)
        pl_pos.rotation.z += ROT_SPEED/2.0f * -r.y * deltaT; // roll

    pl_pos.rotation.z = glm::clamp<float>(pl_pos.rotation.z,-M_PI/4,M_PI/4);

    if(r.y == 0)
    {
        if(pl_pos.rotation.z < 0)
        {
            pl_pos.rotation.z += ROT_SPEED/2.0f * deltaT;
        }
        else if(pl_pos.rotation.z > 0)
        {
            pl_pos.rotation.z -= ROT_SPEED/2.0f * deltaT;
        }
    }

    // Ensure the rotations stay within the range 0 to 2π
    pl_pos.rotation.x = fmod(pl_pos.rotation.x, 2.0f * M_PI);
    pl_pos.rotation.y = fmod(pl_pos.rotation.y, 2.0f * M_PI);
    pl_pos.rotation.z = fmod(pl_pos.rotation.z, 2.0f * M_PI);

// Convert Euler angles to a quaternion
    glm::quat pitchQuat = glm::angleAxis(pl_pos.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // yaw
    glm::quat yawQuat = glm::angleAxis(pl_pos.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));   // Pitch
    glm::quat rollQuat = glm::angleAxis(pl_pos.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));  // roll

// Combine the quaternions
    glm::quat orientation = yawQuat * pitchQuat * rollQuat;

// Calculate direction vectors based on the orientation
    glm::vec3 forwardDirection = glm::normalize(orientation * glm::vec3(0.0f, 0.0f, 1.0f)); // Forward
    glm::vec3 rightDirection = glm::normalize(orientation * glm::vec3(1.0f, 0.0f, 0.0f));    // Right
    glm::vec3 upDirection = glm::normalize(orientation * glm::vec3(0.0f, 1.0f, 0.0f));       // Up

// Update position based on movement input
    pl_pos.origin += forwardDirection * MOVE_SPEED * 2.0f * deltaT; // Forward/Backward
    //pl_pos.origin += upDirection * MOVE_SPEED * m.y * deltaT;      // Up/Down
    //pl_pos.origin += rightDirection * MOVE_SPEED * m.x * deltaT;   // Left/Right
}


void updatePlaneMatrix(glm::mat4& WorldMatrixPlane,const Position3D& pl_pos)
{
    WorldMatrixPlane = glm::mat4(1.0f);
    WorldMatrixPlane = glm::translate(WorldMatrixPlane, glm::vec3(pl_pos.origin));

    WorldMatrixPlane = glm::rotate(WorldMatrixPlane, pl_pos.rotation.y, glm::vec3(0, 1, 0)); // pitch
    WorldMatrixPlane = glm::rotate(WorldMatrixPlane, pl_pos.rotation.x, glm::vec3(1, 0, 0)); // yaw
    //WorldMatrixPlane = glm::rotate(WorldMatrixPlane, pl_pos.rotation.z, glm::vec3(0, 0, 1)); // roll
}


/** Creates a view projection matrix, with near plane at 0.1, and far plane at 150.0, and
 aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
 vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
 defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
 elevation (Pitch), and <Rho> the roll.
 Return the Proj-View matrix , the Proj matrix and the View matrix*/
std::tuple<glm::mat4,glm::mat4,glm::mat4> updateCam(float Ar, Position3D pl_pos,glm::mat4 playerUbo,bool isFirstPerson){

    const float FOVy = glm::radians(105.0f);
    const float nearPlane = 0.1f;
    const float farPlane = 150.f;
    const float camHeight = 20.0f;
    const float camDistZ = -10.0f;

    //First Person
    if(isFirstPerson)
    {
        // Threshold value to determine when to force rotation component to zero
        const float rotationThreshold = 0.06f;

        // Ensure pl_pos.rotation is within acceptable range to avoid numerical instability
        if (std::abs(pl_pos.rotation.x) < rotationThreshold) {
            pl_pos.rotation.x = 0.0f;
        }
        if (std::abs(pl_pos.rotation.y) < rotationThreshold) {
            pl_pos.rotation.y = 0.0f;
        }
        if (std::abs(pl_pos.rotation.z) < rotationThreshold) {
            pl_pos.rotation.z = 0.0f;
        }

        glm::mat4 Proj = glm::scale(glm::mat4(1),glm::vec3(1,-1,1)) *
                         glm::frustum(-Ar*nearPlane*tan(FOVy/2),Ar*nearPlane*tan(FOVy/2),-nearPlane*tan(FOVy/2),nearPlane*tan(FOVy/2),nearPlane,farPlane);
        glm::mat4 View = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,1.6f)) *
                         glm::rotate<float>(glm::mat4(1.0f),M_PI,glm::vec3(0.0f,1.0f,0.0f)) *
                         glm::rotate(glm::mat4(1.0f),-pl_pos.rotation.z,glm::vec3(0.0f,0.0f,1.0f)) *
                         glm::rotate(glm::mat4(1.0f),-pl_pos.rotation.x,glm::vec3(1.0f,0.0f,0.0f)) *
                         glm::rotate(glm::mat4(1.0f),-pl_pos.rotation.y,glm::vec3(0.0f,1.0f,0.0f)) *
                         glm::translate(glm::mat4(1.0f),-pl_pos.origin);

        return {Proj*View,Proj,View};
    }

    // Third Person
    glm::vec3 target = pl_pos.origin;
    glm::vec3 cameraPosition = playerUbo * glm::vec4(0.0f,camHeight,camDistZ,1.0f);
    glm::vec3 up = glm::normalize(playerUbo * glm::vec4(0.0f,1.0f,0.0f,0.0f));
    glm::mat4 Proj = glm::scale(glm::mat4(1),glm::vec3(1,-1,1)) *
                     glm::frustum(-Ar*nearPlane*tan(FOVy/2),Ar*nearPlane*tan(FOVy/2),-nearPlane*tan(FOVy/2),nearPlane*tan(FOVy/2),nearPlane,farPlane);
    glm::mat4 View = glm::lookAt(cameraPosition,target,up);
    return {Proj*View,Proj,View};
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