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

//update Plane position & orientation
void updatePlaneMatrix(Position3D& pl_pos,float deltaT, float time, const glm::vec3 &m, const glm::vec3 &r)
{
    // Update Euler angles
    //pl_pos.rotation.x += ROT_SPEED/2.0f * r.x * deltaT; // pitch
    pl_pos.rotation.y += ROT_SPEED * r.y * deltaT; // yaw
    //pl_pos.rotation.z += ROT_SPEED/2.0f * r.z * deltaT; // roll

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
    WorldMatrixPlane = glm::rotate(WorldMatrixPlane, pl_pos.rotation.z, glm::vec3(0, 0, 1)); // roll
}


/** Creates a view projection matrix, with near plane at 0.1, and far plane at 150.0, and
 aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
 vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
 defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
 elevation (Pitch), and <Rho> the roll.*/
glm::mat4 updateCam(float Ar, Position3D pl_pos,glm::mat4 playerUbo){

    const float FOVy = glm::radians(105.0f);
    const float nearPlane = 0.1f;
    const float farPlane = 150.f;
    const float camHeight = 20.0f;
    const float camDistZ = -10.0f;
    const float camDistX = -10.0f;

    bool model = false;

    if(model) {
        glm::vec3 target = pl_pos.origin;
        glm::vec3 cameraPosition = glm::vec3(pl_pos.origin.x + camDistX, pl_pos.origin.y + camHeight,pl_pos.origin.z + camDistZ);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);


        auto ortho = glm::ortho(-40.0f / 2, 40.0f, -30.0f / 2, 30.0f / 2, nearPlane, farPlane);
        ortho[1][1] *= -1;

        auto P = ortho;
        P = P * glm::rotate(glm::mat4(1), glm::radians(-23.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        return P * glm::lookAt(cameraPosition, target, up);
    }
    else
    {
        glm::vec3 target = pl_pos.origin;
        glm::vec3 cameraPosition = playerUbo * glm::vec4(0.0f,camHeight,camDistZ,1.0f);
        glm::vec3 up = glm::normalize(playerUbo * glm::vec4(0.0f,1.0f,0.0f,0.0f));

        return glm::scale(glm::mat4(1),glm::vec3(1,-1,1)) *
               glm::frustum(-Ar*nearPlane*tan(FOVy/2),Ar*nearPlane*tan(FOVy/2),-nearPlane*tan(FOVy/2),nearPlane*tan(FOVy/2),nearPlane,farPlane) *
               glm::lookAt(cameraPosition,target,up);
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