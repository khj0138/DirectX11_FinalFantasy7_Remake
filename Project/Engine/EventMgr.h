#pragma once
#include "Singleton.h"

namespace ff7r
{
    class EventMgr : public Singleton<EventMgr>
    {
    public:
        void Tick();

        void AddEvent(const Event& _evn) { events.push_back(_evn); }
        bool IsLevelChanged() { return is_level_changed; }

    private:
        SINGLE(EventMgr);

        void GC_Clear();

        vector<Event>          events;
        vector<GameObject*>    delete_objects;

        bool                    is_level_changed;
    };
}
