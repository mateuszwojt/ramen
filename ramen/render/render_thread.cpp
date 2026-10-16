// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/render/render_thread.hpp>

#include<condition_variable>
#include<functional>
#include<future>
#include<mutex>
#include<thread>

#include<ramen/assert.hpp>

namespace ramen
{
namespace render
{

struct render_thread_t::implementation
{
    implementation()
    {
        // start our thread
        finish = false;
        ready = false;
        do_work = false;
        thread = std::thread( &implementation::thread_loop, this);
    }

    ~implementation()
    {
        std::unique_lock<std::mutex> lock( mutex);
        finish = true;
        ready = true;
        do_work = false;
        lock.unlock();
        cond.notify_one();

        // wait for our thread to exit
        thread.join();
    }

    std::future<bool>& run_function( const std::function<void()>& f)
    {
        RAMEN_ASSERT( !do_work && "render_thread_t is not reentrant");
        std::unique_lock<std::mutex> lock( mutex);
        cancel = false;
        ready = true;
        do_work = true;
        task = std::packaged_task<bool()>( std::bind( &implementation::call_fun, this, f));
        future = task.get_future();
        lock.unlock();
        cond.notify_one();
        return future;
    }

private:

    void thread_loop()
    {
        while( 1)
        {
            std::unique_lock<std::mutex> lock( mutex);

            while( !ready)
                cond.wait( lock);

            if( finish)
                break;

            if( do_work)
            {
                do_work = false;
                ready = false;
                task();
            }
        }
    }

    bool call_fun( const std::function<void()>& f)
    {
        f();
        return !cancel;
    }

public:

    std::thread thread;
    bool finish;
    bool ready;
    bool do_work;
    bool cancel;

    std::mutex mutex;
    std::condition_variable cond;

    std::packaged_task<bool()> task;
    std::future<bool> future;    
};

render_thread_t::render_thread_t() : pimpl_( 0) {}
render_thread_t::~render_thread_t() { delete pimpl_;}

void render_thread_t::init()
{
    RAMEN_ASSERT( pimpl_ == 0 && "render_thread_t: init called twice");
    pimpl_ = new implementation();
}

std::future<bool>& render_thread_t::render_image( image_node_renderer_t& renderer)
{
    return render_image( renderer, renderer.format());
}

std::future<bool>& render_thread_t::render_image( image_node_renderer_t& renderer, const Imath::Box2i& roi)
{
    return pimpl_->run_function( [&renderer, roi]() { renderer.render( roi);});
}

bool render_thread_t::cancelled() const { return pimpl_->cancel;}

void render_thread_t::cancel_render() { pimpl_->cancel = true;}

} // namespace
} // namespace
