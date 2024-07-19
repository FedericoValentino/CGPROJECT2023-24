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
Position3D updatePlaneMatrix(Position3D pl_pos,float deltaT, const glm::vec3 &r)
{
    // Update Euler angles
    //pl_pos.rotation.x += ROT_SPEED/2.0f * r.x * deltaT; // pitch
    pl_pos.rotation.y += constant::ROT_SPEED * r.y * deltaT; // yaw
    if(pl_pos.rotation.z + constant::ROT_SPEED/2.0f * -r.y * deltaT <= M_PI/4 && pl_pos.rotation.z + constant::ROT_SPEED/2.0f * -r.y * deltaT >= -M_PI/4)
        pl_pos.rotation.z += constant::ROT_SPEED/2.0f * -r.y * deltaT; // roll

    pl_pos.rotation.z = glm::clamp<float>(pl_pos.rotation.z,-M_PI/4,M_PI/4);

    if(r.y == 0)
    {
        if(pl_pos.rotation.z < 0)
        {
            pl_pos.rotation.z += constant::ROT_SPEED/2.0f * deltaT;
        }
        else if(pl_pos.rotation.z > 0)
        {
            pl_pos.rotation.z -= constant::ROT_SPEED/2.0f * deltaT;
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
    pl_pos.origin += forwardDirection * constant::MOVE_SPEED * 2.0f * deltaT; // Forward/Backward
    //pl_pos.origin += upDirection * MOVE_SPEED * m.y * deltaT;      // Up/Down
    //pl_pos.origin += rightDirection * MOVE_SPEED * m.x * deltaT;   // Left/Right
    return pl_pos;
}


glm::mat4 updatePlaneMatrix(const Position3D& pl_pos, bool cam)
{
    glm::mat4 WorldMatrixPlane = glm::mat4(1.0f);
    WorldMatrixPlane = glm::translate(WorldMatrixPlane, glm::vec3(pl_pos.origin));

    if(!cam)
        WorldMatrixPlane = glm::scale(WorldMatrixPlane, glm::vec3(40.0f));

    WorldMatrixPlane = glm::rotate(WorldMatrixPlane, pl_pos.rotation.y, glm::vec3(0, 1, 0)); // pitch
    WorldMatrixPlane = glm::rotate(WorldMatrixPlane, pl_pos.rotation.x, glm::vec3(1, 0, 0)); // yaw
    if(!cam)
        WorldMatrixPlane = glm::rotate(WorldMatrixPlane, pl_pos.rotation.z, glm::vec3(0, 0, 1)); // roll

    return WorldMatrixPlane;
}


/** Creates a view projection matrix, with near plane at 0.1, and far plane at 150.0, and
 aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
 vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
 defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
 elevation (Pitch), and <Rho> the roll.
 Return the Proj-View matrix and and the View matrix*/
std::tuple<glm::mat4,glm::mat4> updateCam(Position3D pl_pos,glm::mat4 playerUbo,bool isFirstPerson){

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
        glm::mat4 View = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,1.0f,3.6f)) *
                         glm::rotate<float>(glm::mat4(1.0f),M_PI,glm::vec3(0.0f,1.0f,0.0f)) *
                         glm::rotate(glm::mat4(1.0f),-pl_pos.rotation.z,glm::vec3(0.0f,0.0f,1.0f)) *
                         glm::rotate(glm::mat4(1.0f),-pl_pos.rotation.x,glm::vec3(1.0f,0.0f,0.0f)) *
                         glm::rotate(glm::mat4(1.0f),-pl_pos.rotation.y,glm::vec3(0.0f,1.0f,0.0f)) *
                         glm::translate(glm::mat4(1.0f),-pl_pos.origin);

        return {constant::Proj*View,View};
    }

    // Third Person
    glm::vec3 target = pl_pos.origin;
    glm::vec3 cameraPosition = playerUbo * glm::vec4(0.0f,constant::camHeight,constant::camDistZ,1.0f);
    glm::vec3 up = glm::normalize(playerUbo * glm::vec4(0.0f,1.0f,0.0f,0.0f));
    glm::mat4 View = glm::lookAt(cameraPosition,target,up);
    return {constant::Proj*View,View};
}
/*
void updateCamEND()
{
    getSixAxis(deltaT, time, m, r, shoot,isFirstPerson);

    std::array<glm::vec4,6> frustumPlanes;

    //update Starship world matrix
    glm::mat4 WorldMatrixPlane = updatePlaneMatrix(partita->player->getPosition(), true);

    //Camera Update(View-Proj)
    auto [S,proj,view] = updateCam(Ar, partita->player->getPosition(),WorldMatrixPlane,isFirstPerson);

    //View - Proj for bullets
    bullets->buboBullet.proj = proj;
    bullets->buboBullet.view = view;

    //View - Proj for the map
    tiles->tuboFloor.proj = proj;
    tiles->tuboFloor.view = view;

    tiles->tuboHouse.proj = proj;
    tiles->tuboHouse.view = view;

    tiles->tuboSkyscraper.proj = proj;
    tiles->tuboSkyscraper.view = view;

    //View - Proj for planes
    for(auto info : planes->enemyInfo) {
        info->ubo.proj = proj;
        info->ubo.view = view;
    }

    planes->playerInfo->ubo.proj = proj;
    planes->playerInfo->ubo.view = view;

    if(partita->bossSpawned) {
        planes->bossInfo->ubo.proj = proj;
        planes->bossInfo->ubo.view = view;
    }

    //for FrustumCulling
    extractFrustumPlanes(frustumPlanes, S);

    updateLights();

    updateEnemyLights();

    updateMapUniform(S, currentImage);

    updatePlayerUniform(S, currentImage);

    updateEnemyUniform(S, currentImage);

    updateBulletsUniform(S, currentImage);

    updateParticlesUniforms(S, currentImage);

    updatePlaneLightsUniform(currentImage);
}
 */

bool sphereInFrustum(std::array<glm::vec4,6>& m_planes, const glm::vec3& point, float radius)
{
    for (uint32_t i = 0; i < 6; i++) {
        if (m_planes[i][0] * point.x + m_planes[i][1] * point.y + m_planes[i][2] * point.z + m_planes[i][3] <= -radius) {
            return false;
        }
    }
    return true;
}

void extractFrustumPlanes(std::array<glm::vec4,6>& m_planes, glm::mat4 m)
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