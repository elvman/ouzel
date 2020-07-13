// Copyright 2015-2020 Elviss Strazdins. All rights reserved.

#ifndef OUZEL_GRAPHICS_RENDERER_LIGHT_HPP
#define OUZEL_GRAPHICS_RENDERER_LIGHT_HPP

namespace ouzel::graphics::renderer
{
    class Light final
    {
    public:
        enum class Type
        {
            point,
            spot,
            directional
        };

        Light() noexcept = default;
    };
}

#endif // OUZEL_GRAPHICS_RENDERER_LIGHT_HPP