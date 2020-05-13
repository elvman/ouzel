// Copyright 2015-2020 Elviss Strazdins. All rights reserved.

#include <unordered_map>
#import <UIKit/UIPress.h>
#import "ViewIOS.h"
#include "../Engine.hpp"
#include "../Window.hpp"
#include "../../input/ios/InputSystemIOS.hpp"

@implementation ViewIOS

- (void)touchesBegan:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    ouzel::input::InputSystemIOS* inputSystemIOS = static_cast<ouzel::input::InputSystemIOS*>(ouzel::engine->getInputManager()->getInputSystem());
    ouzel::input::TouchpadDevice* touchpadDevice = inputSystemIOS->getTouchpadDevice();
    for (UITouch* touch in touches)
    {
        const CGPoint location = [touch locationInView:self];
        CGFloat force = 1.0;
        if ([touch respondsToSelector:@selector(force)] && [touch respondsToSelector:@selector(maximumPossibleForce)])
            force = (touch.maximumPossibleForce > 0.0F) ? touch.force / touch.maximumPossibleForce : 0.0F;

        std::uint64_t touchId;
        memcpy(&touchId, &touch, sizeof(touch));

        touchpadDevice->handleTouchBegin(touchId,
                                         ouzel::Vector2F(static_cast<float>(location.x / self.bounds.size.width),
                                                         static_cast<float>(location.y / self.bounds.size.height)),
                                         static_cast<float>(force));
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    ouzel::input::InputSystemIOS* inputSystemIOS = static_cast<ouzel::input::InputSystemIOS*>(ouzel::engine->getInputManager()->getInputSystem());
    ouzel::input::TouchpadDevice* touchpadDevice = inputSystemIOS->getTouchpadDevice();
    for (UITouch* touch in touches)
    {
        const CGPoint location = [touch locationInView:self];
        CGFloat force = 1.0;
        if ([touch respondsToSelector:@selector(force)] && [touch respondsToSelector:@selector(maximumPossibleForce)])
            force = (touch.maximumPossibleForce > 0.0F) ? touch.force / touch.maximumPossibleForce : 0.0F;

        std::uint64_t touchId;
        memcpy(&touchId, &touch, sizeof(touch));

        touchpadDevice->handleTouchMove(touchId,
                                        ouzel::Vector2F(static_cast<float>(location.x / self.bounds.size.width),
                                                        static_cast<float>(location.y / self.bounds.size.height)),
                                        static_cast<float>(force));
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    ouzel::input::InputSystemIOS* inputSystemIOS = static_cast<ouzel::input::InputSystemIOS*>(ouzel::engine->getInputManager()->getInputSystem());
    ouzel::input::TouchpadDevice* touchpadDevice = inputSystemIOS->getTouchpadDevice();
    for (UITouch* touch in touches)
    {
        const CGPoint location = [touch locationInView:self];
        CGFloat force = 1.0;
        if ([touch respondsToSelector:@selector(force)] && [touch respondsToSelector:@selector(maximumPossibleForce)])
            force = (touch.maximumPossibleForce > 0.0F) ? touch.force / touch.maximumPossibleForce : 0.0F;

        std::uint64_t touchId;
        memcpy(&touchId, &touch, sizeof(touch));

        touchpadDevice->handleTouchEnd(touchId,
                                       ouzel::Vector2F(static_cast<float>(location.x / self.bounds.size.width),
                                                       static_cast<float>(location.y / self.bounds.size.height)),
                                       static_cast<float>(force));
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    ouzel::input::InputSystemIOS* inputSystemIOS = static_cast<ouzel::input::InputSystemIOS*>(ouzel::engine->getInputManager()->getInputSystem());
    ouzel::input::TouchpadDevice* touchpadDevice = inputSystemIOS->getTouchpadDevice();
    for (UITouch* touch in touches)
    {
        const CGPoint location = [touch locationInView:self];
        CGFloat force = 1.0;
        if ([touch respondsToSelector:@selector(force)] && [touch respondsToSelector:@selector(maximumPossibleForce)])
            force = (touch.maximumPossibleForce > 0.0F) ? touch.force / touch.maximumPossibleForce : 0.0F;

        std::uint64_t touchId;
        memcpy(&touchId, &touch, sizeof(touch));

        touchpadDevice->handleTouchCancel(touchId,
                                          ouzel::Vector2F(static_cast<float>(location.x / self.bounds.size.width),
                                                          static_cast<float>(location.y / self.bounds.size.height)),
                                          static_cast<float>(force));
    }
}

namespace
{
    ouzel::input::Keyboard::Key convertKeyCode(NSInteger keyCode)
    {
        switch (keyCode)
        {
            case UIPressTypeUpArrow: return ouzel::input::Keyboard::Key::up;
            case UIPressTypeDownArrow: return ouzel::input::Keyboard::Key::down;
            case UIPressTypeLeftArrow: return ouzel::input::Keyboard::Key::left;
            case UIPressTypeRightArrow: return ouzel::input::Keyboard::Key::right;
            case UIPressTypeSelect: return ouzel::input::Keyboard::Key::select;
            case UIPressTypeMenu: return ouzel::input::Keyboard::Key::menu;
            case UIPressTypePlayPause: return ouzel::input::Keyboard::Key::pause;
            default: return ouzel::input::Keyboard::Key::none;
        }
    }
}

- (void)pressesBegan:(NSSet<UIPress*>*)presses withEvent:(UIPressesEvent*)event
{
    bool forward = false;

    ouzel::input::InputSystemIOS* inputSystemIOS = static_cast<ouzel::input::InputSystemIOS*>(ouzel::engine->getInputManager()->getInputSystem());
    ouzel::input::KeyboardDevice* keyboardDevice = inputSystemIOS->getKeyboardDevice();
    for (UIPress* press in presses)
    {
        std::future<bool> f = keyboardDevice->handleKeyPress(convertKeyCode(press.type));
        if (press.type == UIPressTypeMenu && !f.get())
            forward = true;
    }

    if (forward)
        [super pressesBegan:presses withEvent:event];
}

- (void)pressesEnded:(NSSet<UIPress*>*)presses withEvent:(UIPressesEvent*)event
{
    bool forward = false;

    ouzel::input::InputSystemIOS* inputSystemIOS = static_cast<ouzel::input::InputSystemIOS*>(ouzel::engine->getInputManager()->getInputSystem());
    ouzel::input::KeyboardDevice* keyboardDevice = inputSystemIOS->getKeyboardDevice();
    for (UIPress* press in presses)
    {
        std::future<bool> f = keyboardDevice->handleKeyRelease(convertKeyCode(press.type));
        if (press.type == UIPressTypeMenu && !f.get())
            forward = true;
    }

    if (forward)
        [super pressesEnded:presses withEvent:event];
}

- (void)pressesCancelled:(NSSet<UIPress*>*)presses withEvent:(UIPressesEvent*)event
{
    bool forward = false;

    ouzel::input::InputSystemIOS* inputSystemIOS = static_cast<ouzel::input::InputSystemIOS*>(ouzel::engine->getInputManager()->getInputSystem());
    ouzel::input::KeyboardDevice* keyboardDevice = inputSystemIOS->getKeyboardDevice();
    for (UIPress* press in presses)
    {
        std::future<bool> f = keyboardDevice->handleKeyRelease(convertKeyCode(press.type));
        if (press.type == UIPressTypeMenu && !f.get())
            forward = true;
    }

    if (forward)
        [super pressesCancelled:presses withEvent:event];
}
@end
