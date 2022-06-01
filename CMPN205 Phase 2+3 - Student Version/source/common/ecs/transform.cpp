#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // DONE: todo: (Req 2) Write this function
        // build 4x4 translation matrix from the position vector where the translation amount
        // is determined by the position vector components value
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position);
        // build 4x4 scaling matrix where the scaling in each axis is determined
        // by the scale vector components value
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), scale);
        // build 4x4 homogeneous rotation matrix where the rotation angles are determined
        // by the rotation vector of euler angles
        glm::mat4 rotation_matrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

        // final transform = multiplication of all three matrices in the reversed order
        // where the order of transformations is: Scaling, Rotation then Translation
        return translation_matrix * rotation_matrix * scaling_matrix;
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);

        // position, rotation and scale with respect to parent entity.
    }

}