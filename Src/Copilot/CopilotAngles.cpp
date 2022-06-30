#include "CopilotAngles.h"

float angleInDegToRaD(float inDeg){
    float angleInRad = 0;
    if(inDeg>=-180 && inDeg<=180){
        angleInRad = inDeg/57.2957801;
    } else if(inDeg>180 && inDeg<=360){
        angleInRad = (inDeg-360)/57.2957801;
    } else if(inDeg>360){
        angleInRad = angleInDegToRaD(inDeg-360);
    } else if(inDeg < -180){
        angleInRad = angleInDegToRaD(inDeg+180);
    }
    return angleInRad;
}

AnglesInDegrees quaternionAltToAngleDronePX4(Antilatency::Math::floatQ rotation,float correctionYaw){
    float yaw{};
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    float w = rotation.w;
    AnglesInDegrees angleInDegrees;
    angleInDegrees.rollDeg = atan2(2.0*(w*z+x*y),1-2.0*(z*z+x*x))*57.2957801;
    angleInDegrees.pitchDeg = asin(2.0*(x*w-y*z))*57.2957801;
    yaw = atan2(2.0*(y*w+z*x),(-1+2*(w*w+z*z)));
    if(yaw>=0){
        yaw=yaw*57.2957801;
    }
    if(yaw<0) {
        yaw=360+yaw*57.2957801;
    }
    yaw = yaw + correctionYaw;
    if(yaw>=360){
        yaw = yaw - 360;
    }
    angleInDegrees.yawDeg = yaw;
    return angleInDegrees;
}

AnglesInRadians anglesInDegToRad(AnglesInDegrees anglesInDegrees){
    AnglesInRadians anglesInRadians;
    anglesInRadians.yawRad = angleInDegToRaD(anglesInDegrees.yawDeg);
    anglesInRadians.rollRad = angleInDegToRaD(anglesInDegrees.rollDeg);
    anglesInRadians.pitchRad = angleInDegToRaD(anglesInDegrees.pitchDeg);
    return anglesInRadians;
}


