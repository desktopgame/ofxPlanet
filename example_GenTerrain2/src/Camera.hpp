#pragma once
#ifndef SHADER_CAMERA_HPP
#define SHADER_CAMERA_HPP
#include <ofEasyCam.h>
#include <ofTypes.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

/**
 * Camera �̓J�����̊�{�I�ȍs����v�Z����N���X�ł��B
 */
class Camera : public std::enable_shared_from_this<Camera> {
       public:
        explicit Camera();

        /**
         * Field Of View ��ݒ肵�܂��B
         * @param fov
         */
        void setFOV(float fov);
        /**
         * Field Of View ��Ԃ��܂��B
         * @return
         */
        float getFOV() const;

        /**
         * �j�A�v���[����ݒ肵�܂��B
         * @param nearPlane
         */
        void setNearPlane(float nearPlane);
        /**
         * �j�A�v���[����Ԃ��܂��B
         * @return
         */
        float getNearPlane() const;

        /**
         * �t�@�[�v���[����ݒ肵�܂��B
         * @param farPlane
         */
        void setFarPlane(float farPlane);
        /**
         * �t�@�[�v���[����Ԃ��܂��B
         * @return
         */
        float getFarPlane() const;

        /**
         * ��ʃT�C�Y��ݒ肵�܂��B
         * @param screenSize
         */
        void setScreenSize(const glm::vec2& screenSize);
        /**
         * ��ʃT�C�Y��Ԃ��܂��B
         * @retunr
         */
        glm::vec2 getScreenSize() const;

        /**
         * �J�����̈ʒu��ݒ肵�܂��B
         * @param position
         */
        void setPosition(const glm::vec3& position);
        /**
         * �J�����̈ʒu��Ԃ��܂��B
         * @return
         */
        glm::vec3 getPosition() const;

        /**
         * �J�����̒����_��ݒ肵�܂��B
         * @param lookAt
         */
        void setLookAt(const glm::vec3& lookAt);
        /**
         * �J�����̒����_��Ԃ��܂��B
         * @return
         */
        glm::vec3 getLookAt() const;

        /**
         * �J�����̏������ݒ肵�܂��B
         * @param up
         */
        void setUp(const glm::vec3& up);
        /**
         * �J�����̏������Ԃ��܂��B
         * @return
         */
        glm::vec3 getUp() const;

        /**
         * �s����Čv�Z���܂��B
         * @return
         */
        bool rehash();
        /**
         * �v���W�F�N�V�����s���Ԃ��܂��B
         * @return
         */
        glm::mat4 getProjectionMatrix() const;
        /**
         * �r���[�s���Ԃ��܂��B
         */
        glm::mat4 getViewMatrix() const;
        /**
         * �m�[�}���s���Ԃ��܂��B
         * @param model
         * @return
         */
        glm::mat4 computeNormalMatrix(const glm::mat4 model) const;

       private:
        bool dirty;
        float fov;
        float nearPlane;
        float farPlane;
        glm::vec2 screenSize;
        glm::vec3 position;
        glm::vec3 lookAt;
        glm::vec3 up;

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
};
#endif