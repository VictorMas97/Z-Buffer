//#define TINYOBJLOADER_IMPLEMENTATION 
#include "Mesh.hpp"
#include <cassert>
#include <iostream>
#include "Matrix.hpp"
#include "View.hpp"

namespace example
{
	
	Mesh::Mesh(const std::string & obj_file_path, Translation3f position, Scaling3f scale, Color color) : translation(position), scaling(scale)
	{
		std::string inputfile = "../../assets/bunny.obj";
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

		if (!ret) 
		{
			exit(1);
		}

		size_t number_of_vertices = attrib.vertices.size() / 3;

		original_vertices.resize(number_of_vertices);

		for (size_t index = 0, vertexIndex = 0; index < number_of_vertices, vertexIndex < attrib.vertices.size(); index++, vertexIndex += 3)
		{
			original_vertices[index] = Vertex({ attrib.vertices[vertexIndex], attrib.vertices[vertexIndex + 1],attrib.vertices[vertexIndex + 2], 1 });
		}

		transformed_vertices.resize(number_of_vertices);
		display_vertices.resize(number_of_vertices);


		size_t number_of_colors = attrib.colors.size() / 3;

		assert(number_of_colors == number_of_vertices);

		original_colors.resize(number_of_colors);

		for (size_t index = 0; index < number_of_colors; index++)
		{
			original_colors[index].set(255, 0, 0);
		}

		size_t number_of_triangles = shapes[0].mesh.indices.size() / 3;

		original_indices.resize(number_of_triangles * 3);

		Index_Buffer::iterator indices_iterator = original_indices.begin();

		for (size_t triangle_index = 0, vertexIndex = 0; triangle_index < number_of_triangles; triangle_index++, vertexIndex += 3)
		{
			*indices_iterator++ = shapes[0].mesh.indices[vertexIndex].vertex_index;
			*indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 1].vertex_index;
			*indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 2].vertex_index;
		}
	}

	void Mesh::update()
	{
		/*float angle = 0.f;

		angle += 0.025f;

		rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(0.50f);
		rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(0.1f);

		 //Creación de la matriz de transformación unificada:
		transformation = projection * translation * rotation_x * rotation_y * scaling;*/

		//Se transforman todos los vértices usando la matriz de transformación resultante:

		for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
		{
			// Se multiplican todos los vértices originales con la matriz de transformación y
			// se guarda el resultado en otro vertex buffer:

			Vertex & vertex = transformed_vertices[index] = Matrix44f(transformation) * Matrix41f(original_vertices[index]);

			// La matriz de proyección en perspectiva hace que el último componente del vector
			// transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:

			float divisor = 1.f / vertex[3];

			vertex[0] *= divisor;
			vertex[1] *= divisor;
			vertex[2] *= divisor;
			vertex[3] = 1.f;
		}
	}

	bool drawn = false;

	void Mesh::paint(Rasterizer< Color_Buffer > & rasterizer)
	{
		for (int * indices = original_indices.data(), *end = indices + original_indices.size(); indices < end; indices += 3)
		{
			if (View::is_frontface(transformed_vertices.data(), indices))
			{
				// Se establece el color del poli­gono a partir del color de su primer vertice:

				rasterizer.set_color(original_colors[*indices]);

				// Se rellena el poli­gono:

				rasterizer.fill_convex_polygon_z_buffer(display_vertices.data(), indices, indices + 3);
			}
		}
		drawn = true;
	}
}