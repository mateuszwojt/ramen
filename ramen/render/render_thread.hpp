// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_RENDER_RENDER_THREAD_HPP
#define RAMEN_RENDER_RENDER_THREAD_HPP

#include<functional>
#include<thread>
#include<future>
#include<mutex>
#include<condition_variable>

#include<ramen/render/image_node_renderer.hpp>

namespace ramen
{
namespace render
{

struct RAMEN_API render_thread_t
{
    render_thread_t();
    ~render_thread_t();

    void init();

    std::future<bool>& render_image( image_node_renderer_t& renderer);
    std::future<bool>& render_image( image_node_renderer_t& renderer, const Imath::Box2i& roi);

    bool cancelled() const;
    void cancel_render();

private:

    struct implementation;
    implementation *pimpl_;
};

} // namespace
} // namespace

#endif
