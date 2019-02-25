
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by √Ångel on december of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "View.hpp"
#include <iostream>
#include "Projection.hpp"
#include "Translation.hpp"

//using namespace toolkit;

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
		meshes.push_back({ "../../assets/bunny.obj", {1.8f,1,-10 }, 1,{ 0,120,0 } });
		//meshes.push_back({ "../../assets/sphere.obj",{ 1,0,-10 }, 1 });
		//meshes.push_back({ "../../assets/bunny.obj",{ 1,0,-10 }, 1, {120,0,0} });
		//meshes.push_back({ "../../assets/terrain.obj",{ 1,0,-10 }, 1, {120,0,0} });
    }

    void View::update ()
    {  
		// Se actualizan los par·metros de transformatiÛn (sÛlo se modifica el ·ngulo):
		static float angle = 0.f;
		angle += 0.025f;

		// Camera
		Projection3f  projection(5, 15, 20, float(width) / float(height));

		for (auto mesh = meshes.begin(); mesh != meshes.end(); ++mesh)
		{
			// Setting rotation
			//mesh->rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(0.50f);
			//mesh->rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(angle);

			// CreaciÛn de la matriz de transformaciÛn unificada:
			mesh->transformation = projection * mesh->translation * mesh->rotation_x * mesh->rotation_y * mesh->scaling;

			// Update mesh
			mesh->update();
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

		// Se borra el frameb˙ffer y se dibujan los tri·ngulos:
		rasterizer.clear();

		for (auto mesh = meshes.begin(); mesh != meshes.end(); ++mesh)
		{
			for (size_t index = 0, number_of_vertices = mesh->transformed_vertices.size(); index < number_of_vertices; index++)
			{
				mesh->display_vertices[index] = Point4i(Matrix44f(transformation) * Matrix41f(mesh->transformed_vertices[index]));
			}

			mesh->paint(rasterizer);
		}

        // Se copia el framebuffer oculto en el framebuffer de la ventana:

        rasterizer.get_color_buffer ().gl_draw_pixels (0, 0);
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
