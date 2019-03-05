
#include "Mesh.hpp"
#include <cassert>
#include <iostream>
#include <math.h> 
#include <algorithm>
#include "Matrix.hpp"
#include "View.hpp"
#include "tiny_obj_loader.h"

namespace example
{
    
    Mesh::Mesh(const std::string & obj_file_path, Translation3f position, Scaling3f scale, Color color) : translation(position), scaling(scale)
    {
        tinyobj::attrib_t			     attrib;
        std::vector<tinyobj::shape_t>    shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_file_path.c_str());

        if (!ret) 
		{
            exit(1);
        }



        size_t number_of_vertices = attrib.vertices.size() / 3;  // Guarda el número de vértices en la variable number_of_vertices
        original_vertices.resize(number_of_vertices);            // Hace un resize teniendo en cuenta la variable number_of_vertices
		original_normals.resize(number_of_vertices);             // Hace un resize teniendo en cuenta la variable number_of_vertices

		// La variable original_vertices guarda la posición de cada vértice en vectores de 4 posiciones (x, y, z, w)
        for (size_t index = 0, vertexIndex = 0; index < number_of_vertices; index++, vertexIndex += 3)
        {
            original_vertices[index] = Vertex({ attrib.vertices[vertexIndex], attrib.vertices[vertexIndex + 1],attrib.vertices[vertexIndex + 2], 1 });
			original_normals[index] = Vertex({ attrib.normals[vertexIndex], attrib.normals[vertexIndex + 1], attrib.normals[vertexIndex + 2], 0 });
        }

        transformed_vertices.resize(number_of_vertices); // Hace un resize teniendo en cuenta la variable number_of_vertices
        display_vertices.resize(number_of_vertices);     // Hace un resize teniendo en cuenta la variable number_of_vertices

		

        size_t number_of_colors = attrib.colors.size() / 3; // Guarda el número de colores en la variable number_of_colors
        assert(number_of_colors == number_of_vertices);     // Compruba si la variable number_of_colors es igual a la variable number_of_vertices
        original_colors.resize(number_of_colors);           // Hace un resize teniendo en cuenta la variable number_of_vertices
		Vertex ligth = Vertex({ 1,0,0 });
		// La variable original_colors guarda el color de cada vértice en vectores de 3 posiciones (r, g, b)
        for (size_t index = 0; index < number_of_colors; index++)
        {
			float normalVectorModule = sqrt(pow(original_normals[index][0], 2) + pow(original_normals[index][1], 2) + pow(original_normals[index][2], 2));
			Vertex normalNormalized = Vertex({ original_normals[index][0] / normalVectorModule, original_normals[index][1] / normalVectorModule, original_normals[index][2] / normalVectorModule });
			float dotProduct = ligth[0] * normalNormalized[0] + ligth[1] * normalNormalized[1] + ligth[2] * normalNormalized[2];
			float intensity = std::max(dotProduct, 0.f);
            original_colors[index].set(255 * intensity, 0, 0);
        }



        size_t number_of_triangles = shapes[0].mesh.indices.size() / 3;     // Guarda el número de triángulos en la variable number_of_triangles
        original_indices.resize(number_of_triangles * 3);                   // Hace un resize teniendo en cuenta la variable number_of_triangles multiplicada por 3
        Index_Buffer::iterator indices_iterator = original_indices.begin(); // Crea un objeto de tipo iterator y lo iguala a la variable original_indices 

		// La variable indices_iterator guarda el indice de cada vértice de cada triángulo en un int
        for (size_t triangle_index = 0, vertexIndex = 0; triangle_index < number_of_triangles; triangle_index++, vertexIndex += 3)
        {
            *indices_iterator++ = shapes[0].mesh.indices[vertexIndex].vertex_index;
            *indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 1].vertex_index;
            *indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 2].vertex_index;
        }
    }

	void Mesh::update()
	{

		// Se transforman todos los vertices usando la matriz de transformacion resultante:
		for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
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
			vertex[3] = 1.f;
		}
	}

	bool drawn = false;

	void Mesh::paint(Rasterizer<Color_Buffer>& rasterizer)
	{
		for (int * indices = original_indices.data(), *end = indices + original_indices.size(); indices < end; indices += 3)
		{
			if (View::is_frontface(transformed_vertices.data(), indices))
			{
				rasterizer.set_color(original_colors[*indices]);

				// Se rellena el polígono:
				rasterizer.fill_convex_polygon_z_buffer(display_vertices.data(), indices, indices + 3);
			}
		}
		drawn = true;
	}
}