
/** TP 5ETI IMI- CPE Lyon - 2015-2016 */



#include "lib/common/error_handling.hpp"

#include "discrete/bresenham.hpp"
#include "discrete/line_discrete.hpp"
#include "discrete/line_interpolation_parameter.hpp"
#include "image/image.hpp"
#include "image/drawer.hpp"
#include "discrete/triangle_scanline.hpp"
#include "image/image_zbuffer.hpp"
#include "lib/mesh/mesh.hpp"
#include "lib/mesh/mesh_io.hpp"
#include "render_engine/render_engine.hpp"
#include "lib/3d/mat4.hpp"
#include "lib/3d/mat3.hpp"
#include "image/texture.hpp"

#include <iostream>
#include <fstream>
#include <cmath>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510582
#endif
using namespace cpe;

int main(int argc,char *argv[])
{

    std::cout<<"***********************"<<std::endl;
    std::cout<<"run "<<argv[0]<<" with "<<argc-1<<" parameters ... \n"<<std::endl;
    //float const M_PI = 3.14159265359;

    try
    {
        int const Nx = 40;
        int const Ny = 40;

        image im(Nx, Ny);
        image_zbuffer zbuffer(Nx, Ny);
        image im_ligne(40, 40);
        image_zbuffer zbuffer_ligne(40, 40);
        im_ligne.fill({ 1.0f,1.0f,1.0f });
        draw_line(im_ligne, { 3,2 }, { 16,19 }, color(1.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 1.0f));
        draw_line(im_ligne, { 30,10 }, { 21,30 }, color(1.0f, 0.0f, 0.0f), color(1.0f, 0.0f, 0.0f));
        draw_line(im_ligne, { 38,20 }, { 16,17 }, color(1.0f, 1.0f, 0.0f), color(0.0f, 1.0f, 1.0f));
        im_ligne.save("my_diago_autre_octant.ppm");
        //        im_ligne.save("ligne.png");
        im.fill({ 1.0f,1.0f,1.0f });

        im(20, 10) = color(1.0f, 1.0f, 0.0f);
        im(21, 10) = color(1.0f, 0.0f, 0.0f);

        for (int k = 0; k < 20; ++k) 
            im(10 + k, 5) = color(k / 20.0f, 0.0f, 1.0f - k / 20.0f);
        for (int k = 0; k < 20; ++k)
            im(15, k) = color(k / 20.0f);
        std::cout << std::endl;

        line_discrete line; // creation d'une ligne discrete
        line.push_back(ivec2(4, 5)); // ajout d'une position
        line.push_back({ 5,6 }); // autre methode d'ajout
        ivec2 const p(6, 7);
        line.push_back(p); // autre methode d'ajout
        std::cout << line << std::endl; // affichage du contenu de la ligne
        //premiere methode de parcours de la ligne
        for (int k = 0; k < line.size(); ++k)
            std::cout << line[k] << " ";
        std::cout << std::endl;
        //deuxieme methode de parcours de la ligne
        for (ivec2 const& p : line)
            std::cout << p << " ";
        std::cout << std::endl;
        line_discrete line2;
        ivec2 p0 = { 30,7 };
        ivec2 p1 = { 20,4 };
        ivec2 p2 = { 10,10 };
        //        ivec2 p0 = {0,0};
        //        ivec2 p1 = {0,1};
        //        ivec2 p2 = {1,1};
        //        color ctriangle = color(0.3f,0.7f,1.0f);
        bresenham(p0, p1, line2);
        std::cout << line2 << std::endl;
        draw_line(im, p0, p1, color(1.0f, 1.0f, 0.0f));
        line_discrete lined;
        bresenham({ 5,5 }, { 5,5 }, lined);
        std::cout << lined << std::endl;
        line_interpolation_parameter interpolation(lined);
        std::cout << "interpolation" << std::endl;
        std::cout << interpolation << std::endl; // if we have just one point the value of interpolation is 0
        // usually values are between 0 and 1
//        draw_line(im,p0,p1,color(1.0f, 1.0f, 0.0f),color(0.0f, 1.0f, 0.0f));
        draw_triangle_wireframe(im, p0, p1, p2, color(0.0f, 0.0f, 1.0f));
        //        draw_triangle_wireframe(im , {15,12},{2,17},{6,3} , {0,0,0});
        //        auto scanline = triangle_scanline_factory({15,12},{2,17},{6,3} , 0.0f,1.0f,2.0f);
        //        auto scanline = triangle_scanline_factory(p0,p1,p2 , 0.0f,1.0f,2.0f);
        //        std::cout<<"scanline\n"<<scanline<<std::endl;
        //        draw_triangle_wireframe(im , {15,12},{2,17},{6,3} , {0,0,0});
        auto scanline = triangle_scanline_factory({ 15,12 }, { 2,17 }, { 6,3 }, color(1, 0, 0), color(0, 1, 0), color(0, 0, 1));
        std::cout << scanline << std::endl;
        image imtri(Nx, Ny);
        imtri.fill({ 1.0f,1.0f,1.0f });
        image_zbuffer zbuffertri(Nx, Ny);
        draw_triangle_fade(imtri, { 6,10 }, { 3,30 }, { 20,17 }, color(1, 0, 0), color(0, 1, 0), color(0, 0, 1));
        imtri.save("montritri_color.ppm");
        color c0 = color(1, 0, 0);
        color c1 = color(0, 1, 0);
        color c2 = color(0, 0, 1);
        float z0 = 0.2f;
        float z1 = 0.7f;
        float z2 = -0.5f;
        //        draw_triangle_fade(im, {15,12},{2,17},{6,3} , color(1,0,0),color(0,1,0),color(0,0,1));
        draw_triangle(im, zbuffer, { 6,10 }, { 3,30 }, { 20,17 }, c0, c1, c2, z0, z1, z2);
        draw_triangle(im, zbuffer, { 15,12 }, { 2,17 }, { 6,3 }, color(1, 1, 0), color(1, 1, 0), color(1, 1, 0), 0.7F, -0.7F, 0.5F);
        im.save("mon_image.ppm");
        //        image improj_2(400,400);
        //        image_zbuffer zbuffer_proj_2(400,400);
        //        improj_2.fill({1.0f,1.0f,1.0f});
        //        draw_triangle(improj_2, zbuffer,{60,100},{3,300},{200,170},c0,c1,c2, z0, z1, z2);
        //        draw_triangle(improj_2,zbuffer,{150,120},{2,170},{60,300},color(1,1,0), color(1,1,0),color(1,1,0),0.7F,-0.7F,0.5F);
        //        improj_2.save("proj_2.ppm");
        image improj(Nx, Ny);
        image_zbuffer zbuffer_proj(Nx, Ny);
        improj.fill({ 1.0f,1.0f,1.0f });
        vec3 n0(0, 1, 1);
        vec3 n1(0, 0, 1);
        vec3 n2(0, 0, 1);
        mat4 model; //identitee
        mat4 view; //identitee
        mat4 projection;
        projection.set_projection_perspective(60 * M_PI / 180.0f, 1.0f, 0.1f, 20.0f);
        //        render(improj,zbuffer_proj,{2.0f,1.0f,-7.0f},{3.0f,-2.5f,-5.0f},{-1.0f,2.0f,-15.0f},color(1,1,0),color(1,1,0),color(1,1,0),{0,0,1},{0,0,1},{0,0,1}, model, view, projection);
        render(improj, zbuffer_proj, { -3.0f,1.5f,-5.0f }, { 1.0f,1.5f,-3.0f }, { -1.0f,1.0f,-20.0f }, c0, c1, c2, n0, n1, n2, model, view, projection);
        //std::cout<<im(91,10)<<std::endl; // error because of the size
        render(improj, zbuffer_proj, { 2.0f,1.0f,-7.0f }, { 3.0f,-2.5f,-5.0f }, { -1.0f,2.0f,-15.0f }, color(1, 1, 0), color(1, 1, 0), color(1, 1, 0), { 0,0,1 }, { 0,0,1 }, { 0,0,1 }, model, view, projection);
        //        std::cout<<view<<std::endl;

        improj.save("mvp.ppm");
        image immesh(1024, 1024);
        immesh.fill({ 1.0f,1.0f,1.0f });
        image_zbuffer zbuffer_mesh(1024, 1024);

        //chargement du fichier
//        mesh m = load_mesh_file("/home/timothee/Documents/5 ETI/Rasterisation/Rendu projectif/5imi-rasterisation-teklaw7/projet/data/dino.obj");
        mesh m = load_mesh_file("data/dino.obj");
        //applique potentiellement une rotation, translation
        // (scaling possible egalement)
        m.transform_apply_rotation({ 0,1,0 }, -M_PI / 4.0f);
        m.transform_apply_translation({ 0,0,-1.5f });
        m.fill_color({ 1,1,1 }); //applique la couleur blanche a
        // l'ensemble des sommets
        m.fill_normal(); //calcul automatique des normales
        render(immesh, zbuffer_mesh, m, model, view, projection);
        immesh.save("dino.ppm");
        texture tex;
        tex.load("data/dino.ppm");
        std::cout << m.size_connectivity() << std::endl;
        image im_texture(1024, 1024);
        image_zbuffer zbuffer_texture(1024, 1024);
        im_texture.fill({ 1,1,1 });
        render_texture(im_texture, zbuffer_texture, m, tex, model, view, projection);
        im_texture.save("dino_texture.ppm");
        int N = 1024;
        image implane(N, N);
        image_zbuffer zbuffer_plane(N, N);
        implane.fill({ 1,1,1 });
        mesh plane = load_mesh_file("data/plane.obj");
        plane.transform_apply_scale(0.7f);
        plane.transform_apply_translation({ 0,0.34f,-1.5f });
        plane.transform_opposite_normal_orientation();
        texture tex_plane;
        tex_plane.load("data/plane.ppm");
        render_texture(implane, zbuffer_plane, plane, tex_plane, model, view, projection);
        implane.save("plane.ppm");
        std::cout << "plane" << std::endl;
    }
    catch(cpe::exception_cpe const& e)
    {


        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"Exception found"<<std::endl;
        std::cout<<e.info()<<std::endl;
        std::cout<<"\nSaving backtrace in file backtrace_log.txt"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;


        std::ofstream back_file("backtrace_log.txt");
        back_file<<e.info()<<std::endl;
        back_file<<e.info_backtrace();
        back_file.close();

        exit(1);
    }


    std::cout<<"Exit Main"<<std::endl;

    return 0;
}