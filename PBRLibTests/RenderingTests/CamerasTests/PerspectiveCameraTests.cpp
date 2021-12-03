//
//  PerspectiveCameraTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../../utils.hpp"

#include "../../../src/Rendering/Cameras/PerspectiveCamera.hpp"

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

    Viewport viewport = { };
    viewport.x          = width / 2.0f;
    viewport.y          = height / 2.0f;
    viewport.width      = width;
    viewport.height     = height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    Matrix4x4<float> look_at = Transform::lookAt(pos, eye, up).getMatrix();
    Matrix4x4<float> perspective = Transform::perspective(fovy, width / height, near, far).getMatrix();

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

    pbrlib::testing::utils::equality(width / height, camera.getAspect());
    pbrlib::testing::utils::equality(near, camera.getNearClipp());
    pbrlib::testing::utils::equality(far, camera.getFarClipp());
    pbrlib::testing::utils::equality(fovy, camera.getFovy());

    pbrlib::testing::utils::equality(eye, camera.getDirection());
    pbrlib::testing::utils::equality(pos, camera.getPosition());
    
    pbrlib::testing::utils::equality(look_at, camera.getView().getMatrix());
    pbrlib::testing::utils::equality(perspective, camera.getProjection().getMatrix());
    pbrlib::testing::utils::equality(perspective * look_at, camera.getViewProjection().getMatrix());

    pbrlib::testing::utils::equality(viewport.x, camera.getViewport().x);
    pbrlib::testing::utils::equality(viewport.y, camera.getViewport().y);
    pbrlib::testing::utils::equality(viewport.width, camera.getViewport().width);
    pbrlib::testing::utils::equality(viewport.height, camera.getViewport().height);
    pbrlib::testing::utils::equality(viewport.minDepth, camera.getViewport().minDepth);
    pbrlib::testing::utils::equality(viewport.maxDepth, camera.getViewport().maxDepth);
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

    Viewport viewport = { };
    viewport.x          = width / 2.0f;
    viewport.y          = height / 2.0f;
    viewport.width      = width;
    viewport.height     = height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

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

    pbrlib::testing::utils::equality(viewport.x, camera.getViewport().x);
    pbrlib::testing::utils::equality(viewport.y, camera.getViewport().y);
    pbrlib::testing::utils::equality(viewport.width, camera.getViewport().width);
    pbrlib::testing::utils::equality(viewport.height, camera.getViewport().height);
    pbrlib::testing::utils::equality(viewport.minDepth, camera.getViewport().minDepth);
    pbrlib::testing::utils::equality(viewport.maxDepth, camera.getViewport().maxDepth);
    pbrlib::testing::utils::equality(width / height, camera.getAspect());
    
    Transform look_at       = Transform::lookAt(pos, eye, up).getMatrix();
    Transform perspective   = Transform::perspective(fovy, width / height, near, far).getMatrix();
    
    pbrlib::testing::utils::equality(look_at.getMatrix(), camera.getView().getMatrix());
    pbrlib::testing::utils::equality(perspective.getMatrix(), camera.getProjection().getMatrix());

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

    pbrlib::testing::utils::equality(viewport.x, camera.getViewport().x);
    pbrlib::testing::utils::equality(viewport.y, camera.getViewport().y);
    pbrlib::testing::utils::equality(viewport.width, camera.getViewport().width);
    pbrlib::testing::utils::equality(viewport.height, camera.getViewport().height);
    pbrlib::testing::utils::equality(viewport.minDepth, camera.getViewport().minDepth);
    pbrlib::testing::utils::equality(viewport.maxDepth, camera.getViewport().maxDepth);
    pbrlib::testing::utils::equality(width / height, camera.getAspect());
    
    look_at     = Transform::lookAt(pos, eye, up);
    perspective = Transform::perspective(fovy, width / height, near, far).getMatrix();

    pbrlib::testing::utils::equality(look_at.getMatrix(), camera.getView().getMatrix());
    pbrlib::testing::utils::equality(perspective.getMatrix(), camera.getProjection().getMatrix());
    
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

    pbrlib::testing::utils::equality(viewport.x, camera.getViewport().x);
    pbrlib::testing::utils::equality(viewport.y, camera.getViewport().y);
    pbrlib::testing::utils::equality(viewport.width, camera.getViewport().width);
    pbrlib::testing::utils::equality(viewport.height, camera.getViewport().height);
    pbrlib::testing::utils::equality(viewport.minDepth, camera.getViewport().minDepth);
    pbrlib::testing::utils::equality(viewport.maxDepth, camera.getViewport().maxDepth);
    pbrlib::testing::utils::equality(width / height, camera.getAspect());

    perspective = Transform::perspective(fovy, width / height, near, far);

    pbrlib::testing::utils::equality(perspective.getMatrix(), camera.getProjection().getMatrix());
}
