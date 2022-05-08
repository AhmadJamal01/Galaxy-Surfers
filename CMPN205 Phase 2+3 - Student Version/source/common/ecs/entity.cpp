#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    //* This function returns the transformation matrix from the entity's local space to the world space
    //* Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    //! Each entity is associated with a local transform indicating the transformation to go from its local space to the parent's local space.
    //* To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    //* its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const {
        using namespace glm;
        //DONE: (Req 7) Write this function
        const Entity* currParent = this->parent;                                // The paren of the calling object.
        mat4 LTWM = this->localTransform.toMat4();                              // Local to World Matrix of the calling object.
        // localTransform stores (x, y, z) for position, rotation, scale and a way to convert those to a matrix ( toMat4() )
        
        while(currParent != nullptr){                                          // So long as there is a parent (not the root yet.)
        LTWM = (currParent)->localTransform.toMat4() * LTWM;                   // So that M2*M1*M0 (starting from child at right)
        currParent = currParent->parent;
        }
        // All parentless objects are already in the world space and hence we go the world in the end.

        return LTWM;                                                            //* Now this is the local to world matrix.
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        name = data.value("name", name);
        localTransform.deserialize(data);
        if(data.contains("components")){
            if(const auto& components = data["components"]; components.is_array()){
                for(auto& component: components){
                    deserializeComponent(component, this);
                }
            }
        }
    }

}