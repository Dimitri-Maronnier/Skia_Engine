#include "camerafirst.h"

CameraFirst::CameraFirst()
{
    pitch = 0;
    yaw = 0;
    m_axeVertical[0] = 0;
    m_axeVertical[1] = 1;
    m_axeVertical[2] = 0;
    distance = 10;
}

CameraFirst::CameraFirst(float distanceFromPivot){
    pitch = 0;
    yaw = 0;
    m_axeVertical[0] = 0;
    m_axeVertical[1] = 1;
    m_axeVertical[2] = 0;
    distance = distanceFromPivot;
}

void CameraFirst::move(float dx, float dy , int dw, int z, int s, int q, int d){
    pitch += -dy * 0.3f;
    yaw += -dx * 0.3f;


    if(pitch > 89.0)
         pitch = 89.0;
    else if(pitch < -89.0)
        pitch = -89.0;
    float phiRadian = Utils::toRadians(pitch);
    float thetaRadian = Utils::toRadians(yaw);

    // Si l'axe vertical est l'axe X


  if(m_axeVertical[1] == 1.0)

    {

        // Calcul des coordonnées sphériques


        m_orientation[0] = distance * cos(phiRadian) * -sin(thetaRadian);

        m_orientation[1] = distance * -sin(phiRadian);

        m_orientation[2] = distance * cos(phiRadian) * cos(thetaRadian);

        lookAt.x =  cos(phiRadian) * -sin(thetaRadian);

        lookAt.y = -sin(phiRadian);

        lookAt.z = cos(phiRadian) * cos(thetaRadian);


    }



    // Sinon c'est l'axe Z




    m_deplacementLateral = glm::cross(m_axeVertical, m_orientation);
    m_deplacementLateral = glm::normalize(m_deplacementLateral);

    m_pointCible = m_position + m_orientation;

}

glm::mat4 CameraFirst::getViewMatrix(){
    viewMatrix = glm::lookAt(glm::vec3(10,10,10),glm::vec3(m_orientation[0],m_orientation[1],m_orientation[2]),glm::vec3(0,1,0));

    return viewMatrix;
}
