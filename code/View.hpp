
/**
* @file View.hpp
* @author Victor Mas Toledo
* @date 20/02/2019
* @class View
* @brief Clase en donde se llama a los métodos de update y paint y al constructor de los padres "absolutos" para que se creen
*/

#pragma once

#ifndef VIEW_HEADER
#define VIEW_HEADER

#include <vector>
#include "Point.hpp"
#include "Rasterizer.hpp"
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Mesh.hpp"

namespace example
{
	using std::vector;
	using toolkit::Point4f;

	class View
	{
	private:

		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Point4f               Vertex;

	private:

		/**
		* @brief Anchura de la escena
		*/	
		size_t width;

		/**
		* @brief Altura de la escena
		*/	
		size_t height;

		/**
		* @brief Contine los colores de los vértices
		*/	
		Color_Buffer                       Color_buffer;

		/**
		* @brief Contine los colores del rasterizador
		*/	
		Rasterizer< Color_Buffer >         rasterizer;

		/**
		* @brief Vector de todas las mehes de la escena
		*/	
		vector< std::shared_ptr <Mesh> >   meshes;

	public:

		/**
		* @brief Crea la escena
		* @param width -> la anchura de la escena
		* @param height -> la altura de la escena
		*/		
		View(size_t width, size_t height);

		/**
		* @brief Actualiza la rotación de la Mesh y la cámara y llama al update de la/s mesh/es padre/s
		*/
		void update();

		/**
		* @brief Llama al método paint de la/s mesh/es padre/s
		*/
		void paint();

		/**
		* @brief Crea modulos
		* @param projected_vertices -> los vertices proyectados de los triángulos de la mesh
		* @param indices -> los indices de los triángulos de la mesh
		*/
		static bool is_frontface(const Vertex * const projected_vertices, const int * const indices);
	};
}
#endif
