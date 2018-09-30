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

#define CHILI_GFX_EXCEPTION( note ) Graphics::Exception( note,_CRT_WIDE(__FILE__),__LINE__ )

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
        _aligned_malloc(sizeof(Color) * Graphics::ScreenWidth * Graphics::ScreenHeight, 16u));

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pSysBuffer);

    shader_program = CompileShaders();
    glUseProgram(shader_program);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr); // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float))); // texcoords
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( pSysBuffer )
	{
		_aligned_free( pSysBuffer );
		pSysBuffer = nullptr;
	}
}

void Graphics::EndFrame()
{
    //glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSysBuffer);
    //glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    win.SwapBuffers();
}

void Graphics::BeginFrame()
{
	// clear the sysbuffer
    // TODO: temporary
    //glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
	memset( pSysBuffer,155u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
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
    static constexpr char* vertex_shader_src = R"shader(#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoord = aTexCoord;
})shader";
    static constexpr char* fragment_shader_src = R"shader(#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
})shader";

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        CHILI_GFX_EXCEPTION(L"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        CHILI_GFX_EXCEPTION(L"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        CHILI_GFX_EXCEPTION(L"ERROR::PROGRAM::LINK_FAILED\n");
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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