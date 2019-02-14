// Copyright 2015-2019 Elviss Strazdins. All rights reserved.

#ifndef SOUNDSAMPLE_HPP
#define SOUNDSAMPLE_HPP

#include "ouzel.hpp"

class SoundSample: public ouzel::scene::Scene
{
public:
    SoundSample();

private:
    bool handleGamepad(const ouzel::GamepadEvent& event);
    bool handleUI(const ouzel::UIEvent& event);
    bool handleKeyboard(const ouzel::KeyboardEvent& event) const;

    ouzel::audio::Submix submix;
    ouzel::audio::Listener listener;
    ouzel::audio::Gain soundGain;

    ouzel::audio::Submix test8BitSubmix;
    ouzel::audio::Voice test8BitVoice;
    ouzel::audio::Pitch test8BitPitch;

    ouzel::audio::Submix test24BitSubmix;
    ouzel::audio::Voice test24BitVoice;
    ouzel::audio::Pitch test24BitPitch;

    ouzel::audio::Submix jumpSubmix;
    ouzel::audio::Voice jumpVoice;
    ouzel::audio::Panner jumpPanner;
    ouzel::audio::Reverb jumpReverb;

    ouzel::audio::Voice ambientVoice;
    ouzel::audio::Voice music;
    std::shared_ptr<ouzel::audio::Sound> tone;
    ouzel::audio::Voice toneVoice;

    ouzel::scene::Layer layer;
    ouzel::scene::Camera camera;
    ouzel::scene::Actor cameraActor;
    ouzel::gui::Menu menu;
    ouzel::gui::Button backButton;

    ouzel::scene::Actor soundActor;
    ouzel::gui::Button test8BitButton;
    ouzel::gui::Button test24BitButton;
    ouzel::gui::Button jumpButton;
    ouzel::gui::Button ambientButton;
    ouzel::gui::Button musicButton;
    ouzel::gui::Button toneButton;

    ouzel::EventHandler handler;
};

#endif // SOUNDSAMPLE_HPP
