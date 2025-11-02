#include "utils.hpp"

#include <pbrlib/event_system.hpp>

#include <string_view>
#include <atomic>

using namespace std::literals;

class EventSystemTests : 
    public ::testing::Test
{
public:
    EventSystemTests() : _call_count{0} {}

    void SetUp() override
    {
        _call_count = 0;
    }

    void TearDown() override
    {
        _call_count = 0;
    }

protected:
    std::atomic<int> _call_count{0};
};

TEST_F(EventSystemTests, EmitOn)
{
    struct TestEvent final
    {
        int value = 0;
    };

    constexpr int value = 123;

    int received_value = -1;

    pbrlib::EventSystem::on([this, &received_value] (const TestEvent& event) {
        received_value = event.value;
        ++_call_count;
    });

    const TestEvent event {
        .value = value
    };

    pbrlib::EventSystem::emmit(event);
    
    pbrlib::testing::equality(received_value, value);
    pbrlib::testing::equality(_call_count.load(), 1);
}

TEST_F(EventSystemTests, MultipleListeners)
{
    struct TestEvent final
    { };

    bool event_1 = false;
    bool event_2 = false;
    bool event_3 = false;

    pbrlib::EventSystem::on([this, &event_1] ([[maybe_unused]] const TestEvent& event) {
        event_1 = true;
        ++_call_count;
    });

    pbrlib::EventSystem::on([this, &event_2] ([[maybe_unused]] const TestEvent& event) {
        event_2 = true;
        ++_call_count;
    });

    pbrlib::EventSystem::on([this, &event_3] ([[maybe_unused]] const TestEvent& event) {
        event_3 = true;
        ++_call_count;
    });

    pbrlib::EventSystem::emmit(TestEvent());

    pbrlib::testing::thisTrue(event_1);
    pbrlib::testing::thisTrue(event_2);
    pbrlib::testing::thisTrue(event_3);

    constexpr int call_count = 3;
    pbrlib::testing::equality(_call_count, call_count);
}

TEST_F(EventSystemTests, DifferentEventTypes)
{
    struct TestEvent1 final
    { };
    
    struct TestEvent2 final
    { };

    uint8_t test_event_1 = 0;
    uint8_t test_event_2 = 0;

    pbrlib::EventSystem::on([&test_event_1] ([[maybe_unused]] const TestEvent1& event) {
        ++test_event_1;
    });

    pbrlib::EventSystem::on([&test_event_2] ([[maybe_unused]] const TestEvent2& event) {
        ++test_event_2;
    });

    pbrlib::EventSystem::emmit(TestEvent1());
    pbrlib::EventSystem::emmit(TestEvent2());

    pbrlib::testing::equality(test_event_1, 1);
    pbrlib::testing::equality(test_event_2, 1);
}

TEST_F(EventSystemTests, EventWithComplexData)
{
    struct TestEvent final
    {
        uint32_t            value_1 = 0;
        float               value_2 = 0.0;
        std::string_view    str;
    };

    static constexpr uint32_t   value_1 = 432;
    static constexpr float      value_2 = 0.4534;
    static constexpr auto       str     = "Hello World"sv;

    pbrlib::EventSystem::on([](const TestEvent& event) {
        pbrlib::testing::equality(event.value_1, value_1);
        pbrlib::testing::equality(event.value_2, value_2);
        pbrlib::testing::equality(event.str, str);
    });

    constexpr TestEvent event = {
        .value_1    = value_1,
        .value_2    = value_2,
        .str        = str
    };

    pbrlib::EventSystem::emmit(event);
}
