#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our
{
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState
    {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true;                           // To know how to use it, check glDepthMask

        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const
        {
            // TODO: (Req 3) Write this function

            // checks the bool enabled defined in the struct faceCulling
            if (faceCulling.enabled)
            {
                // if faceCulling.enabled, we have to enable OpenGL's GL_CULL_FACE option
                glEnable(GL_CULL_FACE);

                // next we choose which face to render while discarding the other
                glCullFace(faceCulling.culledFace);

                // we also need to specify which ordering of vertices define the front face
                glFrontFace(faceCulling.frontFace);
            }

            if (depthTesting.enabled)
            {
                // configure openGL to use depth testing
                glEnable(GL_DEPTH_TEST);

                // configures the function used to compare and select the depth to be used
                // for example in the tutorial we would select the less value to be set in the depth buffer
                glDepthFunc(depthTesting.function);
            }

            if (blending.enabled)
            {
                glEnable(GL_BLEND);

                glBlendEquation(blending.equation);

                glBlendFunc(blending.sourceFactor, blending.destinationFactor);

                glBlendColor(blending.constantColor[0],
                             blending.constantColor[1],
                             blending.constantColor[2],
                             blending.constantColor[3]);
            }

            // make sure new values for all color channels are written to the color buffer
            glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
            // make sure that new values are written to the depth buffer after the depth function
            // if set to false, no new values will be written to the depth/color buffer
            glDepthMask(depthMask);
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json &data);
    };

}