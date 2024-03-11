#pragma once

#include "Engine/Singleton.hpp"

#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/CollisionEvent.h"
#include "Engine/Events/InputEvent.h"
#include "Engine/Events/SceneEvent.h"
#include "Engine/Events/GameStateEvent.h"
#include "Engine/Events/AnimationEvent.h"
#include "Engine/Events/EventBus.hpp"

#include <map>

namespace MyEngine
{
	class EventFacade : public Singleton<EventFacade>
	{
		friend class Singleton<EventFacade>;
	public:
		EventBus<eWindowEvents, WindowCloseEvent> ebWindow;
		EventBus<eCollisionEvents, RigidBodyCollisionEvent> ebRigidCollision;
		EventBus<eInputEvents, KeyboardEvent> ebKeyboard;
		EventBus<eInputEvents, MouseEvent> ebMouse;
		EventBus<eSceneEvents, SceneChangeEvent> ebSceneChange;
		EventBus<eSceneEvents, EntityAddEvent> ebEntityAdd;
		EventBus<eSceneEvents, EntityRemoveEvent> ebEntityRemove;
		EventBus<eGameStateEvents, GameStartedEvent> ebStartedState;
		EventBus<eGameStateEvents, GameRunningEvent> ebRunningState;
		EventBus<eGameStateEvents, GameStoppedEvent> ebStoppedState;
		EventBus<eGameStateEvents, GameOverEvent> ebGameOverState;
		EventBus<eAnimationEvents, PositionKeyFrameEvent> ebPosKeyFrame;
		EventBus<eAnimationEvents, RotationKeyFrameEvent> ebRotKeyFrame;
		EventBus<eAnimationEvents, ScaleKeyFrameEvent> ebScaleKeyFrame;

	private:
		EventFacade() {};
	};

#define SUBSCRIBE_WINDOW_CLOSE_EVENT(handler) EventFacade::GetInstance()->ebWindow.Subscribe(eWindowEvents::WINDOW_CLOSE, handler);
#define SUBSCRIBE_RIGID_COLLISION_EVENT(handler) EventFacade::GetInstance()->ebRigidCollision.Subscribe(eCollisionEvents::RIGIDBODY, handler);
#define SUBSCRIBE_SOFT_COLLISION_EVENT(handler) EventFacade::GetInstance()->ebSoftCollision.Subscribe(eCollisionEvents::SOFTBODY, handler);
#define SUBSCRIBE_KEYBOARD_EVENT(handler) EventFacade::GetInstance()->ebKeyboard.Subscribe(eInputEvents::KEYBOARD, handler);
#define SUBSCRIBE_MOUSE_EVENT(handler) EventFacade::GetInstance()->ebMouse.Subscribe(eInputEvents::MOUSE, handler);
#define SUBSCRIBE_SCENE_CHANGE_EVENT(handler) EventFacade::GetInstance()->ebSceneChange.Subscribe(eSceneEvents::SCENE_CHANGE, handler);
#define SUBSCRIBE_ENTITY_ADD_EVENT(handler) EventFacade::GetInstance()->ebEntityAdd.Subscribe(eSceneEvents::ENTITY_ADD, handler);
#define SUBSCRIBE_ENTITY_REMOVE_EVENT(handler) EventFacade::GetInstance()->ebEntityRemove.Subscribe(eSceneEvents::ENTITY_REMOVE, handler);
#define SUBSCRIBE_GAME_STARTED_EVENT(handler) EventFacade::GetInstance()->ebStartedState.Subscribe(eGameStateEvents::GAME_STARTED, handler);
#define SUBSCRIBE_GAME_RUNNING_EVENT(handler) EventFacade::GetInstance()->ebRunningState.Subscribe(eGameStateEvents::GAME_RUNNING, handler);
#define SUBSCRIBE_GAME_STOPPED_EVENT(handler) EventFacade::GetInstance()->ebStoppedState.Subscribe(eGameStateEvents::GAME_STOPPED, handler);
#define SUBSCRIBE_GAME_OVER_EVENT(handler) EventFacade::GetInstance()->ebGameOverState.Subscribe(eGameStateEvents::GAME_OVER, handler);
#define SUBSCRIBE_POSITION_KEYFRAME_EVENT(handler) EventFacade::GetInstance()->ebPosKeyFrame.Subscribe(eAnimationEvents::POSITION_KEYFRAME, handler);
#define SUBSCRIBE_ROTATION_KEYFRAME_EVENT(handler) EventFacade::GetInstance()->ebRotKeyFrame.Subscribe(eAnimationEvents::ROTATION_KEYFRAME, handler);
#define SUBSCRIBE_SCALE_KEYFRAME_EVENT(handler) EventFacade::GetInstance()->ebScaleKeyFrame.Subscribe(eAnimationEvents::SCALE_KEYFRAME, handler);

#define UNSUBSCRIBE_WINDOW_CLOSE_EVENT(handler) EventFacade::GetInstance()->ebWindow.Unsubscribe(eWindowEvents::WINDOW_CLOSE, handler);
#define UNSUBSCRIBE_RIGID_COLLISION_EVENT(handler) EventFacade::GetInstance()->ebRigidCollision.Unsubscribe(eCollisionEvents::RIGIDBODY, handler);
#define UNSUBSCRIBE_SOFT_COLLISION_EVENT(handler) EventFacade::GetInstance()->ebSoftCollision.Unsubscribe(eCollisionEvents::SOFTBODY, handler);
#define UNSUBSCRIBE_KEYBOARD_EVENT(handler) EventFacade::GetInstance()->ebKeyboard.Unsubscribe(eInputEvents::KEYBOARD, handler);
#define UNSUBSCRIBE_MOUSE_EVENT(handler) EventFacade::GetInstance()->ebMouse.Unsubscribe(eInputEvents::MOUSE, handler);
#define UNSUBSCRIBE_ENTITY_ADD_EVENT(handler) EventFacade::GetInstance()->ebEntityAdd.Unsubscribe(eSceneEvents::ENTITY_ADD, handler);
#define UNSUBSCRIBE_ENTITY_REMOVE_EVENT(handler) EventFacade::GetInstance()->ebEntityRemove.Unsubscribe(eSceneEvents::ENTITY_REMOVE, handler);
#define UNSUBSCRIBE_SCENE_CHANGE_EVENT(handler) EventFacade::GetInstance()->ebSceneChange.Unsubscribe(eSceneEvents::SCENE_CHANGE, handler);
#define UNSUBSCRIBE_GAME_STARTED_EVENT(handler) EventFacade::GetInstance()->ebStartedState.Unsubscribe(eGameStateEvents::GAME_STARTED, handler);
#define UNSUBSCRIBE_GAME_RUNNING_EVENT(handler) EventFacade::GetInstance()->ebRunningState.Unsubscribe(eGameStateEvents::GAME_RUNNING, handler);
#define UNSUBSCRIBE_GAME_STOPPED_EVENT(handler) EventFacade::GetInstance()->ebStoppedState.Unsubscribe(eGameStateEvents::GAME_STOPPED, handler);
#define UNSUBSCRIBE_GAME_OVER_EVENT(handler) EventFacade::GetInstance()->ebGameOverState.Unsubscribe(eGameStateEvents::GAME_OVER, handler);
#define UNSUBSCRIBE_POSITION_KEYFRAME_EVENT(handler) EventFacade::GetInstance()->ebPosKeyFrame.Unsubscribe(eAnimationEvents::POSITION_KEYFRAME, handler);
#define UNSUBSCRIBE_ROTATION_KEYFRAME_EVENT(handler) EventFacade::GetInstance()->ebRotKeyFrame.Unsubscribe(eAnimationEvents::ROTATION_KEYFRAME, handler);
#define UNSUBSCRIBE_SCALE_KEYFRAME_EVENT(handler) EventFacade::GetInstance()->ebScaleKeyFrame.Unsubscribe(eAnimationEvents::SCALE_KEYFRAME, handler);

#define PUBLISH_WINDOW_CLOSE_EVENT(event) EventFacade::GetInstance()->ebWindow.Publish(event)
#define PUBLISH_RIGID_COLLISION_EVENT(event) EventFacade::GetInstance()->ebRigidCollision.Publish(event)
#define PUBLISH_SOFT_COLLISION_EVENT(event) EventFacade::GetInstance()->ebSoftCollision.Publish(event)
#define PUBLISH_KEYBOARD_EVENT(event) EventFacade::GetInstance()->ebKeyboard.Publish(event)
#define PUBLISH_MOUSE_EVENT(event) EventFacade::GetInstance()->ebMouse.Publish(event)
#define PUBLISH_SCENE_CHANGE_EVENT(event) EventFacade::GetInstance()->ebSceneChange.Publish(event)
#define PUBLISH_ENTITY_ADD_EVENT(event) EventFacade::GetInstance()->ebEntityAdd.Publish(event)
#define PUBLISH_ENTITY_REMOVE_EVENT(event) EventFacade::GetInstance()->ebEntityRemove.Publish(event)
#define PUBLISH_GAME_STARTED_EVENT(event) EventFacade::GetInstance()->ebStartedState.Publish(event)
#define PUBLISH_GAME_RUNNING_EVENT(event) EventFacade::GetInstance()->ebRunningState.Publish(event)
#define PUBLISH_GAME_STOPPED_EVENT(event) EventFacade::GetInstance()->ebStoppedState.Publish(event)
#define PUBLISH_GAME_OVER_EVENT(event) EventFacade::GetInstance()->ebGameOverState.Publish(event)
#define PUBLISH_POSITION_KEYFRAME_EVENT(event) EventFacade::GetInstance()->ebPosKeyFrame.Publish(event)
#define PUBLISH_ROTATION_KEYFRAME_EVENT(event) EventFacade::GetInstance()->ebRotKeyFrame.Publish(event)
#define PUBLISH_SCALE_KEYFRAME_EVENT(event) EventFacade::GetInstance()->ebScaleKeyFrame.Publish(event)


}
