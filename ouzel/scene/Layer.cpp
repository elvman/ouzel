// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "Layer.h"
#include "core/Engine.h"
#include "Node.h"
#include "Camera.h"
#include "graphics/Renderer.h"
#include "Scene.h"
#include "math/Matrix4.h"
#include "Component.h"

namespace ouzel
{
    namespace scene
    {
        Layer::Layer()
        {
        }

        Layer::~Layer()
        {
            for (const CameraPtr& camera : cameras)
            {
                camera->removeFromLayer();
            }
        }

        void Layer::draw()
        {
            for (const CameraPtr& camera : cameras)
            {
                camera->clearDrawQueue();
                
                for (const NodePtr& child : children)
                {
                    if (!child->isHidden())
                    {
                        child->visit(Matrix4::IDENTITY, false, camera, 0.0f);
                    }
                }

                const auto& drawQueue = camera->getDrawQueue();

                for (const auto& node : drawQueue)
                {
                    node->draw(camera);
                }

                if (wireframe)
                {
                    for (const auto& node : drawQueue)
                    {
                        node->drawWireframe(camera);
                    }
                }
            }
        }

        bool Layer::addChild(const NodePtr& node)
        {
            if (NodeContainer::addChild(node))
            {
                node->updateTransform(Matrix4::IDENTITY);

                return true;
            }
            else
            {
                return false;
            }
        }

        void Layer::addCamera(const CameraPtr& camera)
        {
            auto i = cameras.insert(camera);

            if (i.second)
            {
                camera->addToLayer(this);
                camera->recalculateProjection();
            }
        }

        void Layer::removeCamera(const CameraPtr& camera)
        {
            auto i = cameras.find(camera);

            if (i != cameras.end())
            {
                camera->removeFromLayer();
                cameras.erase(i);
            }
        }

        NodePtr Layer::pickNode(const Vector2& position) const
        {
            for (auto i = cameras.rbegin(); i != cameras.rend(); ++i)
            {
                const CameraPtr& camera = *i;

                std::vector<NodePtr> nodes;

                Vector2 worldPosition = camera->convertNormalizedToWorld(position);

                NodeContainer::pickNodes(worldPosition, nodes);

                if (!nodes.empty()) return nodes.front();
            }

            return nullptr;
        }

        std::vector<NodePtr> Layer::pickNodes(const Vector2& position) const
        {
            std::vector<NodePtr> result;

            for (auto i = cameras.rbegin(); i != cameras.rend(); ++i)
            {
                const CameraPtr& camera = *i;

                Vector2 worldPosition = camera->convertNormalizedToWorld(position);

                std::vector<NodePtr> nodes;
                NodeContainer::pickNodes(worldPosition, nodes);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }

            return result;
        }

        std::vector<NodePtr> Layer::pickNodes(const std::vector<Vector2>& edges) const
        {
            std::vector<NodePtr> result;

            for (auto i = cameras.rbegin(); i != cameras.rend(); ++i)
            {
                const CameraPtr& camera = *i;
                
                std::vector<Vector2> worldEdges;
                worldEdges.reserve(edges.size());

                for (const Vector2& edge : edges)
                {
                    worldEdges.push_back(camera->convertNormalizedToWorld(edge));
                }

                std::vector<NodePtr> nodes;
                NodeContainer::pickNodes(worldEdges, nodes);

                result.insert(result.end(), nodes.begin(), nodes.end());
            }

            return result;
        }

        void Layer::setOrder(int32_t newOrder)
        {
            order = newOrder;
        }

        void Layer::recalculateProjection()
        {
            for (const CameraPtr& camera : cameras)
            {
                camera->recalculateProjection();
            }
        }

        void Layer::enter()
        {
            NodeContainer::enter();

            recalculateProjection();
        }
    } // namespace scene
} // namespace ouzel
