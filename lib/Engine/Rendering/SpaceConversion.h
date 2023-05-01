#ifndef PIPELINE_H
#define PIPELINE_H

#include "../Engine.h"

// Aliases for Vector3 to make it clear which space a coordinate is in

// Eye space is a coordinate space in which the X, Y, and Z
// line up with the camera, and the origin is the camera's position
typedef Vector3 EyeSpace;

// Homogeneous Clip Space is the space immediately after applying
// the projection matrix, but BEFORE performing perspective division.
typedef Vector4 HomoClipSpace;

// Normalized Device Space is the space after applying perspective division
// to the Homogeneous Clip Space coordinate. In this space, the visible
// X, Y, and Z are between -1 and 1.
typedef Vector3 DeviceSpace;

// Viewport Space is the (mostly) 2D space in which the coordinates line up
// with pixels on the screen. The Z coordinate is included in this space for
// sorting.
typedef ScreenPoint ViewportSpace;

EyeSpace ToEyeSpace(Vector3 worldSpace, Transform cameraTransform);
HomoClipSpace ToClipSpace(EyeSpace eyeSpace, FrameConstants constants);
DeviceSpace ToNormalDeviceSpace(HomoClipSpace clipSpace);
ViewportSpace ToViewportSpace(DeviceSpace deviceSpace, FrameConstants constants);

#endif
