#pragma once

#ifndef MESH_HEADER
#define MESH_HEADER
#define TINYOBJLOADER_IMPLEMENTATION 


#include <string>
#include "Point.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Translation.hpp"
#include "Projection.hpp"
#include "Rotation.hpp"
#include "Scaling.hpp"
#include "Rasterizer.hpp"


namespace example
{
    using std::vector;
    using toolkit::Point4i;
    using toolkit::Point4f;
    using toolkit::Matrix44f;
    using toolkit::Matrix41f;
    using toolkit::Translation3f;
    using toolkit::Rotation3f;
    using toolkit::Scaling3f;
    using toolkit::Transformation3f;
	using toolkit::Projection3f;

    class Mesh
    {
    private:

        typedef Color_Buffer_Rgba8888 Color_Buffer;
        typedef Color_Buffer::Color   Color;
        typedef Point4f               Vertex;
        typedef vector< Vertex >      Vertex_Buffer;
        typedef vector< int    >      Index_Buffer;
        typedef vector< Color  >      Vertex_Colors;


    public:

        Mesh() {}
        Mesh(const std::string & obj_file_path, Translation3f position, Scaling3f scale, Color color);
		void update(Projection3f  projection, Transformation3f parentTransform = Transformation3f());
        void paint(Rasterizer< Color_Buffer > & rasterizer, Transformation3f parentTransform);

        Vertex_Buffer     original_vertices;
        Index_Buffer      original_indices;
        Vertex_Colors     original_colors;
        Vertex_Buffer     original_normals;
        Vertex_Buffer     transformed_vertices;
        vector< Point4i > display_vertices;
		Mesh*             parent;
		vector< std::shared_ptr <Mesh> > children;

        Translation3f        translation;
        Rotation3f           rotation_x;
        Rotation3f           rotation_y;
        Scaling3f            scaling;
        Transformation3f     transformation;
    };
}
#endif