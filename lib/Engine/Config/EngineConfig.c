#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../Engine.h"

FrameConstants GetFrameConstants(EngineConfig config) {
    float aspect = (float)config.viewportHeight / config.viewportWidth;
    float effectiveAspect = config.effectiveHeight / config.effectiveWidth;
    float distanceScaling = 1.0 / tan(0.5 * config.fovRadians);
    float frustum = config.zFar / (config.zFar - config.zNear);

    FrameConstants constants = {
        .viewportWidth = config.viewportWidth,
        .viewportHeight = config.viewportHeight,
        .aspectRatio = aspect,
        .effectiveAspectRatio = effectiveAspect,
        .zNear = config.zNear,
        .zFar = config.zFar,
        .frustum = frustum,
        .fovRadians = config.fovRadians,
        .distanceScaling = distanceScaling,
        .backfaceCulling = config.backfaceCulling,
        .fillTriangles = config.wireframeMode,
    };

    return constants;
}
