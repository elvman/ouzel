// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include "AudioDevice.hpp"
#include "math/MathUtils.hpp"

namespace ouzel
{
    namespace audio
    {
        AudioDevice::AudioDevice(Audio::Driver aDriver):
            driver(aDriver)
        {
        }

        AudioDevice::~AudioDevice()
        {
        }

        bool AudioDevice::init(bool)
        {
            return true;
        }

        bool AudioDevice::process()
        {
            executeAll();

            return true;
        }

        void AudioDevice::setRenderCommands(const std::vector<RenderCommand>& newRenderCommands)
        {
            std::lock_guard<std::mutex> renderQueueLock(renderQueueMutex);

            renderQueue = newRenderCommands;
        }

        bool AudioDevice::processRenderCommands(uint32_t frames, std::vector<float>& result)
        {
            std::vector<RenderCommand> renderCommands;

            {
                std::lock_guard<std::mutex> renderQueueLock(renderQueueMutex);

                renderCommands = renderQueue;
            }

            std::vector<float> data(frames * channels);

            for (const RenderCommand& renderCommand : renderCommands)
            {
                RenderCommand::ListenerAttributes listenerAttributes;

                if (!processRenderCommand(renderCommand,
                                          frames,
                                          listenerAttributes,
                                          data)) return false;

                for (uint32_t i = 0; i < data.size() && i < result.size(); ++i)
                {
                    // mix the sound into the buffer
                    result[i] += data[i];
                }
            }

            return true;
        }

        bool AudioDevice::processRenderCommand(const RenderCommand& renderCommand,
                                               uint32_t frames,
                                               RenderCommand::ListenerAttributes& listenerAttributes,
                                               std::vector<float>& result)
        {
            std::vector<float> data(frames * channels);

            if (renderCommand.attributeCallback)
            {
                renderCommand.attributeCallback(listenerAttributes);
            }

            for (const RenderCommand& command : renderCommand.renderCommands)
            {
                processRenderCommand(command,
                                     frames,
                                     listenerAttributes,
                                     data);

                for (uint32_t i = 0; i < data.size() && i < result.size(); ++i)
                {
                    // mix the sound into the buffer
                    result[i] += data[i];
                }
            }

            if (renderCommand.renderCallback)
            {
                if (!renderCommand.renderCallback(frames,
                                                  channels,
                                                  sampleRate,
                                                  listenerAttributes,
                                                  result)) return false;
            }

            return true;
        }

        bool AudioDevice::getData(uint32_t frames, std::vector<uint8_t>& result)
        {
            buffer.resize(frames * channels);
            std::fill(buffer.begin(), buffer.end(), 0.0f);

            if (!processRenderCommands(frames, buffer)) return false;

            for (float& f : buffer)
            {
                f = clamp(f, -1.0f, 1.0f);
            }

            switch (format)
            {
                case Audio::Format::SINT16:
                {
                    result.resize(frames * channels * sizeof(int16_t));
                    int16_t* resultPtr = reinterpret_cast<int16_t*>(result.data());

                    for (uint32_t i = 0; i < buffer.size(); ++i)
                    {
                        *resultPtr = static_cast<int16_t>(buffer[i] * 32767.0f);
                        ++resultPtr;
                    }
                    break;
                }
                case Audio::Format::FLOAT32:
                {
                    result.reserve(frames * channels * sizeof(float));
                    result.assign(reinterpret_cast<uint8_t*>(buffer.data()),
                                  reinterpret_cast<uint8_t*>(buffer.data()) + buffer.size() * 4);
                    break;
                }
            }

            return true;
        }

        void AudioDevice::executeOnAudioThread(const std::function<void(void)>& func)
        {
            std::lock_guard<std::mutex> lock(executeMutex);

            executeQueue.push(func);
        }

        void AudioDevice::executeAll()
        {
            std::function<void(void)> func;

            for (;;)
            {
                {
                    std::lock_guard<std::mutex> lock(executeMutex);

                    if (executeQueue.empty())
                    {
                        break;
                    }
                    
                    func = std::move(executeQueue.front());
                    executeQueue.pop();
                }
                
                if (func)
                {
                    func();
                }
            }
        }
    } // namespace audio
} // namespace ouzel
