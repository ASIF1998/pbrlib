//
//  PerspectiveCameraTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>
#include "../../../src/Rendering/Cameras/PerspectiveCamera.hpp"

using namespace testing;
using namespace pbrlib;
using namespace pbrlib::math;

TEST(RenderingCameraPerspectiveCamera, Builder)
{
    constexpr float width   = 800.0f;
    constexpr float height  = 600.0f;

    constexpr float near    = 0.010f;
    constexpr float far     = 100.0f;
    constexpr float fovy    = 45.00f;

    constexpr Vec3<float> eye   (0.0f, 0.0f, -1.0f);
    constexpr Vec3<float> pos   (0.0f, 0.0f, 0.00f);
    constexpr Vec3<float> up    (0.0f, 1.0f, 0.00f);
    
    constexpr Viewport viewport {
        .x          = width / 2.0f,
        .y          = height / 2.0f,
        .width      = width,
        .height     = height,
        .minDepth   = 0.0f,
        .maxDepth   = 1.0f
    };

    Transform r1 = Transform::lookAt(pos, eye, up);
    Transform r2 = Transform::perspective(fovy, width / height, near, far);

    PerspectiveCamera::Builder build_camera;

    build_camera.setAspect(width, height);
    build_camera.setEye(eye);
    build_camera.setNearClipp(near);
    build_camera.setFarClipp(far);
    build_camera.setPosition(pos);
    build_camera.setUp(up);
    build_camera.setFovy(fovy);
    build_camera.setViewport(viewport);
    
    PerspectiveCamera camera = build_camera.build();

    EXPECT_EQ(width / height, camera.getAspect());
    EXPECT_EQ(near, camera.getNearClipp());
    EXPECT_EQ(far, camera.getFarClipp());
    EXPECT_EQ(fovy, camera.getFovy());
    EXPECT_EQ(eye, camera.getDirection());
    EXPECT_EQ(pos, camera.getPosition());
    EXPECT_EQ(r1, camera.getView());
    EXPECT_EQ(r2, camera.getProjection());
    EXPECT_EQ(r2 * r1, camera.getViewProjection());
    
    EXPECT_EQ(viewport.x, camera.getViewport().x);
    EXPECT_EQ(viewport.y, camera.getViewport().y);
    EXPECT_EQ(viewport.width, camera.getViewport().width);
    EXPECT_EQ(viewport.height, camera.getViewport().height);
    EXPECT_EQ(viewport.minDepth, camera.getViewport().minDepth);
    EXPECT_EQ(viewport.maxDepth, camera.getViewport().maxDepth);
}

TEST(RenderingCameraPerspectiveCamera, Setters)
{
    float width   = 800.0f;
    float height  = 600.0f;

    float near    = 0.010f;
    float far     = 100.0f;
    float fovy    = 45.00f;

    Vec3<float> eye   (0.0f, 0.0f, -1.0f);
    Vec3<float> pos   (0.0f, 0.0f, 0.00f);
    Vec3<float> up    (0.0f, 1.0f, 0.00f);

    Viewport viewport {
        .x          = width / 2.0f,
        .y          = height / 2.0f,
        .width      = width,
        .height     = height,
        .minDepth   = 0.0f,
        .maxDepth   = 1.0f
    };

    PerspectiveCamera::Builder build_camera;

    build_camera.setAspect(width, height);
    build_camera.setEye(eye);
    build_camera.setNearClipp(near);
    build_camera.setFarClipp(far);
    build_camera.setPosition(pos);
    build_camera.setUp(up);
    build_camera.setFovy(fovy);
    build_camera.setViewport(viewport);

    PerspectiveCamera camera = build_camera.build();


    EXPECT_EQ(viewport.x, camera.getViewport().x);
    EXPECT_EQ(viewport.y, camera.getViewport().y);
    EXPECT_EQ(viewport.width, camera.getViewport().width);
    EXPECT_EQ(viewport.height, camera.getViewport().height);
    EXPECT_EQ(viewport.minDepth, camera.getViewport().minDepth);
    EXPECT_EQ(viewport.maxDepth, camera.getViewport().maxDepth);
    
    EXPECT_EQ(Transform::lookAt(pos, eye, up), camera.getView());
    EXPECT_EQ(Transform::perspective(fovy, width / height, near, far), camera.getProjection());
    EXPECT_EQ(width / height, camera.getAspect());

    eye[0]  =   1.0f;
    pos[2]  -=  1.0f;
    up[0]   =   -1.0f;

    near    = 50.0f;
    far     = 60.0f;
    fovy    = 60.0f;

    width   = 1000.0f;
    height  = 700.00f;

    viewport.x          = 0.0000f;
    viewport.y          = 0.0000f;
    viewport.width      = 1000.0f;
    viewport.height     = 500.00f;
    viewport.minDepth   = 0.2000f;
    viewport.maxDepth   = 0.8000f;

    camera.setLookAt(eye, pos, up);
    camera.setNearAndFarClipp(near, far);
    camera.setAspect(width / height);
    camera.setFovy(fovy);
    camera.setViewport(
        viewport.x,
        viewport.y,
        viewport.width,
        viewport.height,
        viewport.minDepth,
        viewport.maxDepth
    );
    

    EXPECT_EQ(viewport.x, camera.getViewport().x);
    EXPECT_EQ(viewport.y, camera.getViewport().y);
    EXPECT_EQ(viewport.width, camera.getViewport().width);
    EXPECT_EQ(viewport.height, camera.getViewport().height);
    EXPECT_EQ(viewport.minDepth, camera.getViewport().minDepth);
    EXPECT_EQ(viewport.maxDepth, camera.getViewport().maxDepth);
    
    EXPECT_EQ(Transform::lookAt(pos, eye, up), camera.getView());
    EXPECT_EQ(Transform::perspective(fovy, width / height, near, far), camera.getProjection());
    EXPECT_EQ(width / height, camera.getAspect());
    
    fovy    -=  35.00f;
    width   =   800.0f;
    height  =   500.0f;

    viewport.x          = 500.00f;
    viewport.y          = 500.00f;
    viewport.width      = 1500.0f;
    viewport.height     = 1500.0f;
    viewport.minDepth   = 0.3000f;
    viewport.maxDepth   = 0.7000f;
    
    camera.setAspectAndFovy(width, height, fovy);
    camera.setViewport(viewport);
    

    EXPECT_EQ(viewport.x, camera.getViewport().x);
    EXPECT_EQ(viewport.y, camera.getViewport().y);
    EXPECT_EQ(viewport.width, camera.getViewport().width);
    EXPECT_EQ(viewport.height, camera.getViewport().height);
    EXPECT_EQ(viewport.minDepth, camera.getViewport().minDepth);
    EXPECT_EQ(viewport.maxDepth, camera.getViewport().maxDepth);
    
    EXPECT_EQ(Transform::perspective(fovy, width / height, near, far), camera.getProjection());
    EXPECT_EQ(width / height, camera.getAspect());
}
