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
    glfwMakeContextCurrent(window.get());
    glfwSetKeyCallback(window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    });
}

MainWindow::~MainWindow()
{
    glfwTerminate();
}

bool MainWindow::IsActive() const
{
	// TODO: implement, consider removing
    return true;
}

bool MainWindow::IsMinimized() const
{
    // TODO: implement, consider removing
    return false;
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
//
//LRESULT WINAPI MainWindow::_HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
//{
//	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
//	if( msg == WM_NCCREATE )
//	{
//		// extract ptr to window class from creation data
//		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
//		MainWindow* const pWnd = reinterpret_cast<MainWindow*>( pCreate->lpCreateParams );
//		// sanity check
//		assert( pWnd != nullptr );
//		// set WinAPI-managed user data to store ptr to window class
//		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>( pWnd ) );
//		// set message proc to normal (non-setup) handler now that setup is finished
//		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>( &MainWindow::_HandleMsgThunk ) );
//		// forward message to window class handler
//		return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
//	}
//	// if we get a message before the WM_NCCREATE message, handle with default handler
//	return DefWindowProc( hWnd,msg,wParam,lParam );
//}
//
//LRESULT WINAPI MainWindow::_HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
//{
//	// retrieve ptr to window class
//	MainWindow* const pWnd = reinterpret_cast<MainWindow*>( GetWindowLongPtr( hWnd,GWLP_USERDATA ) );
//	// forward message to window class handler
//	return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
//}
//
//LRESULT MainWindow::HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
//{
//	switch( msg )
//	{
//	case WM_DESTROY:
//		PostQuitMessage( 0 );
//		break;
//	case WM_KILLFOCUS:
//		kbd.ClearState();
//		break;
//
//		// ************ KEYBOARD MESSAGES ************ //
//	case WM_KEYDOWN:
//		if( !(lParam & 0x40000000) || kbd.AutorepeatIsEnabled() ) // no thank you on the autorepeat
//		{
//			kbd.OnKeyPressed( static_cast<unsigned char>(wParam) );
//		}
//		break;
//	case WM_KEYUP:
//		kbd.OnKeyReleased( static_cast<unsigned char>(wParam) );
//		break;
//	case WM_CHAR:
//		kbd.OnChar( static_cast<unsigned char>(wParam) );
//		break;
//		// ************ END KEYBOARD MESSAGES ************ //
//
//		// ************ MOUSE MESSAGES ************ //
//	case WM_MOUSEMOVE:
//	{
//		POINTS pt = MAKEPOINTS( lParam );
//		if( pt.x > 0 && pt.x < Graphics::ScreenWidth && pt.y > 0 && pt.y < Graphics::ScreenHeight )
//		{
//			mouse.OnMouseMove( pt.x,pt.y );
//			if( !mouse.IsInWindow() )
//			{
//				SetCapture( hWnd );
//				mouse.OnMouseEnter();
//			}
//		}
//		else
//		{
//			if( wParam & (MK_LBUTTON | MK_RBUTTON) )
//			{
//				pt.x = std::max( short( 0 ),pt.x );
//				pt.x = std::min( short( Graphics::ScreenWidth - 1 ),pt.x );
//				pt.y = std::max( short( 0 ),pt.y );
//				pt.y = std::min( short( Graphics::ScreenHeight - 1 ),pt.y );
//				mouse.OnMouseMove( pt.x,pt.y );
//			}
//			else
//			{
//				ReleaseCapture();
//				mouse.OnMouseLeave();
//				mouse.OnLeftReleased( pt.x,pt.y );
//				mouse.OnRightReleased( pt.x,pt.y );
//			}
//		}
//		break;
//	}
//	case WM_LBUTTONDOWN:
//	{
//		const POINTS pt = MAKEPOINTS( lParam );
//		mouse.OnLeftPressed( pt.x,pt.y );
//		SetForegroundWindow( hWnd );
//		break;
//	}
//	case WM_RBUTTONDOWN:
//	{
//		const POINTS pt = MAKEPOINTS( lParam );
//		mouse.OnRightPressed( pt.x,pt.y );
//		break;
//	}
//	case WM_LBUTTONUP:
//	{
//		const POINTS pt = MAKEPOINTS( lParam );
//		mouse.OnLeftReleased( pt.x,pt.y );
//		break;
//	}
//	case WM_RBUTTONUP:
//	{
//		const POINTS pt = MAKEPOINTS( lParam );
//		mouse.OnRightReleased( pt.x,pt.y );
//		break;
//	}
//	case WM_MOUSEWHEEL:
//	{
//		const POINTS pt = MAKEPOINTS( lParam );
//		if( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 )
//		{
//			mouse.OnWheelUp( pt.x,pt.y );
//		}
//		else if( GET_WHEEL_DELTA_WPARAM( wParam ) < 0 )
//		{
//			mouse.OnWheelDown( pt.x,pt.y );
//		}
//		break;
//	}
//	// ************ END MOUSE MESSAGES ************ //
//	}
//
//	return DefWindowProc( hWnd,msg,wParam,lParam );
//}