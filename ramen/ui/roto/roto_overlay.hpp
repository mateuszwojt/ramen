// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_ROTO_ROTO_OVERLAY_HPP
#define RAMEN_UI_ROTO_ROTO_OVERLAY_HPP

#include<ramen/ui/manipulators/manipulable.hpp>

#include<QPointer>

#include<ramen/nodes/image/roto/roto_node_fwd.hpp>
#include<ramen/ui/roto/toolbar_fwd.hpp>

namespace ramen
{
namespace roto
{

/// Viewer overlay of the roto node. Draws the shapes and sends events to the active tool.
class roto_overlay_t : public manipulable_t
{
public:

    explicit roto_overlay_t( image::roto_node_t& node);

    std::unique_ptr<QWidget> create_toolbar() override;

    /// The toolbar with the roto tools. Null until the viewer creates it.
    toolbar_t *toolbar() const;

private:

    image::roto_node_t& node_;
    QPointer<toolbar_t> toolbar_;
};

} // roto
} // ramen

#endif
