#pragma once
#include "Interface/IRuntimeModule.h"
#include "Utils/Delegate.h"
#include "Utils/Event.h"
#include "Core/Template.h"

#if defined(RK_DESKTOP)
struct GLFWwindow;
#endif
constexpr uint16_t EVENTMANAGER_NUM_QUEUES = 2;

namespace Rocket {
    using EventCallbackFn = std::function<void(EventPtr &)>;
    using EventListenerFnptr = bool(*)(EventPtr&);
    using EventListenerDelegate = Delegate<bool(EventPtr&)>;
    using EventListenerList = List<EventListenerDelegate>;
    using EventListenerMap = Map<EventType, EventListenerList>;
    using EventQueue = List<EventPtr>;
    //using EventThreadQueue = ThreadSafeQueue<EventPtr>;

#define REGISTER_DELEGATE_CLASS(c,f,x,name) {\
    bool ret = false;\
    EventListenerDelegate delegate; \
    delegate.Bind<c,&f>(x); \
    ret = static_cast<EventManager*>(g_EventManager)->AddListener(delegate, ""#name""_hash); \
    RK_CORE_ASSERT(ret, "Register Delegate "#f" To "#name" Failed");}
#define REGISTER_DELEGATE_FN(f,name) {\
    bool ret = false;\
    EventListenerDelegate delegate; \
    delegate.Bind<&f>(); \
    ret = static_cast<EventManager*>(g_EventManager)->AddListener(delegate, ""#name""_hash); \
    RK_CORE_ASSERT(ret, "Register Delegate "#f" To "#name" Failed");}

    class EventManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(EventManager);
    public:
        EventManager(bool global = true);
        virtual ~EventManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;

        void OnEvent(EventPtr& event);

        [[maybe_unused]] bool Update(uint64_t maxMillis = 100);
        [[maybe_unused]] bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
        [[maybe_unused]] bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);
        [[maybe_unused]] bool TriggerEvent(EventPtr& event) const;
        [[maybe_unused]] bool QueueEvent(const EventPtr& event);
        [[maybe_unused]] bool ThreadSafeQueueEvent(const EventPtr& event);
        [[maybe_unused]] bool AbortEvent(const EventType& type, bool allOfType = false);

        // Getter for the main global event manager.
        static EventManager* Get(void);

    private:
        void SetupCallback();
        void SetupListener();
        // Window attributes
		void SetEventCallback(const EventCallbackFn& callback) { window_data_.event_callback = callback; }

    private:
        struct WindowData {
            String title;
            EventCallbackFn event_callback;
        };

        bool        global_;
#if defined(RK_DESKTOP)
        GLFWwindow* window_handle_ = nullptr;
#endif
        WindowData  window_data_;

        int32_t     active_event_queue_;
        EventQueue  event_queue_[EVENTMANAGER_NUM_QUEUES];
        //EventThreadQueue event_thread_queue_;
        EventListenerMap event_listener_;
        
        ElapseTimer    timer_;
        
    private:
        static EventManager* instance_;
    };

    extern IRuntimeModule* g_EventManager;
}
