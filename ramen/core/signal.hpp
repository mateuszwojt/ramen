// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CORE_SIGNAL_HPP
#define RAMEN_CORE_SIGNAL_HPP

#include<atomic>
#include<functional>
#include<memory>
#include<mutex>
#include<vector>

namespace ramen
{
namespace core
{
namespace detail
{

struct slot_state_t
{
    std::atomic<bool> connected{ true};
};

} // detail

/*!
\ingroup core
\brief Handle to a slot connected to a signal_t. Copies refer to the same connection.
*/
class connection_t
{
public:

    connection_t() = default;

    explicit connection_t( const std::shared_ptr<detail::slot_state_t>& state) : state_( state) {}

    bool connected() const
    {
        std::shared_ptr<detail::slot_state_t> s( state_.lock());
        return s && s->connected;
    }

    void disconnect()
    {
        if( std::shared_ptr<detail::slot_state_t> s = state_.lock())
            s->connected = false;
    }

private:

    std::weak_ptr<detail::slot_state_t> state_;
};

template<class Signature>
class signal_t;

/*!
\ingroup core
\brief A minimal thread safe signal. Slots are called in connection order.
*/
template<class... Args>
class signal_t<void ( Args...)>
{
public:

    typedef std::function<void ( Args...)> slot_type;

    signal_t() = default;

    signal_t( const signal_t&) = delete;
    signal_t& operator=( const signal_t&) = delete;

    ~signal_t() { disconnect_all_slots();}

    connection_t connect( slot_type f)
    {
        std::shared_ptr<slot_t> s( std::make_shared<slot_t>( std::move( f)));

        std::lock_guard<std::mutex> lock( mutex_);
        remove_disconnected_slots();
        slots_.push_back( s);
        return connection_t( s->state);
    }

    void disconnect_all_slots()
    {
        std::lock_guard<std::mutex> lock( mutex_);

        for( const std::shared_ptr<slot_t>& s : slots_)
            s->state->connected = false;

        slots_.clear();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock( mutex_);

        for( const std::shared_ptr<slot_t>& s : slots_)
        {
            if( s->state->connected)
                return false;
        }

        return true;
    }

    void operator()( Args... args) const
    {
        // call the slots without holding the lock, so that they can connect and disconnect.
        std::vector<std::shared_ptr<slot_t> > slots;
        {
            std::lock_guard<std::mutex> lock( mutex_);
            slots = slots_;
        }

        for( const std::shared_ptr<slot_t>& s : slots)
        {
            if( s->state->connected)
                s->fun( args...);
        }
    }

private:

    struct slot_t
    {
        explicit slot_t( slot_type f) : fun( std::move( f)), state( std::make_shared<detail::slot_state_t>()) {}

        slot_type fun;
        std::shared_ptr<detail::slot_state_t> state;
    };

    void remove_disconnected_slots()
    {
        std::erase_if( slots_, []( const std::shared_ptr<slot_t>& s) { return !s->state->connected;});
    }

    mutable std::mutex mutex_;
    std::vector<std::shared_ptr<slot_t> > slots_;
};

} // core
} // ramen

#endif
