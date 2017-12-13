#ifndef SMATH_H
#define SMATH_H
#include <math.h>
#include <glm/vec3.hpp>

namespace SMath {


    template<typename  T>
    /**
     * @brief toRadians
     * @param angle
     * @return
     */
    inline T toRadians(const T angle){
        return angle * M_PI / 180.0 ;
    }

    template<typename  T>
    /**
     * @brief rotateAround
     * @param pitch
     * @param yaw
     * @param distance
     * @param point
     * @return
     */
    inline glm::vec3 rotateAround(const T pitch,const T yaw,const T distance,const glm::vec3 origin){
        glm::vec3 position;
        T hD =  (T) (distance * cos(toRadians(pitch)));
        T vD = (T) (distance * sin(toRadians(pitch)));
        T offsetX = (T) (hD * sin(toRadians(yaw)));
        T offsetZ = (T) (hD * cos(toRadians(yaw)));
        position.x = origin.x - offsetX;
        position.y = origin.y + vD;
        position.z = origin.z - offsetZ;
        return position;
    }

}

#endif // SMATH_H
