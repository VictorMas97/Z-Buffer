
/**
* @file Mesh.hpp
* @author Victor Mas Toledo
* @date 25/02/2019
* @class Mesh
* @brief Clase en donde se crean, renderizan y pintan todas las meshes
*/

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

		/**
		* @brief Contructor por defecto de la mesh
		*/	
        Mesh() {}

		/**
		* @brief Contructor alternativo de la mesh
		* @param obj_file_path -> ruta del archivo de la mesh
		* @param position -> posición de la mesh
		* @param scale -> escala de la mesh
		* @param color -> color de la mesh
		*/	
        Mesh(const std::string & obj_file_path, Translation3f position, Scaling3f scale, Color color);

		/**
		* @brief Se actualizan todas las meshes, padres e hijas
		* @param projection -> la proyección de la cámara
		* @param parentTransform -> transform del padre, crea uno por defecto si no existe un padre
		*/	
		void update(Projection3f  projection, Transformation3f parentTransform = Transformation3f());

		/**
		* @brief Se pintan en la escena todas las meshes, padres e hijas
		* @param rasterizer -> Referencia de los colores del rasterizador
		* @param parentTransform -> transform del padre
		*/	
        void paint(Rasterizer< Color_Buffer > & rasterizer, Transformation3f parentTransform);

		/**
		* @brief Vertices originales de la mesh
		*/	
        Vertex_Buffer     original_vertices;

		/**
		* @brief Indices originales de cada vertice de la mesh
		*/	
        Index_Buffer      original_indices;

		/**
		* @brief Colores originales de cada triángulo de la mesh
		*/	
        Vertex_Colors     original_colors;

		/**
		* @brief Normales originales de cada triángulo de la mesh
		*/	
        Vertex_Buffer     original_normals;

		/**
		* @brief Vertices transformados de la mesh
		*/	
        Vertex_Buffer     transformed_vertices;

		/**
		* @brief Vertices mostrador por pantalla de la mesh
		*/	
        vector< Point4i > display_vertices;

		/**
		* @brief Referencia al padre de la mesh (No siempre hay un padre)
		*/	
		Mesh*             parent;

		/**
		* @brief Referencia a las meshes hijas (No siempre hijos)
		*/	
		vector< std::shared_ptr <Mesh> > children;



		/**
		* @brief Translación de la mesh
		*/	
        Translation3f        translation;

		/**
		* @brief Rotación en el eje X de la mesh
		*/	
        Rotation3f           rotation_x;

		/**
		* @brief Rotación en el eje Y de la mesh
		*/	
        Rotation3f           rotation_y;

		/**
		* @brief Escala de la mesh
		*/	
        Scaling3f            scaling;

		/**
		* @brief Transform de la mesh
		*/	
        Transformation3f     transformation;
    };
}
#endif