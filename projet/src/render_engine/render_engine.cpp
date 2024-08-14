/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "render_engine.hpp"

#include "image/image.hpp"
#include "image/color.hpp"
#include "image/image_zbuffer.hpp"
#include "lib/3d/vec3.hpp"
#include "lib/3d/vec4.hpp"
#include "lib/3d/mat4.hpp"
#include "image/drawer.hpp"
#include "lib/mesh/mesh.hpp"
#include "lib/common/basic_functions.hpp"

#include <cmath>

namespace cpe
{

    // convert the (x,y) coordinate of a vec3 in discrete pixel position in an image of size (Nx,Ny)
    static ivec2 coordinates_to_pixel_index(vec3 const& coord, int const Nx, int const Ny)
    {
        int const x = (coord.x() + 1.0f) / 2.0f * Nx;
        int const y = (coord.y() + 1.0f) / 2.0f * Ny;

        return ivec2(x, y);
    }

    //helper function converting a vec3 (x,y,z) into a color (r,g,b)
    // (a mesh structure store a color as a vec3)
    static color to_color(vec3 const& p)
    {
        return color(p.x(), p.y(), p.z());
    }


    void render(image& im, image_zbuffer& zbuffer,
        vec3 const& p0, vec3 const& p1, vec3 const& p2,
        color const& c0, color const& c1, color const& c2,
        vec3 const& n0, vec3 const& n1, vec3 const& n2,
        mat4 const& model, mat4 const& view, mat4 const& projection
    )
    {
        vec3 p0_proj, p1_proj, p2_proj;
        color c0_shading, c1_shading, c2_shading;

        //apply vertex shader on the 3 vertices
        vertex_shader(p0_proj, c0_shading, p0, c0, n0, model, view, projection);
        vertex_shader(p1_proj, c1_shading, p1, c1, n1, model, view, projection);
        vertex_shader(p2_proj, c2_shading, p2, c2, n2, model, view, projection);

        //convert normalized coordinates to pixel coordinate
        ivec2 u0 = coordinates_to_pixel_index(p0_proj, im.Nx(), im.Ny());
        ivec2 u1 = coordinates_to_pixel_index(p1_proj, im.Nx(), im.Ny());
        ivec2 u2 = coordinates_to_pixel_index(p2_proj, im.Nx(), im.Ny());

    //draw triangle in the screen space
    draw_triangle(im,zbuffer,u0,u1,u2 , c0_shading,c1_shading,c2_shading ,
                  p0_proj.z(),p1_proj.z(),p2_proj.z() );
}

void render_texture(image& im,image_zbuffer& zbuffer,
            vec3 const& p0,vec3 const& p1,vec3 const& p2,
            color const& c0,color const& c1,color const& c2,
            vec3 const& n0,vec3 const& n1,vec3 const& n2,
            vec2 const& uv0, vec2 const& uv1, vec2 const& uv2, texture const& tex,
            mat4 const& model,mat4 const& view,mat4 const& projection
            )
{
    vec3 p0_proj,p1_proj,p2_proj;
    color c0_shading,c1_shading,c2_shading;

    //apply vertex shader on the 3 vertices
    vertex_shader(p0_proj,c0_shading , p0,c0,n0 , model,view,projection);
    vertex_shader(p1_proj,c1_shading , p1,c1,n1 , model,view,projection);
    vertex_shader(p2_proj,c2_shading , p2,c2,n2 , model,view,projection);

    //convert normalized coordinates to pixel coordinate
    ivec2 u0 = coordinates_to_pixel_index(p0_proj,im.Nx(),im.Ny());
    ivec2 u1 = coordinates_to_pixel_index(p1_proj,im.Nx(),im.Ny());
    ivec2 u2 = coordinates_to_pixel_index(p2_proj,im.Nx(),im.Ny());

    //draw triangle in the screen space
    draw_triangle_texture(im,zbuffer,u0,u1,u2 , c0_shading,c1_shading,c2_shading ,
                  p0_proj.z(),p1_proj.z(),p2_proj.z(), uv0, uv1, uv2, tex );
}

void render(image& im,image_zbuffer& zbuffer,mesh const& m,
            mat4 const& model,mat4 const& view,mat4 const& projection)
{
    /*************************************
    // TO DO
    /*************************************
    // Completez l'affichage d'un maillage
    //
    // Pour tous les triangles
    //   p0,p1,p2 <- coordonnees des sommets du triangle
    //   c0,c1,c2 <- couleurs des sommets du triangle (utilisez la fonction to_color pour convertir un vec3 vers la structure color)
    //   n0,n1,n2 <- normales des sommets du triangle
    //
    //   Affichage du triangle courant
    */
    vec3 p0;
    vec3 p1;
    vec3 p2;
    color c0;
    color c1;
    color c2;
    vec3 n0;
    vec3 n1;
    vec3 n2;
    triangle_index triangle;
    for(int k=0;k<m.size_connectivity();++k){
        triangle = m.connectivity(k);
        p0 = m.vertex(triangle[0]);
        p1 = m.vertex(triangle[1]);
        p2 = m.vertex(triangle[2]);

        c0 = to_color(m.color(triangle[0]));
        c1 = to_color(m.color(triangle[1]));
        c2 = to_color(m.color(triangle[2]));

        n0 = normalized(m.normal(triangle[0]));
        n1 = normalized(m.normal(triangle[1]));
        n2 = normalized(m.normal(triangle[2]));
        render(im,zbuffer,p0,p1,p2,c0,c1,c2,n0,n1,n2,model,view,projection);
    }

}


void render_texture(image& im,image_zbuffer& zbuffer,mesh const& m, texture const& tex,
            mat4 const& model,mat4 const& view,mat4 const& projection)
{
    /*************************************
    // TO DO
    /*************************************
    // Completez l'affichage d'un maillage
    //
    // Pour tous les triangles
    //   p0,p1,p2 <- coordonnees des sommets du triangle
    //   c0,c1,c2 <- couleurs des sommets du triangle (utilisez la fonction to_color pour convertir un vec3 vers la structure color)
    //   n0,n1,n2 <- normales des sommets du triangle
    //
    //   Affichage du triangle courant
    */
    vec3 p0;
    vec3 p1;
    vec3 p2;
    color c0;
    color c1;
    color c2;
    vec3 n0;
    vec3 n1;
    vec3 n2;
    triangle_index triangle;
    for(int k=0;k<m.size_connectivity();++k){
        triangle = m.connectivity(k);
        p0 = m.vertex(triangle[0]);
        p1 = m.vertex(triangle[1]);
        p2 = m.vertex(triangle[2]);

        c0 = to_color(m.color(triangle[0]));
        c1 = to_color(m.color(triangle[1]));
        c2 = to_color(m.color(triangle[2]));

        n0 = normalized(m.normal(triangle[0]));
        n1 = normalized(m.normal(triangle[1]));
        n2 = normalized(m.normal(triangle[2]));
        vec2 const& uv0 = m.texture_coord(triangle[0]);
        vec2 const& uv1 = m.texture_coord(triangle[1]);
        vec2 const& uv2 = m.texture_coord(triangle[2]);

        render_texture(im,zbuffer,p0,p1,p2,c0,c1,c2,n0,n1,n2,uv0,uv1,uv2,tex,model,view,projection);
    }

}



void vertex_shader(vec3& p_proj,color& c_shading,
                   vec3 const& p,color const& c,vec3 const& n,
                   mat4 const& modelview, mat4 const& view,mat4 const& projection)
{
    /*************************************
    // TO DO
    /*************************************
    // Completez le vertex shader
    */
    vec4 p_p(p.x(),p.y(),p.z(),1.0f);
    vec4 p_ps = projection * view * modelview * p_p;
    float w = p_ps.w();
//    p_proj=p;    //a modifier
//    c_shading=c; //a modifier
    p_proj = vec3(p_ps.x()/w, p_ps.y()/w, p_ps.z()/w);
//    p_proj = vec3(p_ps.x(), p_ps.y(), p_ps.z());
    vec3 ul = p-vec3(-1.5f,0.3f,1.0f);
    ul=ul/norm(ul);
    vec3 s =reflected(n,ul);
    vec3 t=-p;
    c_shading=(0.2f + 0.8f*clamp(dot(n,ul)))*c+0.6f*(pow(clamp(dot(s,t)),128))*color(1.0f,1.0f,1.0f);
}




}

