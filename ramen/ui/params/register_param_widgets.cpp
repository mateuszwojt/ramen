// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/param_widget.hpp>

#include<ramen/ui/params/separator_param_widget.hpp>
#include<ramen/ui/params/bool_param_widget.hpp>
#include<ramen/ui/params/button_param_widget.hpp>
#include<ramen/ui/params/string_param_widget.hpp>
#include<ramen/ui/params/popup_param_widget.hpp>
#include<ramen/ui/params/file_param_widget.hpp>
#include<ramen/ui/params/float_param_widget.hpp>
#include<ramen/ui/params/float2_param_widget.hpp>
#include<ramen/ui/params/float3_param_widget.hpp>
#include<ramen/ui/params/color_param_widget.hpp>
#include<ramen/ui/params/image_format_param_widget.hpp>
#include<ramen/ui/params/aspect_ratio_param_widget.hpp>
#include<ramen/ui/params/motion_blur_param_widget.hpp>
#include<ramen/ui/params/ocio_colorspace_param_widget.hpp>
#include<ramen/ui/params/inspector_param_widget.hpp>
#include<ramen/ui/params/composite_param_widget.hpp>
#include<ramen/ui/params/group_param_widget.hpp>
#include<ramen/ui/params/tab_group_param_widget.hpp>
#include<ramen/ui/params/combo_group_param_widget.hpp>
#include<ramen/ui/params/image_seq_param_widget.hpp>
#include<ramen/ui/params/image_channels_param_widget.hpp>
#include<ramen/ui/params/roto_shape_param_widget.hpp>

namespace ramen
{
namespace ui
{

void register_builtin_param_widgets()
{
    // base classes first, the most specific widget registered is used.
    register_param_widget<separator_param_t, separator_param_widget_t>();
    register_param_widget<bool_param_t, bool_param_widget_t>();
    register_param_widget<button_param_t, button_param_widget_t>();
    register_param_widget<string_param_t, string_param_widget_t>();
    register_param_widget<popup_param_t, popup_param_widget_t>();
    register_param_widget<file_param_t, file_param_widget_t>();
    register_param_widget<float_param_t, float_param_widget_t>();
    register_param_widget<float2_param_t, float2_param_widget_t>();
    register_param_widget<float3_param_t, float3_param_widget_t>();
    register_param_widget<color_param_t, color_param_widget_t>();
    register_param_widget<image_format_param_t, image_format_param_widget_t>();
    register_param_widget<aspect_ratio_param_t, aspect_ratio_param_widget_t>();
    register_param_widget<motion_blur_param_t, motion_blur_param_widget_t>();
    register_param_widget<ocio_colorspace_param_t, ocio_colorspace_param_widget_t>();
    register_param_widget<inspector_param_t, inspector_param_widget_t>();
    register_param_widget<composite_param_t, composite_param_widget_t>();
    register_param_widget<group_param_t, group_param_widget_t>();
    register_param_widget<tab_group_param_t, tab_group_param_widget_t>();
    register_param_widget<combo_group_param_t, combo_group_param_widget_t>();
    register_param_widget<image_seq_param_t, image_seq_param_widget_t>();
    register_param_widget<image_channels_param_t, image_channels_param_widget_t>();
    register_param_widget<roto_shape_param_t, roto_shape_param_widget_t>();
}

} // ui
} // ramen
