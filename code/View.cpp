
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by √Ångel on december of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//#define TINYOBJLOADER_IMPLEMENTATION 
#include <cmath>
#include <cassert>
#include <Vector.hpp>
#include <Scaling.hpp>
#include <Rotation.hpp>
#include <Projection.hpp>
#include <Translation.hpp>
#include "View.hpp"

using namespace toolkit;

namespace example
{
    View::View(size_t width, size_t height)
    :
        width       (width ),
        height      (height),
        Color_buffer(width, height),
        rasterizer  (Color_buffer)
    {
		//      // Se definen los datos de los vertices:
		//std::string inputfile = "../../assets/bunny.obj";
		//tinyobj::attrib_t attrib;
		//std::vector<tinyobj::shape_t> shapes;
		//std::vector<tinyobj::material_t> materials;

		//std::string warn;
		//std::string err;
		//bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

		//if (!ret) 
		//{
		//	exit(1);
		//}

		//size_t number_of_vertices = attrib.vertices.size() / 3;

		//original_vertices.resize(number_of_vertices);

		//for (size_t index = 0, vertexIndex = 0; index < number_of_vertices, vertexIndex < attrib.vertices.size(); index++, vertexIndex += 3)
		//{
		//	original_vertices[index] = Vertex({ attrib.vertices[vertexIndex], attrib.vertices[vertexIndex + 1],attrib.vertices[vertexIndex + 2], 1 });
		//}

		//transformed_vertices.resize(number_of_vertices);
		//display_vertices.resize(number_of_vertices);


		//size_t number_of_colors = attrib.colors.size() / 3;

		//assert(number_of_colors == number_of_vertices);

		//original_colors.resize(number_of_colors);

		//for (size_t index = 0; index < number_of_colors; index++)
		//{
		//	original_colors[index].set(255, 0, 0);
		//}

		//size_t number_of_triangles = shapes[0].mesh.indices.size() / 3;

		//original_indices.resize(number_of_triangles * 3);

		//Index_Buffer::iterator indices_iterator = original_indices.begin();

		//for (size_t triangle_index = 0, vertexIndex = 0; triangle_index < number_of_triangles; triangle_index++, vertexIndex += 3)
		//{
		//	*indices_iterator++ = shapes[0].mesh.indices[vertexIndex].vertex_index;
		//	*indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 1].vertex_index;
		//	*indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 2].vertex_index;
		//}
    }

    void View::update ()
    {  
       /* static float angle = 0.f;

        angle += 0.025f;

        Scaling3f     scaling     (0.2f);
        Rotation3f    rotation_x;
        Rotation3f    rotation_y;
        Translation3f translation (0, 0, -10);
        Projection3f  projection  (5, 15, 20, float(width) / float(height));

        rotation_x.set< Rotation3f::AROUND_THE_X_AXIS > (0.50f);
        rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS > (angle);

        // Creacion de la matriz de transformacion unificada:

        Transformation3f transformation = projection * translation * rotation_x * rotation_y * scaling;*/

       /* // Se transforman todos los vertices usando la matriz de transformacion resultante:

        for (size_t index = 0, number_of_vertices = original_vertices.size (); index < number_of_vertices; index++)
        {
            // Se multiplican todos los vertices originales con la matriz de transformacion y
            // se guarda el resultado en otro vertex buffer:

            Vertex & vertex = transformed_vertices[index] = Matrix44f(transformation) * Matrix41f(original_vertices[index]);

            // La matriz de proyeccion en perspectiva hace que el ultimo componente del vector
            // transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:

            float divisor = 1.f / vertex[3];

            vertex[0] *= divisor;
            vertex[1] *= divisor;
            vertex[2] *= divisor;
            vertex[3]  = 1.f;
        }*/
    }

    void View::paint ()
    {
        // Se convierten las coordenadas transformadas y proyectadas a coordenadas
        // de recorte (-1 a +1) en coordenadas de pantalla con el origen centrado.
        // La coordenada Z se escala a un valor suficientemente grande dentro del
        // rango de int (que es lo que espera fill_convex_polygon_z_buffer).

        /*Scaling3f        scaling        = Scaling3f    (float(width / 2), float(height / 2), 100000000.f);
        Translation3f    translation    = Translation3f(float(width / 2), float(height / 2), 0.f);
        Transformation3f transformation = translation * scaling;

        for (size_t index = 0, number_of_vertices = transformed_vertices.size (); index < number_of_vertices; index++)
        {
            display_vertices[index] = Point4i( Matrix44f(transformation) * Matrix41f(transformed_vertices[index]) );
        }

        // Se borra el framebuffer y se dibujan los triangulos:

        rasterizer.clear ();

        for (int * indices = original_indices.data (), * end = indices + original_indices.size (); indices < end; indices += 3)
        {
            if (is_frontface (transformed_vertices.data (), indices))
            {
                // Se establece el color del poli≠gono a partir del color de su primer vertice:

                rasterizer.set_color (original_colors[*indices]);

                // Se rellena el poli≠gono:

                rasterizer.fill_convex_polygon_z_buffer (display_vertices.data (), indices, indices + 3);
            }
        }

        // Se copia el framebuffer oculto en el framebuffer de la ventana:

        rasterizer.get_color_buffer ().gl_draw_pixels (0, 0);*/
    }

    bool View::is_frontface (const Vertex * const projected_vertices, const int * const indices)
    {
        const Vertex & v0 = projected_vertices[indices[0]];
        const Vertex & v1 = projected_vertices[indices[1]];
        const Vertex & v2 = projected_vertices[indices[2]];

		// Se asumen coordenadas proyectadas y polÌgonos definidos en sentido horario.
		// Se comprueba a quÈ lado de la lÌnea que pasa por v0 y v1 queda el punto v2:

        return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
    }
}
