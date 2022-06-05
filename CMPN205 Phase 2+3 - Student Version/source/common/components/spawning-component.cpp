#include "spawning-component.hpp"
namespace our
{

    glm::vec3 SpawningController::getMinLimit()
    {
        return minLimit;
    }
    glm::vec3 SpawningController::getMaxLimit()
    {
        return maxLimit;
    }
    glm::vec3 SpawningController::getRangeValues()
    {
        return rangeValues;
    }

    void SpawningController::deserialize(const nlohmann::json &data)
    {
    }
}