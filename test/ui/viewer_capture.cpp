// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

// Opens a composition in the user interface, makes a node active and saves
// what the viewer shows: viewer_capture comp.rmn node out.png [delay ms]

#include<cstdlib>
#include<iostream>
#include<string>

#include<QApplication>
#include<QImage>
#include<QOpenGLWidget>
#include<QTimer>

#include<ramen/app/application.hpp>
#include<ramen/app/composition.hpp>
#include<ramen/app/document.hpp>

#include<ramen/filesystem/path.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/viewer/viewer.hpp>

using namespace ramen;

int main( int argc, char **argv)
{
    if( argc < 4)
    {
        std::cerr << "usage: viewer_capture comp.rmn node out.png [delay ms]\n";
        return EXIT_FAILURE;
    }

    const std::string comp( argv[1]);
    const std::string node_name( argv[2]);
    const std::string output( argv[3]);
    const int delay = argc > 4 ? std::atoi( argv[4]) : 3000;

    char *app_argv[] = { argv[0], 0};
    application_t app( 1, app_argv);

    int result = EXIT_FAILURE;

    QTimer::singleShot( 0, [&]()
    {
        app.ui()->open_document( std::filesystem::absolute( filesystem::path_from_utf8( comp)));

        node_t *n = app.document().composition().find_node( node_name);

        if( !n)
        {
            std::cerr << "viewer_capture: node " << node_name << " not found\n";
            app.set_quitting( true);
            qApp->exit( EXIT_FAILURE);
            return;
        }

        app.ui()->set_active_node( n);
    });

    QTimer::singleShot( delay, [&]()
    {
        QImage img( app.ui()->viewer().gl_widget()->grabFramebuffer());

        if( img.isNull() || !img.save( QString::fromStdString( output)))
        {
            std::cerr << "viewer_capture: couldn't save " << output << "\n";
            app.set_quitting( true);
            qApp->exit( EXIT_FAILURE);
            return;
        }

        result = EXIT_SUCCESS;
        app.set_quitting( true);
        qApp->exit( EXIT_SUCCESS);
    });

    app.run();
    return result;
}
