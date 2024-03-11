#include <gtest/gtest.h>
#include <iostream>

#include <Engine/Events/EventsFacade.h>
#include <Engine/Events/WindowEvents.h>

using namespace MyEngine;

class EventBusTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(EventBusTest, SubscribeAndPublishEvent)
{
    bool eventHandled = false;

    // Subscribe to the "WINDOW_CLOSE" event
    SUBSCRIBE_WINDOW_CLOSE_EVENT([&eventHandled](const Event<eWindowEvents>& event) {
        eventHandled = true;
        });

    // Publish the "WINDOW_CLOSE" event
    WindowCloseEvent event = WindowCloseEvent();
    PUBLISH_WINDOW_CLOSE_EVENT(event);

    // Assert that the event was handled
    EXPECT_TRUE(eventHandled);
}

TEST_F(EventBusTest, PublishEventWithNoSubscribers)
{
    // Publish the "WINDOW_CLOSE" event
    // This should not cause any issues even if there are no subscribers
    WindowCloseEvent event = WindowCloseEvent();
    ASSERT_NO_THROW(PUBLISH_WINDOW_CLOSE_EVENT(event));
}

TEST_F(EventBusTest, AccessUnsetEventBus)
{
    // Attempt to publish an event without a set eventBus
    WindowCloseEvent event = WindowCloseEvent();
    ASSERT_NO_THROW(PUBLISH_WINDOW_CLOSE_EVENT(event));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
