// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"

namespace ouzel
{
    namespace video
    {
        enum VertexAttributes
        {
            VERTEX_POSITION = 0x01,
            VERTEX_COLOR = 0x02,
            VERTEX_NORMAL = 0x04,
            VERTEX_TEXCOORD0 = 0x08,
            VERTEX_TEXCOORD1 = 0x10
        };

        class VertexPC
        {
        public:
            static const uint32_t ATTRIBUTES = VERTEX_POSITION | VERTEX_COLOR;

            Vector3 position;
            Color color;

            VertexPC();
            VertexPC(Vector3 position, Color color);
        };

        class VertexPCT
        {
        public:
            static const uint32_t ATTRIBUTES = VERTEX_POSITION | VERTEX_COLOR | VERTEX_TEXCOORD0;

            Vector3 position;
            Color color;
            Vector2 texCoord;

            VertexPCT();
            VertexPCT(Vector3 position, Color color, Vector2 texCoord);
        };
    } // namespace video
} // namespace ouzel
