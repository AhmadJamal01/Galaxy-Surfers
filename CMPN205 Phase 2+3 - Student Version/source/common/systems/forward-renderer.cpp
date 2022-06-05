#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
#include <iostream>
namespace our
{

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
    {
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if (config.contains("sky"))
        {
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram *skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();

            // TODO: (Req 9) Pick the correct pipeline state to draw the sky
            //  Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            //  We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};

            skyPipelineState.depthTesting.enabled = true;       // set depthTesting Enabled as sky would be drawn after opaque
            skyPipelineState.depthTesting.function = GL_LEQUAL; // LEQUAL because object would be drawn from near to far

            skyPipelineState.faceCulling.enabled = true;        // set faceCulling Enabled so we can draw the sphere from inside
            skyPipelineState.faceCulling.culledFace = GL_FRONT; // culling the Front of sphere

            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky
            Sampler *skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if (config.contains("postprocess"))
        {
            // done TODO: (Req 10) Create a framebuffer
            glGenFramebuffers(1, &postprocessFrameBuffer);
            // bind the frame buffer so it becomes the target buffer for rendering operations
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);

            // done TODO: (Req 10) Create a color and a depth texture and attach them to the framebuffer
            //  Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            //  The depth format can be (Depth component with 24 bits).
            // color texture
            colorTarget = texture_utils::empty(GL_RGBA8, windowSize);
            // depth texture
            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24, windowSize);

            // attach color texture as a color_attachment as a logical buffer of the framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
            // attach color texture as a depth_attachment as a logical buffer of the framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);

            // done TODO: (Req 10) Unbind the framebuffer just to be safe
            // unbinding is done by calling glBindFramebuffer with 0 as the target buffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler *postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            //= Create the post processing shaders for each effect in postprocess
            for (const auto &shader : config["postprocess"]) // Assume its an array of postprocessing shaders
            {
                ShaderProgram *postprocessShader = new ShaderProgram();
                postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
                postprocessShader->attach(shader.get<std::string>(), GL_FRAGMENT_SHADER);
                postprocessShader->link();
                postprocessShaders.push_back(postprocessShader); //= push into vector of postprocessing shaders
            }

            //= Initially postProcessMaterial will be null and we use the normal shader.
            //= Whenever we need, we can switch between postProcessMaterial from null to this value to see the postprocessing effect.
            if (postprocessShaders.size() > 0)
            {
                postprocessMaterial = new TexturedMaterial();
                postprocessMaterial->shader = postprocessShaders[0]; //= the default postprocessing effect does nothing.
                postprocessMaterial->texture = colorTarget;
                postprocessMaterial->sampler = postprocessSampler;
                // The default options are fine but we don't need to interact with the depth buffer
                // so it is more performant to disable the depth mask
                postprocessMaterial->pipelineState.depthMask = false;
            }
        }
    }

    void ForwardRenderer::choosePostProcessing(int index) //= To choose the preprocessing effect (also used in ninja system upon key press)
    {
        if (index < postprocessShaders.size())
            postprocessMaterial->shader = postprocessShaders[index];
    }

    void ForwardRenderer::destroy()
    {
        // Delete all objects related to the sky
        if (skyMaterial)
        {
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if (postprocessMaterial)
        {
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World *world)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent *camera = nullptr;
        opaqueCommands.clear();
        lightSupportCommands.clear();
        transparentCommands.clear();
        int entityIdx = 0;
        for (auto entity : world->getEntities())
        {
            // If we hadn't found a camera yet, we look for a camera in this entity
            if (!camera)
                camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
            {
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if (command.material->transparent)
                {
                    transparentCommands.push_back(command);
                }
                else if (command.material->affectedByLight)
                {
                    lightSupportCommands.push_back(command);
                }
                else
                {
                    // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if (camera == nullptr)
            return;

        // TODO: (Req 8) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        // cameraForward is a vector looking ahead in front of the camera
        // by convention (in OpenGL) the camera points towards the negative z-axis
        // so we want negate the direction vector since we want the coordinate system z-axis to be positive
        auto owner = camera->getOwner();
        auto M = owner->getLocalToWorldMatrix();
        glm::vec3 cameraForward = M * glm::vec4(0.0, 0.0, -1.0, 0.0);

        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand &first, const RenderCommand &second)
                  {
            //TODO: (Req 8) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second". 
            //one way to determine which object is further from the camera is to find its projection
            //on the direction of cameraForward, this can be easily noticed when the problem is sketched
            //the object with the higher value of projection is the one farthest and thus should be drawn first
            return (glm::dot(cameraForward,first.center) > glm::dot(cameraForward , second.center)); });

        // TODO: (Req 8) Get the camera ViewProjection matrix and store it in VP
        // in order to get the VP matrix, we multiply the projection and view matrices in order
        glm::mat4 VP = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();
        // glm::mat4 VP = glm::mat4(1.0f);

        // TODO: (Req 8) Set the OpenGL viewport using windowSize
        // configures the size of the rendering window
        // this also decides which part of the screen to draw in
        // The first two parameters set the location of the lower left corner of the window.
        // The third and fourth parameters set the width and height of the rendering window in pixels
        glViewport(0, 0, windowSize.x, windowSize.y);
        // TODO: (Req 8) Set the clear color to black and the clear depth to 1
        // https://www.dei.isep.ipp.pt/~psousa/aulas/OpenGL/course/course-opengl-2.html
        // the values used to clear the buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);

        // TODO: (Req 8) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        // make sure new values for all color channels are written to the color buffer
        glColorMask(true, true, true, true);
        // make sure that new values are written to the depth buffer after the depth function
        // if set to false, no new values will be written to the depth/color buffer
        glDepthMask(true);

        // If there is a postprocess material, bind the framebuffer
        if (postprocessMaterial)
        {
            // done TODO: (Req 10) bind the framebuffer
            // bind the framebuffer to be the target buffer for rendering operations
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
        }

        // TODO: (Req 8) Clear the color and depth buffers
        // configures openGl to clear the color buffer and depth buffer each frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: (Req 8) Draw all the opaque commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (auto &command : opaqueCommands)
        {
            // model matrix is the transformation matrix from local space to world space
            // to obtain MVP we need to multiply the model matrix from the left which is equivalent to
            // multiplying from the right in code
            glm::mat4 transform = VP * command.localToWorld;

            command.material->setup();
            command.material->shader->set("transform", transform);
            command.mesh->draw();
        }

        //! ADDED FOR LIGHT
        //--------------------
        int light_count = world->light_count;
        Light *lights = world->lights;
        for (auto &command : lightSupportCommands)
        {
            command.material->setup();

            glm::mat4 M = command.localToWorld;
            glm::mat4 M_IT = glm::transpose(glm::inverse(M));
            glm::vec3 eye = camera->getOwner()->localTransform.position;
            glm::vec3 sky_top = glm::vec3(0.3f, 0.6f, 1.0f);
            glm::vec3 sky_middle = glm::vec3(0.3f, 0.3f, 0.3f);
            glm::vec3 sky_bottom = glm::vec3(0.1f, 0.1f, 0.0f);
            command.material->shader->set("M", M);
            command.material->shader->set("VP", VP);
            command.material->shader->set("M_IT", M_IT);
            command.material->shader->set("eye", eye);
            command.material->shader->set("sky.top", sky_top);
            command.material->shader->set("sky.middle", sky_middle);
            command.material->shader->set("sky.bottom", sky_bottom);

            // light
            for (int i = 0; i < light_count; i++)
            {
                command.material->shader->set("lights[" + std::to_string(i) + "].type", lights[i].kind);
                command.material->shader->set("lights[" + std::to_string(i) + "].position", lights[i].position);
                command.material->shader->set("lights[" + std::to_string(i) + "].diffuse", lights[i].diffuse);
                command.material->shader->set("lights[" + std::to_string(i) + "].specular", lights[i].specular);
                command.material->shader->set("lights[" + std::to_string(i) + "].attenuation", lights[i].attenuation);
                command.material->shader->set("lights[" + std::to_string(i) + "].direction", lights[i].direction);
                command.material->shader->set("lights[" + std::to_string(i) + "].cone_angles", lights[i].cone_angles);
            }
            command.material->shader->set("light_count", light_count);
            command.mesh->draw();
        }
        //--------------------
        // If there is a sky material, draw the sky
        if (this->skyMaterial)
        {
            // TODO: (Req 9) setup the sky material
            this->skyMaterial->setup();
            // TODO: (Req 9) Get the camera position
            glm::vec3 cameraPosition = owner->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1.0);

            // TODO: (Req 9) Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)
            glm::mat4 identity(1.0f);
            glm::mat4 M = glm::translate(identity, cameraPosition); // translating shpere position to camera position

            // TODO: (Req 9) We want the sky to be drawn behind everything (in NDC space, z=1)
            //  We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
            glm::mat4 alwaysBehindTransform = glm::mat4(
                //  Row1, Row2, Row3, Row4
                1.0f, 0.0f, 0.0f, 0.0f, // Column1
                0.0f, 1.0f, 0.0f, 0.0f, // Column2
                0.0f, 0.0f, 0.0f, 0.0f, // Column3       // set z=0
                0.0f, 0.0f, 1.0f, 1.0f  // Column4      // set z=1
            );
            // TODO: (Req 9) set the "transform" uniform
            glm::mat4 skyTransform = alwaysBehindTransform * VP * M; // trasforming sky to depth = 1
            skyMaterial->shader->set("transform", skyTransform);
            // TODO: (Req 9) draw the sky sphere
            skySphere->draw();
        }
        // TODO: (Req 8) Draw all the transparent commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (auto &command : transparentCommands)
        {
            glm::mat4 transform = VP * command.localToWorld;
            command.material->setup();
            command.material->shader->set("transform", transform);
            command.mesh->draw();
        }
        // If there is a postprocess material, apply postprocessing
        if (postprocessMaterial)
        {
            // DONE: (Req 10) Return to the default framebuffer
            // by calling glBindFramebuffer with 0 as the target buffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            // done TODO: (Req 10) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }
}