/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MainWindow.h"
#include "ChiliException.h"
#include "Colors.h"

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
    // vertex format for the framebuffer fullscreen textured quad
    struct Vertex
    {
        float x, y;		// position
        float u, v;		// texcoords
    };
public:
	Graphics(MainWindow& win);
	Graphics(const Graphics&) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	~Graphics();
private:
    MainWindow& win;
	Color* pSysBuffer = nullptr;
    unsigned int shader_program = 0;
    static constexpr Vertex vertices[] =
    {
        // vertices       // texcoords, use top-left as 0,0 instead of bottom left
        {-1.0f, 1.0f,      0.0f, 0.0f},
        {-1.0f,-1.0f,      0.0f, 1.0f},
        {1.0f,  1.0f,      1.0f, 0.0f},
        {1.0f, -1.0f,      1.0f, 1.0f}
    };
    static constexpr unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    unsigned int CompileShaders();
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int texture;
public:
	static constexpr unsigned int ScreenWidth = 800u;
	static constexpr unsigned int ScreenHeight = 600u;
};