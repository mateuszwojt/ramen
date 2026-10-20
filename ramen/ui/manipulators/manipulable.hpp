// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MANIPULABLE_HPP
#define RAMEN_MANIPULABLE_HPP

#include<memory>

#include<ramen/core/noncopyable.hpp>
#include<ramen/container/ptr_vector.hpp>
#include<ramen/core/signal.hpp>

#include<ramen/ui/manipulators/manipulator.hpp>

#include<ramen/params/parameterised_fwd.hpp>

class QWidget;

namespace ramen
{

/*!
\ingroup manips
\brief Viewer overlay of an object, like a node. Has manipulators, reacts to events and draws using GL.
*/
class RAMEN_API manipulable_t : ramen::core::noncopyable_t
{
public:

    explicit manipulable_t( parameterised_t& target);

    virtual ~manipulable_t();

    /// The object the manipulators edit.
    const parameterised_t& target() const   { return target_;}
    parameterised_t& target()               { return target_;}

    /// Creates the viewer toolbar for this overlay. Returns null if it does not have one.
    virtual std::unique_ptr<QWidget> create_toolbar();


    template<class T>
    void add_manipulator( std::unique_ptr<T> p)
    {
        p->set_parent( this);
        manipulators_.push_back( p.release());
        active_ = manipulators_.end();
    }

    const container::ptr_vector<manipulator_t>& manipulators() const    { return manipulators_;}
    container::ptr_vector<manipulator_t>& manipulators()                { return manipulators_;}

    typedef container::ptr_vector<manipulator_t>::const_iterator const_iterator;
    typedef container::ptr_vector<manipulator_t>::iterator iterator;

    const_iterator begin() const    { return manipulators_.begin();}
    const_iterator end() const	    { return manipulators_.end();}

    iterator begin()	{ return manipulators_.begin();}
    iterator end()	{ return manipulators_.end();}

    /// Asks the viewer to redraw the overlay.
    void update_overlay();

    /// Called when the overlay needs redrawing.
    void draw_overlay( const ui::paint_event_t& event) const;

    /// Called when a key_press_event_t is recieved.
    bool key_press_event( const ui::key_press_event_t& event);

    /// Called when a key_release_event_t is recieved.
    void key_release_event( const ui::key_release_event_t& event);

    /// Called when a mouse_enter_event_t is recieved.
    void mouse_enter_event( const ui::mouse_enter_event_t& event);

    /// Called when a mouse_leave_event_t is recieved.
    void mouse_leave_event( const ui::mouse_leave_event_t& event);

    /// Called when a mouse_press_event_t is recieved.
    bool mouse_press_event( const ui::mouse_press_event_t& event);

    /// Called when a mouse_move_event_t is recieved.
    void mouse_move_event( const ui::mouse_move_event_t& event);

    /// Called when a mouse_drag_event_t is recieved.
    void mouse_drag_event( const ui::mouse_drag_event_t& event);

    /// Called when a mouse_release_event_t is recieved.
    void mouse_release_event( const ui::mouse_release_event_t& event);

protected:

    /*!
        \brief Customization hook for manipulable_t::draw_overlay.
        Implement in subclasses to draw a visual representation of the manipulator.
    */
    virtual void do_draw_overlay( const ui::paint_event_t& event) const;

    /*!
        \brief Customization hook for manipulable_t::key_press_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual bool do_key_press_event( const ui::key_press_event_t& event);

    /*!
        \brief Customization hook for manipulable_t::key_release_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_key_release_event( const ui::key_release_event_t& event);

    /*!
        \brief Customization hook for manipulable_t::mouse_enter_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_enter_event( const ui::mouse_enter_event_t& event);

    /*!
        \brief Customization hook for manipulable_t::mouse_leave_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_leave_event( const ui::mouse_leave_event_t& event);

    /*!
        \brief Customization hook for manipulable_t::mouse_press_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual bool do_mouse_press_event( const ui::mouse_press_event_t& event);

    /*!
        \brief Customization hook for manipulable_t::mouse_move_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_move_event( const ui::mouse_move_event_t& event);

    /*!
        \brief Customization hook for manipulable_t::mouse_drag_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_drag_event( const ui::mouse_drag_event_t& event);

    /*!
        \brief Customization hook for manipulable_t::mouse_release_event.
        Implement in subclasses if you need to handle this event class.
    */    virtual void do_mouse_release_event( const ui::mouse_release_event_t& event);

private:


    parameterised_t& target_;
    container::ptr_vector<manipulator_t> manipulators_;
    iterator active_;
};

} // namespace

#endif
