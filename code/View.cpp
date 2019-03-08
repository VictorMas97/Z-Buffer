
/**
* @file View.cpp
* @author Victor Mas Toledo
* @date 20/02/2019
* @class View
* @brief Clase en donde se llama a los métodos de update y paint y al constructor de los padres "absolutos" para que se creen
*/

#include "View.hpp"
#include <iostream>
#include "Translation.hpp"

namespace example
{
	using namespace toolkit;

    View::View(size_t width, size_t height)
    :
        width       (width ),
        height      (height),
        Color_buffer(width, height),
        rasterizer  (Color_buffer)
    {
		Mesh object1 { "../../assets/tree1.obj",{-1.f,0.f,-10.f }, 0.5,{ 0,120,0 } };
		Mesh children1 { "../../assets/tree1.obj", {-1.f,0.f,0.f }, 0.05,{ 0,0,120 } };
		object1.children.push_back(std::make_shared <Mesh>(children1));
		meshes.push_back(std::make_shared <Mesh>(object1));
    }

    void View::update ()
    {  
		// Se actualizan los parámetros de transformatión (sólo se modifica el ángulo):
		static float angle = 0.f;
		angle += 0.025f;

		// Camera
		Projection3f  projection(5, 15, 20, float(width) / float(height));

		for (auto &mesh : meshes)
		{
			// Setting rotation
			//mesh->rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(0.50f);
			mesh->rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(angle);

			// Update mesh
			mesh->update(projection);
		}
    }

    void View::paint ()
    {
        // Se convierten las coordenadas transformadas y proyectadas a coordenadas
        // de recorte (-1 a +1) en coordenadas de pantalla con el origen centrado.
        // La coordenada Z se escala a un valor suficientemente grande dentro del
        // rango de int (que es lo que espera fill_convex_polygon_z_buffer).

        Scaling3f        scaling        = Scaling3f    (float(width / 2), float(height / 2), 100000000.f);
        Translation3f    translation    = Translation3f(float(width / 2), float(height / 2), 0.f);
        Transformation3f transformation = translation * scaling;

		// Se borra el framebúffer y se dibujan los triángulos:
		rasterizer.clear();

		for (auto &mesh : meshes)
		{
			mesh->paint(rasterizer, transformation);
		}

        // Se copia el framebuffer oculto en el framebuffer de la ventana:

        rasterizer.get_color_buffer ().gl_draw_pixels (0, 0);
    }

    bool View::is_frontface (const Vertex * const projected_vertices, const int * const indices)
    {
        const Vertex & v0 = projected_vertices[indices[0]];
        const Vertex & v1 = projected_vertices[indices[1]];
        const Vertex & v2 = projected_vertices[indices[2]];

		// Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
		// Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

        return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
    }
}
