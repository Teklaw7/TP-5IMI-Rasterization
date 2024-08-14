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


#include "drawer.hpp"

#include "discrete/ivec2.hpp"
#include "discrete/line_discrete.hpp"
#include "discrete/line_interpolation_parameter.hpp"
#include "discrete/bresenham.hpp"
#include "discrete/triangle_scanline.hpp"
#include "image.hpp"
#include "image_zbuffer.hpp"
#include "color.hpp"
#include "lib/3d/vec2.hpp"
#include "lib/3d/vec3.hpp"
#include "lib/3d/vec3.hpp"
#include "texture.hpp"

namespace cpe
{


void draw_line(image& im,ivec2 const& p0,ivec2 const& p1,color const& c)
{
    /*************************************
    // TO DO
    /*************************************
    // Affichez une ligne sur l'image
    //
    // Completez une structure line_discrete a l'aide de
    //   l'algorithme de Bresenham entre p0 et p1.
    // Pour tous les pixel p du segment
    //   Affecter im(p) a la couleur c
    //
    */
    line_discrete line;
    bresenham(p0, p1, line);
//    for(int k=0; len())
//    std::cout<<bresenham<<std::endl;
    for (int k=0;k<line.size();++k){
        im(line[k]) = c;
    }
//    im.save("ma_ligne.ppm");
}

    void draw_line(image& im, ivec2 const& p0, ivec2 const& p1, color const& c0, color const& c1)
    {

    /*************************************
    // TO DO
    /*************************************
    // Affichez une ligne de couleur variable entre p0 de couleur c0, et p1 de couleur c1
    //
    // Completez une structure line_discrete a l'aide de
    //   l'algorithme de Bresenham entre p0 et p1.
    // Construire une structure line_interpolation_parameter
    //   a partir de la ligne discrete
    //
    // Pour toute les pixels de la ligne a l'indice k
    //    p <- position a l'indice k
    //    alpha <- parametre d'interpolation a l'indice k
    //    Calculer couleur courante encolor(1.0f, 1.0f, 0.0f) fonction de alpha, c0, et c1
    //    Appliquer la couleur courante a im(p)
    //
    */
    line_discrete lined;
    bresenham(p0,p1,lined);
    line_interpolation_parameter interpolation(lined);
    for(int k=0; k<lined.size();++k){
        ivec2 p = {lined[k].x(), lined[k].y()};
        float alpha = interpolation[k];
        color c = (1-alpha)*c0 + alpha*c1;
        im(p)= color(c);
    }
//    im.save("mon_interpolation.ppm");
}

void draw_line_depth(image& im, image_zbuffer& zbuffer,     // images
                        ivec2 const& p0,ivec2 const& p1,    // pixels
                        color const& c0,color const& c1,    // couleurs
                        float const& z0, float const& z1)   // depth
{
    line_discrete line;
    bresenham(p0,p1,line);

    line_interpolation_parameter alpha(line);

    for(int k=0;k<line.size();++k)
        {
            // std::cout<<line[k]<<" ";
            ivec2 pk = line[k];
            color ck = (1-alpha[k])*c0 + alpha[k]*c1;
            float zk = (1-alpha[k])*z0 + alpha[k]*z1;
            draw_point(im,zbuffer, pk, zk, ck);

        }
    // std::cout<<std::endl;
}

void draw_line_depth_texture(image& im, image_zbuffer& zbuffer,     // images
                        ivec2 const& p0,ivec2 const& p1,    // pixels
                        color const& c0,color const& c1,    // couleurs
                        float const& z0, float const& z1,
                        vec2 const& uv0, vec2 const& uv1, texture const& tex)   // depth
{
    line_discrete line;
    bresenham(p0,p1,line);

    line_interpolation_parameter alpha(line);

    for(int k=0;k<line.size();++k)
        {
            // std::cout<<line[k]<<" ";
            ivec2 pk = line[k];
            color ck = (1-alpha[k])*c0 + alpha[k]*c1;
            float zk = (1-alpha[k])*z0 + alpha[k]*z1;
            vec2 const uv = (1-alpha[k])*uv0+alpha[k]*uv1;
//            std::cout<<uv<<std::endl;
            draw_point_texture(im,zbuffer, pk, zk, ck,uv,tex);

        }
    // std::cout<<std::endl;
}

void draw_triangle_wireframe(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,color const& c)
{


    /*************************************
    // TO DO
    /*************************************
    // Affichez un triangle plein de couleur uniforme
    //
    // Definir une structure scanline sur (p0,p1,p2).
    //   le parametere peut etre definit arbitrairement.
    //   (par exemple triangle_scanline_factory(p0,p1,p2 , c,c,c); )
    //
    // Pour toutes les valeurs y du scanline
    //   pa <- position a gauche
    //   pb <- position a droite
    //   Afficher segment entre pa et pb de couleur c
    //
    */
//    draw_line(im,p0,p1,c);
//    draw_line(im,p1,p2,c);
//    draw_line(im,p2,p0,c);
    auto scanline = triangle_scanline_factory(p0,p1,p2 , c,c,c);
    for(auto const& value : scanline)
        {
//            int const x = value.first;
            auto const& left = value.second.left;
            auto const& right = value.second.right;

            ivec2 const& p_left = left.coordinate;
            ivec2 const& p_right = right.coordinate;

            color const& param_left = left.parameter;
            color const& param_right = right.parameter;

            // trace la ligne avec les parametres recup par le scanline
            draw_line(im, p_left, p_right, param_left, param_right);
        }
//    im.save("triangle.ppm");

}

void draw_triangle_fade(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2)
{

    /*************************************
    // TO DO
    /*************************************
    // Affichez un triangle plein de couleur uniforme
    //
    // Definir une structure scanline sur (p0,p1,p2).
    //   de parametre (c0,c1,c2)
    //
    // Pour toutes les valeurs y du scanline
    //   pa <- position a gauche
    //   pb <- position a droite
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   Afficher segment entre pa et pb
    //        avec couleur variant entre ca et cb
    //
    */
    //    draw_line(im,p0,p1,c);
    //    draw_line(im,p1,p2,c);
    //    draw_line(im,p2,p0,c);
        auto scanline = triangle_scanline_factory(p0,p1,p2 , c0,c1,c2);
        auto it = scanline.begin();
        auto it_end = scanline.end();
        for(; it!=it_end ; ++it)
            {
//                int const x = it->first;
                auto const& left = it->second.left;
                auto const& right = it->second.right;

                ivec2 const& p_left = left.coordinate;
                ivec2 const& p_right = right.coordinate;

                color const& param_left = left.parameter;
                color const& param_right = right.parameter;

                // trace la ligne avec les parametres recup par le scanline
                draw_line(im, p_left, p_right, param_left, param_right);
            }
        im.save("triangle_fondu.ppm");
}


void draw_point(image& im,image_zbuffer& zbuffer,ivec2 const& p,float const z,color const& c)
{
    if(p.x()<0 || p.x()>=im.Nx())
        return ;
    if(p.y()<0 || p.y()>=im.Ny())
        return ;

    /*************************************
    // TO DO
    /*************************************
    // Si z est compris dans [-1,1] et que z
    //   est plus faible que zbuffer(p)
    //  Alors mettre a jour la couleur et le zbuffer
    */
    if(z>=-1 && z<=1 && z <= zbuffer(p)){
        im(p) = c;
        zbuffer(p) = z;
    }
}

void draw_point_texture(image& im,image_zbuffer& zbuffer,ivec2 const& p,float const z,color const& c, vec2 const& uv, texture const& tex)
{
    if(p.x()<0 || p.x()>=im.Nx())
        return ;
    if(p.y()<0 || p.y()>=im.Ny())
        return ;

    /*************************************
    // TO DO
    /*************************************
    // Si z est compris dans [-1,1] et que z
    //   est plus faible que zbuffer(p)
    //  Alors mettre a jour la couleur et le zbuffer
    */
    if(z>=-1 && z<=1 && z <= zbuffer(p)){
        color const col_texture = tex(uv);
        im(p) = c*col_texture;
        zbuffer(p) = z;
    }
}


void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2)
{


    /*************************************
    // TO DO
    /*************************************
    // Construire scanline avec couleur
    // Construire scanline avec profondeur
    // Parcourir les deux scanline (utilisez les iterateurs)
    //   pa <- point gauche
    //   pb <- point droit
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   za <- profondeur a gauche
    //   zb <- profondeur a droite
    //
    //   affiche ligne entre pa et pb
    //     de couleur variant entre ca et cb
    //     de pronfondeur variant entre za et zb
    */
    vec3 A(1,0,0);
    vec3 B(0,1,0);
    vec3 C(0,0,1);
    auto scanline_barycentrique = triangle_scanline_factory(p0,p1,p2,A,B,C);
    auto it = scanline_barycentrique.begin();
    auto it_end = scanline_barycentrique.end();
    for(; it!=it_end; ++it){
//        int const x = it->first;
        auto const& left = it->second.left;
        auto const& right = it->second.right;

        ivec2 const& p_left = left.coordinate;
        ivec2 const& p_right = right.coordinate;

        vec3 const& coordBarycent_left = left.parameter;
        vec3 const& coordBarycent_right = right.parameter;

        color color_left = coordBarycent_left[0]*c0 + coordBarycent_left[1]*c1 + coordBarycent_left[2]*c2;
        color color_right = coordBarycent_right[0]*c0 + coordBarycent_right[1]*c1 + coordBarycent_right[2]*c2;

        float depth_left = coordBarycent_left[0]*z0 + coordBarycent_left[1]*z1 + coordBarycent_left[2]*z2;
        float depth_right = coordBarycent_right[0]*z0 + coordBarycent_right[1]*z1 + coordBarycent_right[2]*z2;

        // trace la ligne avec les parametres recup par le scanline
        draw_line_depth(im, zbuffer,                 // images
                        p_left, p_right,             // pixels
                        color_left, color_right,     // couleurs
                        depth_left, depth_right);    // depth
    }
//    im.save("two_triangles_with_depth.ppm");
}
void draw_triangle_texture(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2, vec2 const& uv0, vec2 const& uv1, vec2 const& uv2, texture const& tex)
{


    /*************************************
    // TO DO
    /*************************************
    // Construire scanline avec couleur
    // Construire scanline avec profondeur
    // Parcourir les deux scanline (utilisez les iterateurs)
    //   pa <- point gauche
    //   pb <- point droit
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   za <- profondeur a gauche
    //   zb <- profondeur a droite
    //
    //   affiche ligne entre pa et pb
    //     de couleur variant entre ca et cb
    //     de pronfondeur variant entre za et zb
    */
    vec3 A(1,0,0);
    vec3 B(0,1,0);
    vec3 C(0,0,1);
    auto scanline_barycentrique = triangle_scanline_factory(p0,p1,p2,A,B,C);
    auto scanline_uv = triangle_scanline_factory(p0,p1,p2,uv0,uv1,uv2);
    auto it = scanline_barycentrique.begin();
    auto it_end = scanline_barycentrique.end();
    auto it_uv = scanline_uv.begin();
    for(; it!=it_end; ++it, ++it_uv){
//        int const x = it->first;
        auto const& left = it->second.left;
        auto const& right = it->second.right;

        ivec2 const& p_left = left.coordinate;
        ivec2 const& p_right = right.coordinate;

        vec3 const& coordBarycent_left = left.parameter;
        vec3 const& coordBarycent_right = right.parameter;

        color color_left = coordBarycent_left[0]*c0 + coordBarycent_left[1]*c1 + coordBarycent_left[2]*c2;
        color color_right = coordBarycent_right[0]*c0 + coordBarycent_right[1]*c1 + coordBarycent_right[2]*c2;

        float depth_left = coordBarycent_left[0]*z0 + coordBarycent_left[1]*z1 + coordBarycent_left[2]*z2;
        float depth_right = coordBarycent_right[0]*z0 + coordBarycent_right[1]*z1 + coordBarycent_right[2]*z2;

        // trace la ligne avec les parametres recup par le scanline
        vec2 const& uva = it_uv->second.left.parameter;
        vec2 const& uvb = it_uv->second.right.parameter;
        draw_line_depth_texture(im, zbuffer,                 // images
                        p_left, p_right,             // pixels
                        color_left, color_right,     // couleurs
                        depth_left, depth_right,
                        uva, uvb, tex);    // depth
    }
//    im.save("two_triangles_with_depth.ppm");
}



}
