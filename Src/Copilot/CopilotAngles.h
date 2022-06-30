#pragma once
#include <cmath>

#include <Antilatency.Api.h>

    struct AnglesInDegrees {
        float rollDeg{};
        float pitchDeg{};
        float yawDeg{};
    };

    struct AnglesInRadians {
        float rollRad{};
        float pitchRad{};
        float yawRad{};
    };

    struct QuaternionDrone
    {
        float w, x, y, z;
    };

    /**
     * @brief Convert degree to radians [-pi : pi]
     */
    float angleInDegToRaD(float angleInDeg);

    /**
     * @brief Convert Quaternion of Alt to Angle (degrees) of Drone PX4 (don't convert another Quaternion)
     */
    AnglesInDegrees quaternionAltToAngleDronePX4(Antilatency::Math::floatQ rotation,float correctionYaw);

    /**
     * @brief Convert Degrees of YAW ROLL PITCH to Radians
     */
    AnglesInRadians anglesInDegToRad(AnglesInDegrees anglesInDegrees);


