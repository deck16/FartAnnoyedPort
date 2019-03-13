/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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
#include "Graphics.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>
#include <cstring>
#include <iostream>

#undef _CRT_WIDE_
#undef _CRT_WIDE
#define _CRT_WIDE_(s) L ## s
#define _CRT_WIDE(s) _CRT_WIDE_(s)

#define CHILI_GFX_EXCEPTION( note ) Graphics::Exception( note,_CRT_WIDE(__FILE__),__LINE__ )

using aligned_malloc_type = void*(*)(std::size_t, std::size_t);
using aligned_free_type = void(*)(void*);
#ifdef _MSC_VER
    constexpr aligned_malloc_type aligned_malloc_independent = _aligned_malloc;
    constexpr aligned_free_type aligned_free_independent = _aligned_free;
#else
    constexpr aligned_malloc_type aligned_malloc_independent = &aligned_alloc;
    constexpr aligned_free_type aligned_free_independent = &free;
#endif

Graphics::Graphics(MainWindow& win)
    :
    win(win)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw CHILI_GFX_EXCEPTION(L"GladGLLoader failed");
    }
    
    // allocate memory for sysbuffer (16-byte aligned for faster access)
    pSysBuffer = reinterpret_cast<Color*>(
        aligned_malloc_independent(sizeof(Color) * Graphics::ScreenWidth * Graphics::ScreenHeight, 16u));

    glGenVertexArrays(1, &VAO);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glGenVertexArrays failed");
    }
    glBindVertexArray(VAO);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glBindVertexArray failed");
    }

    glGenBuffers(1, &VBO);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glGenBuffers failed");
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glBindBuffer failed");
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glBufferData failed");
    }

    glGenBuffers(1, &EBO);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glGenBuffers failed");
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glBindBuffer failed");
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glBufferData failed");
    }

    glGenTextures(1, &texture);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glGenTextures failed");
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glBindTexture failed");
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pSysBuffer);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glTexImage2D failed");
    }

    shader_program = CompileShaders();
    glUseProgram(shader_program);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glUseProgram failed");
    }

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr); // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float))); // texcoords
    glEnableVertexAttribArray(0);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glEnableVertexAttribArray failed");
    }
    glEnableVertexAttribArray(1);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glEnableVertexAttribArray failed");
    }
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( pSysBuffer )
	{
		aligned_free_independent( pSysBuffer );
		pSysBuffer = nullptr;
	}
}

void Graphics::EndFrame()
{    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenWidth, ScreenHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, pSysBuffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    win.SwapBuffers();
}

void Graphics::BeginFrame()
{
	std::memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}

unsigned int Graphics::CompileShaders()
{
    static constexpr const char* vertex_shader_src = R"shader(#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoord = aTexCoord;
})shader";
    static constexpr const char* fragment_shader_src = R"shader(#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
})shader";

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glCreateShader of vertex shader failed");
    }
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glShaderSource of vertex shader failed");
    }
    glCompileShader(vertex_shader);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glCompileShader of vertex shader failed");
    }
    int  success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cerr << infoLog;
        throw CHILI_GFX_EXCEPTION(L"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glCreateShader of fragment shader failed");
    }
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glShaderSource of fragment shader failed");
    }
    glCompileShader(fragment_shader);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glCompileShader of fragment shader failed");
    }
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cerr << infoLog;
        throw CHILI_GFX_EXCEPTION(L"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }

    unsigned int shader_program = glCreateProgram();
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glCreateProgram failed");
    }
    glAttachShader(shader_program, vertex_shader);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glAttachShader of vertex shader failed");
    }
    glAttachShader(shader_program, fragment_shader);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glAttachShader of fragment shader failed");
    }
    glLinkProgram(shader_program);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glLinkProgram failed");
    }
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cerr << infoLog;
        throw CHILI_GFX_EXCEPTION(L"ERROR::PROGRAM::LINK_FAILED\n");
    }

    glDeleteShader(vertex_shader);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glDeleteShader of vertex shader failed");
    }
    glDeleteShader(fragment_shader);
    if (glGetError() != GL_NO_ERROR)
    {
        throw CHILI_GFX_EXCEPTION(L"glDeleteShader of fragment shader failed");
    }

    return shader_program;
}

//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
    // TODO: implement or remove
	//return DXGetErrorString( hr );
    return {};
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
    // TODO: implement or remove
    return {};
	//std::array<wchar_t,512> wideDescription;
	//DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	//return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}