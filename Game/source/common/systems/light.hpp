#pragma once

#include "../ecs/world.hpp"
#include "../components/light.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>


namespace our
{
    class LightSystem {
    public:
        void update(World* world) {
            world->light_count = 0;
            for(auto entity : world->getEntities()){
				LightComponent* light = entity->getComponent<LightComponent>();
				if(light){
                    world->lights[world->light_count].kind = light->kind;
                    world->lights[world->light_count].position = glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(entity->localTransform.position, 1));
                    world->lights[world->light_count].direction = glm::vec3(glm::transpose(glm::inverse(entity->getLocalToWorldMatrix())) * glm::vec4(entity->localTransform.rotation,0));
                    world->lights[world->light_count].diffuse = light->diffuse;
                    world->lights[world->light_count].specular = light->specular;
                    world->lights[world->light_count].attenuation = light->attenuation;
                    world->lights[world->light_count].cone_angles = light->cone_angles;
					world->light_count++;
				}
            }
        }
    };

}
