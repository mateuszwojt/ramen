// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/composite_param.hpp>

#include<string>
#include<stdexcept>

#include<functional>


#include<algorithm>

#include<QWidget>
#include<QVBoxLayout>
#include<QFrame>

#include<ramen/params/param_set.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{

composite_param_t::composite_param_t() : param_t(), create_track_( true) {}
composite_param_t::composite_param_t( const std::string& name) : param_t( name), create_track_( true) {}
composite_param_t::composite_param_t( const composite_param_t& other) : param_t( other), params_( other.params_)
{
    create_track_ = other.create_track_;
}

void composite_param_t::do_init()
{
    std::ranges::for_each( params(), std::bind( &param_t::init, std::placeholders::_1));
}

void composite_param_t::do_set_param_set( param_set_t *parent)
{
    for( param_t& p : params())
        p.set_param_set( parent);
}

void composite_param_t::do_add_param( param_t *p)
{
    p->set_param_set( param_set());

    if( param_set())
    {
        try
        {
            param_set()->find( p->id());
            throw std::runtime_error( std::string( "Duplicated param id found: ").append( p->id()));
        }
        catch( std::runtime_error& e) {}
    }
    else
    {
        if( find( p->id()))
            throw std::runtime_error( std::string( "Duplicated param id found: ").append( p->id()));
    }

    params().push_back( p);
}

const param_t *composite_param_t::find( const std::string& id) const
{
    composite_param_t& self = const_cast<composite_param_t&>( *this);
    return self.find( id);
}

param_t *composite_param_t::find( const std::string& id)
{
    for( param_t& p : params())
    {
        if( p.id() == id)
            return &p;

        if( composite_param_t *cp = dynamic_cast<composite_param_t*>( &p))
        {
            param_t *q = cp->find( id);

            if( q)
                return q;
        }
    }

    return 0;
}

void composite_param_t::do_set_frame( float frame)
{
    std::ranges::for_each( params(), std::bind( &param_t::set_frame, std::placeholders::_1, frame));
}

void composite_param_t::do_create_tracks( anim::track_t *parent)
{
    if( create_track_)
    {
        std::unique_ptr<anim::track_t> t( new anim::track_t( name()));
        std::ranges::for_each( params(), std::bind( &param_t::create_tracks, std::placeholders::_1, t.get()));

        if( t->num_children() != 0)
            parent->add_child( std::move( t));
    }
    else
        std::ranges::for_each( params(), std::bind( &param_t::create_tracks, std::placeholders::_1, parent));
}

void composite_param_t::do_evaluate( float frame)
{
    std::ranges::for_each( params(), std::bind( &param_t::evaluate, std::placeholders::_1, frame));
}

void composite_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    for( const param_t& p : params())
        p.add_to_hash( hash_gen);
}

void composite_param_t::do_update_widgets()
{
    std::ranges::for_each( params_, std::bind( &param_t::update_widgets, std::placeholders::_1));
}

void composite_param_t::do_enable_widgets( bool e)
{
    std::ranges::for_each( params_, std::bind( &param_t::enable_widgets, std::placeholders::_1, e));
}

void composite_param_t::do_format_changed( const Imath::Box2i& new_format, float aspect, const Imath::V2f& proxy_scale)
{
    std::ranges::for_each( params_, std::bind( &param_t::format_changed, std::placeholders::_1, new_format, aspect, proxy_scale));
}

void composite_param_t::do_convert_relative_paths( const std::filesystem::path& old_base, const std::filesystem::path& new_base)
{
    std::ranges::for_each( params_, std::bind( &param_t::convert_relative_paths, std::placeholders::_1, old_base, new_base));
}

void composite_param_t::do_make_paths_absolute()
{
    std::ranges::for_each( params_, std::bind( &param_t::make_paths_absolute, std::placeholders::_1));
}

void composite_param_t::do_make_paths_relative()
{
    std::ranges::for_each( params(), std::bind( &param_t::make_paths_relative, std::placeholders::_1));
}

// util
void composite_param_t::do_apply_function( const std::function<void ( param_t*)>& f)
{
    std::ranges::for_each( params(), std::bind( &param_t::apply_function, std::placeholders::_1, f));
}

void composite_param_t::do_read( serialization::yaml_iarchive_t& node)
{
    RAMEN_ASSERT( param_set());

    serialization::yaml_node_t nodes = node.get_node( "children");

    for( int i = 0; i < nodes.size(); ++i)
        param_set()->read_param( nodes[i]);
}

void composite_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "children" << YAML::Value;
        out.begin_seq();
            std::ranges::for_each( params(), std::bind( &param_t::write, std::placeholders::_1, std::ref( out)));
        out.end_seq();
}

QWidget *composite_param_t::do_create_widgets()
{
    QWidget *widget = new QWidget();
    create_widgets_inside_widget( widget);
    return widget;
}

void composite_param_t::create_widgets_inside_widget( QWidget *parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins( 0, 0, 0, 0);
    layout->setSpacing( 5);
    layout->setSizeConstraint( QLayout::SetFixedSize);

    for( param_t& p : params())
    {
        if( !p.secret())
        {
            QWidget *w = p.create_widgets();

            if( w)
                layout->addWidget( w);
        }
    }

    layout->addStretch();
    parent->setLayout( layout);
}

} // namespace
