// Copyright (C) 2015 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Renderer.h"

namespace ouzel
{
    namespace video
    {
        class RendererMetal: public Renderer
        {
            friend Engine;
        public:
            virtual ~RendererMetal();
            
            virtual void setClearColor(Color color) override;
            
            virtual void setSize(const Size2& size) override;
            
            virtual void clear() override;
            virtual void flush() override;
            
            virtual std::shared_ptr<Texture> loadTextureFromFile(const std::string& filename, bool dynamic, bool mipmaps = true) override;
            virtual std::shared_ptr<Texture> loadTextureFromData(const void* data, const Size2& size, bool dynamic, bool mipmaps = true) override;
            
            virtual std::shared_ptr<Shader> loadShaderFromFiles(const std::string& fragmentShader, const std::string& vertexShader, uint32_t vertexAttributes) override;
            virtual std::shared_ptr<Shader> loadShaderFromBuffers(const uint8_t* fragmentShader, uint32_t fragmentShaderSize, const uint8_t* vertexShader, uint32_t vertexShaderSize, uint32_t vertexAttributes) override;
            
            virtual std::shared_ptr<MeshBuffer> createMeshBuffer(const void* indices, uint32_t indexSize, uint32_t indexCount, bool dynamicIndexBuffer, const void* vertices, uint32_t vertexSize, uint32_t vertexCount, bool dynamicVertexBuffer, uint32_t vertexAttributes) override;
            virtual bool drawMeshBuffer(const MeshBufferPtr& meshBuffer, uint32_t indexCount = 0, DrawMode drawMode = DrawMode::TRIANGLE_LIST) override;
            
        private:
            RendererMetal();
        };
    } // namespace video
} // namespace ouzel
