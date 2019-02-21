//
//#include "Mesh.hpp"
//#include <cassert>
//#include <iostream>
//#include "tiny_obj_loader.h"
//#include "Matrix.hpp"
//#include "View.hpp"
//
//namespace example
//{
//	
//	Mesh::Mesh(const std::string & obj_file_path, Translation3f position, Scaling3f scale, Color color) : translation(position), scaling(scale)
//	{
//		std::string inputfile = "../../assets/bunny.obj";
//		tinyobj::attrib_t attrib;
//		std::vector<tinyobj::shape_t> shapes;
//		std::vector<tinyobj::material_t> materials;
//
//		std::string warn;
//		std::string err;
//		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());
//
//		if (!ret) {
//			exit(1);
//		}
//
//		size_t number_of_vertices = attrib.vertices.size() / 3;
//
//		original_vertices.resize(number_of_vertices);
//
//		for (size_t index = 0, vertexIndex = 0; index < number_of_vertices, vertexIndex < attrib.vertices.size(); index++, vertexIndex += 3)
//		{
//			original_vertices[index] = Vertex({ attrib.vertices[vertexIndex], attrib.vertices[vertexIndex + 1],attrib.vertices[vertexIndex + 2], 1 });
//		}
//
//		transformed_vertices.resize(number_of_vertices);
//		display_vertices.resize(number_of_vertices);
//
//
//		size_t number_of_colors = attrib.colors.size() / 3;
//
//		assert(number_of_colors == number_of_vertices);
//
//		original_colors.resize(number_of_colors);
//
//		for (size_t index = 0; index < number_of_colors; index++)
//		{
//			original_colors[index].set(255, 0, 0);
//		}
//
//		size_t number_of_triangles = shapes[0].mesh.indices.size() / 3;
//
//		original_indices.resize(number_of_triangles * 3);
//
//		Index_Buffer::iterator indices_iterator = original_indices.begin();
//
//		for (size_t triangle_index = 0, vertexIndex = 0; triangle_index < number_of_triangles; triangle_index++, vertexIndex += 3)
//		{
//			*indices_iterator++ = shapes[0].mesh.indices[vertexIndex].vertex_index;
//			*indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 1].vertex_index;
//			*indices_iterator++ = shapes[0].mesh.indices[vertexIndex + 2].vertex_index;
//		}
//	}
//}