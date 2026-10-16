// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/render/render_sequence.hpp>

#include<algorithm>
#include<iostream>

#include<boost/bind.hpp>
#include<chrono>

#include<ramen/app/composition.hpp>
#include<ramen/app/application.hpp>

namespace ramen
{
namespace render
{
namespace
{

struct count_outputs
{
    count_outputs() : num_outputs( 0) {}
    void operator()( node_output_interface *out) { ++num_outputs;}
    int num_outputs;
};

struct compare_outputs
{
	bool operator()( node_output_interface *a, node_output_interface *b) const
	{
		return a->priority() > b->priority();
	}
};

} // unnamed

int count_output_nodes( composition_t& comp, bool selected_only)
{
    count_outputs f;
	f = for_each_output( comp, selected_only, f);
	return f.num_outputs;
}

void get_output_nodes( composition_t& comp, std::vector<node_output_interface*> outputs, bool selected_only)
{
	for_each_output( comp, selected_only, [&outputs]( node_output_interface *o) { outputs.push_back( o);});
	std::sort( outputs.begin(), outputs.end(), compare_outputs());
}

int total_frames_to_render( composition_t& comp, int start, int end, bool selected_only)
{
    if( end < start)
		return 0;

    count_outputs f;
    f = for_each_output( comp, selected_only, f);
    return f.num_outputs * (end - start + 1);
}

void render_sequence( composition_t& comp, int start, int end, int proxy_level,
					  int subsample, int mb_extra_samples, float mb_shutter_factor)
{
    if( end < start)
        return;

    context_t new_context = comp.current_context();
    new_context.mode = process_render;
	new_context.proxy_level = proxy_level;
    new_context.subsample = subsample;
    new_context.motion_blur_extra_samples = mb_extra_samples;
    new_context.motion_blur_shutter_factor = mb_shutter_factor;

    for_each_output( comp, false, boost::bind( &node_output_interface::begin_output, _1, start, end));

    int rendered_frames = 0;
	typedef std::chrono::steady_clock clock_type;
	clock_type::time_point frame_start = clock_type::now();
	const clock_type::time_point total_start = frame_start;
	
    for( int i = start; i <= end; ++i, ++rendered_frames)
    {
        new_context.frame = i;

        BOOST_FOREACH( node_t& n, comp.nodes())
        {
            if( node_output_interface *out = dynamic_cast<node_output_interface*>( &n))
            {

                if( n.ignored())
                    continue;

                try
                {
					frame_start = clock_type::now();
                    new_context.result_node = &n;
					out->process_and_write( new_context);
					std::cout << "Render: " << n.name() << ", frame = " << i << ". elapsed = " << std::chrono::duration<double>( clock_type::now() - frame_start).count() << " seconds.";
                }
                catch( std::exception& e)
                {
                    app().error( std::string( "exception thrown during render sequence. what = ") + e.what());
                }
            }
        }
    }
	
    for_each_output( comp, false, boost::bind( &node_output_interface::end_output, _1, true));
	std::cout << "\nRender done. Frames renderered = " << rendered_frames << ". Total time = " << std::chrono::duration<double>( clock_type::now() - total_start).count() << " seconds.\n";
}

} // namespace
} // namespace
