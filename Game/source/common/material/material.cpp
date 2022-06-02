#include "material.hpp"
#include <iostream>
#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //DONE: (Req 6) Write this function
        //*Comment: Setup the pipeline state
        pipelineState.setup();
        //*Comment: Set the shader to be used
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
        std::string type =data["type"].get<std::string>();
        if (type == "LightMaterial"){
            affectedByLight = true;
        }else{
            affectedByLight = false;
        }
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //DONE: (Req 6) Write this function
        //* Comment: Calling the setup of the parent class
        Material::setup();
        //* Comment: Setting the uniform tint
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //DONE: (Req 6) Write this function
        //* Comment: Calling the setup of the parent class
        TintedMaterial::setup();
        //* Comment: Setting the alpha threshold uniform
        shader->set("alphaThreshold", alphaThreshold);
        //* Comment: Binding the texture and sampler to the active texture unit (if nothing is active, GL_TEXTURE0 is used)
        int texUnit = GL_TEXTURE0;
        //!What texture unit should we use?
        //?I assumed that we should use the active texture unit.
        glGetIntegerv(GL_ACTIVE_TEXTURE, &texUnit);
        texUnit = texUnit - GL_TEXTURE0;
        texture->bind();
        sampler->bind(texUnit);
        shader->set("tex", texUnit);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
        
    }

    //!ADDED FOR LIGHT
    //----------------------
    void LightMaterial::setup() const {
        Material::setup();
        glActiveTexture(GL_TEXTURE0);
        albedo->bind();
        sampler->bind(0);
        shader->set("material.albedo", 0);
        
        glActiveTexture(GL_TEXTURE1);
        ambient_occlusion->bind();
        sampler->bind(1);
        shader->set("material.ambient_occlusion", 1);
        
        glActiveTexture(GL_TEXTURE2);
        roughness->bind();
        sampler->bind(2);
        shader->set("material.roughness", 2);
        
        glActiveTexture(GL_TEXTURE3);
        emissive->bind();
        sampler->bind(3);
        shader->set("material.emissive", 3);
        
        glActiveTexture(GL_TEXTURE4);
        specular->bind();
        sampler->bind(4);
        shader->set("material.specular", 4);
    }

    void LightMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", "black"));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", "black"));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", "black"));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", "black"));
        specular = AssetLoader<Texture2D>::get(data.value("specular", "black"));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", "default"));
        
        std::cout <<  std::endl << albedo << std::endl;
        std::cout <<  std::endl << ambient_occlusion << std::endl;
        std::cout <<  std::endl << roughness << std::endl;
        std::cout <<  std::endl << emissive << std::endl;
        std::cout <<  std::endl << specular << std::endl;
        std::cout <<  std::endl << sampler << std::endl;
    }
    //----------------------
}