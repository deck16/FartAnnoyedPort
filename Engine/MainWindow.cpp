/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	MainWindow.cpp																		  *
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
#include "MainWindow.h"
#include "Resource.h"
#include "Graphics.h"
#include "ChiliException.h"
#include "Game.h"
#include <assert.h>
#include <iostream>

MainWindow::MainWindow()
    :
    window{}
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = window_ptr{ glfwCreateWindow(Graphics::ScreenWidth, Graphics::ScreenHeight, "Chili Framework", nullptr, nullptr) };
    glfwSetWindowUserPointer(window.get(), this); // used for GLFWwindow* to MainWindow* lookback in callbacks
    glfwMakeContextCurrent(window.get());
    glfwSetKeyCallback(window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // TODO: maybe temporary
            glfwSetWindowShouldClose(window, GL_TRUE);

        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS)
        {
            mw->kbd.OnKeyPressed(key);
        }
        else if (action == GLFW_RELEASE)
        {
            mw->kbd.OnKeyReleased(key);
        }
        else if (action == GLFW_REPEAT)
        {
            if (mw->kbd.AutorepeatIsEnabled())
                mw->kbd.OnKeyPressed(key);
        }
        // else don't handle GLFW_KEY_UNKNOWN 
    });
    glfwSetWindowFocusCallback(window.get(), [](GLFWwindow* window, int focused)
    {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
        if (!focused)
        {
            // The window lost input focus
            mw->kbd.ClearState();
        }
    });
    glfwSetCharCallback(window.get(), [](GLFWwindow* window, std::uint32_t codepoint)
    {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
        mw->kbd.OnChar(codepoint);
    });
    glfwSetCursorPosCallback(window.get(), [](GLFWwindow* window, double xpos, double ypos)
    {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
        mw->mouse.OnMouseMove((int)xpos, (int)ypos);
        // std::cout << xpos << ":" << ypos << '\n'; // TODO: remove
    });
    glfwSetCursorEnterCallback(window.get(), [](GLFWwindow* window, int entered)
    {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
        if (entered)
        {
            mw->mouse.OnMouseEnter();
        }
        else
        {
            // The cursor left the client area of the window
            auto left_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            auto right_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            short xposi = (short)xpos, yposi = (short)ypos;
            if (left_state == GLFW_PRESS || right_state == GLFW_PRESS)
            {                
                xposi = std::clamp((unsigned int)xposi, 0u, Graphics::ScreenWidth - 1);
                yposi = std::clamp((unsigned int)yposi, 0u, Graphics::ScreenHeight - 1);
                mw->mouse.OnMouseMove(xposi, yposi);
            }
            else
            {
                mw->mouse.OnMouseLeave();
                mw->mouse.OnLeftReleased(xposi, yposi);
                mw->mouse.OnRightReleased(xposi, yposi);
            }
        }
    });
    glfwSetMouseButtonCallback(window.get(), [](GLFWwindow* window, int button, int action, int mods)
    {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (action == GLFW_PRESS)
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT)
            {
                mw->mouse.OnLeftPressed((int)xpos, (int)ypos);
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            {
                mw->mouse.OnRightPressed((int)xpos, (int)ypos);
            }
        }
        else if (action == GLFW_RELEASE)
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT)
            {
                mw->mouse.OnLeftReleased((int)xpos, (int)ypos);
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            {
                mw->mouse.OnRightReleased((int)xpos, (int)ypos);
            }
        }
    });
    glfwSetScrollCallback(window.get(), [](GLFWwindow* window, double xoffset, double yoffset)
    {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (yoffset > 0.f) // TODO: check directions
        {
            mw->mouse.OnWheelDown((int)xpos, (int)ypos);
        }
        else if (yoffset < 0.f)
        {
            mw->mouse.OnWheelUp((int)xpos, (int)ypos);
        }
    });
}

MainWindow::~MainWindow()
{
    glfwTerminate();
}

bool MainWindow::IsActive() const
{
    return glfwGetWindowAttrib(window.get(), GLFW_FOCUSED);
}

bool MainWindow::IsMinimized() const
{
    return glfwGetWindowAttrib(window.get(), GLFW_ICONIFIED);
}

void MainWindow::ShowMessageBox( const std::wstring& title,const std::wstring& message,UINT type ) const
{
	std::cout << message.c_str() << title.c_str() << type;
}

bool MainWindow::ProcessMessage()
{
    glfwPollEvents();
	return !glfwWindowShouldClose(window.get());
}
void MainWindow::SwapBuffers()
{
    glfwSwapBuffers(window.get());
}
