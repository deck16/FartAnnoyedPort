/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	MainWindow.h																		  *
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
#include "Keyboard.h"
#include "Mouse.h"
#include "ChiliException.h"
#include <string>
#include <memory>

class MainWindow
{
public:
	class Exception : public ChiliException
	{
	public:
		using ChiliException::ChiliException;
		virtual std::wstring GetFullMessage() const override { return GetNote() + L"\nAt: " + GetLocation(); }
		virtual std::wstring GetExceptionType() const override { return L"Windows Exception"; }
	};
public:
	MainWindow();
	MainWindow( const MainWindow& ) = delete;
	MainWindow& operator=( const MainWindow& ) = delete;
	~MainWindow();
	bool IsActive() const;
	bool IsMinimized() const;
	void ShowMessageBox( const std::wstring& title,const std::wstring& message ) const;
	void Kill()
	{
		glfwSetWindowShouldClose(window.get(), GLFW_TRUE);
	}
	// returns false if quitting
	bool ProcessMessage();
    void SwapBuffers();
//private:
//	static LRESULT WINAPI _HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam );
//	static LRESULT WINAPI _HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam );
//	LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam );
public:
	Keyboard kbd;
	Mouse mouse;
private:
    struct glfw_window_destroyer
    {
        void operator()(GLFWwindow* window)
        {
            glfwDestroyWindow(window);
        }
    };
    using window_ptr = std::unique_ptr<GLFWwindow, glfw_window_destroyer>;
    window_ptr window;
};