#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        //DONE: (Req 7) Complete this function
        using namespace glm;
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        //!but to use glm::lookAt, we need eye, center and up in the world space.
        // Since M (see above) transforms from the camera to the world space, you can use M to compute:
        // - the [eye position] which is the point (0,0,0) but after being transformed by M
        // - the [center position] which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        vec4 camera_eye = vec4(0, 0, 0, 1);
        vec4 camera_center = vec4 (0, 0, -1, 1);
        vec4 camera_up = vec4(0, 1, 0, 0);                                          // It's a vector.       

        // Deriving the view matrix from the camera's model matrix.
        mat4 view = lookAt(
            vec3(M * camera_eye),                                                  // camera position
            vec3(M * camera_center),                                               // position camera is looking at
            vec3(M * camera_up)                                                    // turn up vector
        );
        
        return view;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        using namespace glm;

        float asp_ratio = viewportSize[0]/(float)viewportSize[1];
        
        //DONE: (Req 7) Wrtie this function

        // Defining an Orthographic Projection
        if (this->cameraType == CameraType::ORTHOGRAPHIC){
             mat4 orthographic_projection = ortho(
            -(this->orthoHeight)/2 * asp_ratio,                         // Left and Right computed from Up and Down + Aspect Ratio                               
            // Left Plane
            (this->orthoHeight)/2 * asp_ratio,
            // Right Plane
            -(this->orthoHeight)/2,
            // Top Plane
            (this->orthoHeight)/2
            // Bottom Plane
        );
            
        return orthographic_projection;
        }
        else{
            // Defining a Perspective Projection
            mat4 perspective_projection = perspective(
            this->fovY,                                         
            // The vertical field of view angle (smaller means more zoom in)
            asp_ratio,
            //aspect ratio (width ÷ height of the window)
            this->near,
            //near plane relative to camera
            this->far
            // far plane relative to camera
        );
        return perspective_projection;
        }
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
    }
}