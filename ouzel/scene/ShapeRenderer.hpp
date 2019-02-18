// Copyright 2015-2019 Elviss Strazdins. All rights reserved.

#ifndef OUZEL_SCENE_SHAPERENDERER_HPP
#define OUZEL_SCENE_SHAPERENDERER_HPP

#include <vector>
#include "scene/Component.hpp"
#include "graphics/Renderer.hpp"
#include "math/Color.hpp"
#include "graphics/BlendState.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Shader.hpp"

namespace ouzel
{
    namespace scene
    {
        class ShapeRenderer: public Component
        {
        public:
            static constexpr uint32_t CLASS = Component::SHAPE_RENDERER;

            ShapeRenderer();

            void draw(const Matrix4F& transformMatrix,
                      float opacity,
                      const Matrix4F& renderViewProjection,
                      bool wireframe) override;

            void clear();

            void line(const Vector2F& start,
                      const Vector2F& finish,
                      Color color,
                      float thickness = 0.0F);

            void circle(const Vector2F& position,
                        float radius,
                        Color color,
                        bool fill = false,
                        uint32_t segments = 16,
                        float thickness = 0.0F);

            void rectangle(const RectF& rectangle,
                           Color color,
                           bool fill = false,
                           float thickness = 0.0F);

            void polygon(const std::vector<Vector2F>& edges,
                         Color color,
                         bool fill = false,
                         float thickness = 0.0F);

            void curve(const std::vector<Vector2F>& controlPoints,
                       Color color,
                       uint32_t segments = 16,
                       float thickness = 0.0F);

            inline graphics::Shader* getShader() const { return shader; }
            inline void setShader(graphics::Shader* newShader) { shader = newShader; }

            inline graphics::BlendState* getBlendState() const { return blendState; }
            inline void setBlendState(graphics::BlendState* newBlendState) { blendState = newBlendState; }

        private:
            struct DrawCommand final
            {
                graphics::DrawMode mode;
                uint32_t indexCount;
                uint32_t startIndex;
            };

            graphics::Shader* shader = nullptr;
            graphics::BlendState* blendState = nullptr;
            std::unique_ptr<graphics::Buffer> indexBuffer;
            std::unique_ptr<graphics::Buffer> vertexBuffer;

            std::vector<DrawCommand> drawCommands;

            std::vector<uint16_t> indices;
            std::vector<graphics::Vertex> vertices;
            bool dirty = false;
        };
    } // namespace scene
} // namespace ouzel

#endif // OUZEL_SCENE_SHAPERENDERER_HPP
