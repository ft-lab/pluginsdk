Shade 13.2.2 Plugin SDK (463042)
----------------------------------------
Thank you for downloading Shade 13 Plugin SDK. Before you use this SDK, please read the following requirements and up-to-date information for your development of Shade plugin.

This plugin SDK is based on Shade 13.1.1 build 461395 and is compatible with both Professional and Standard version. All plugins built with this SDK will run on Shade 13.1.1 and above version. If you want to run your plugin in the previous version of Shade, try use the old Plugin SDK or avoid to use those new functions listed in "What's new in Shade 13.1.1 Plugin SDK" section.


------------------------------------------
Inside this Plugin SDK
------------------------------------------

Shade 13.2.2 Plugin SDK contains:
- Sample Plugins and Project files for Windows development (/samples/windows/plugins)
- Sample Plugins and Project files for Mac OS X development (/samples/macosx/plugins)
- Source codes for sample plugins (/sample/source/plugins)
- SDK header files (/include)
- BOOST C++ Library files (/boost_1_43_0)
- Plugin SDK Document (/doc/index.en.html)


------------------------------------------
Plugin Development Environments
------------------------------------------

Windows OS:
- Windows 7 32bit / 64bit
- Windows Vista 32bit / 64bit
- Windows XP Professional x64 Edition
- Windows XP Professional / Home Edition / Media Center Edition (SP2 or above)

Windows Development Tools:
- Visual Studio 2010 (Visual C++ 2010) SP1
- Visual Studio 2008 (Visual C++ 2008) SP1

* Windows Me/98/95/NT 4.0 are not compatible.
* Visual Studio .NET 2003 / Visual C++ .NET 2003、Visual Studio .NET 2002 / Visual C++ .NET 2002 / Visual C++ 6.0 are not compatible.
* Visual C++ 2008/2010 Express Edition is not supported 64-bit development of Shade Plugin.
* Service Pack 1 is highly recommend when using Visual Studio 2008/2010.

	Download Visual Studio 2010 Service Pack 1,
	http://www.microsoft.com/en-us/download/details.aspx?id=23691

	Download Visual Studio 2008 Service Pack 1
	http://www.microsoft.com/en-us/download/details.aspx?id=10986

Mac OS X:
- OS X 10.6 / 10.7 / 10.8

Mac OS X Development Tools
- Xcode 3.2.1 later / Xcode 4.3.x / Xcode 4.4

* Classic / Mac OS 9 / Mac OS X 10.5 and previous versions are not supported.
* CodeWarrior, Project Builder and Xcode 3.2 are not supported.


--------------------------------------------------------
Plugin Development with Visual C++ 2010 Express Edition
--------------------------------------------------------

- For Visual Studio 2010 Standard Edition or later / Visual C++ 2010 Express Edition, Platform SDK is already embedded, so there is not need for extra settings.

	Download free version of Visual C++ 2010 Express Edition
	http://www.microsoft.com/visualstudio/en-us/products/2010-editions/visual-cpp-express

- Use this free version of Visual C++ 2010 Express Edition for Shade 13 plugin development.


------------------------------------------
Plugin Development with Xcode 4.4
------------------------------------------

- You can use Xcode 4.4 under Mac OS X 10.8 (Mountain Lion) for Shade Plugin development, however, you need to change some settings in your plugin project file as shown at the followings.

- In your plugins project, go to "Build Settings" and under the "Architectures" group, change the Base SDK to "OS X 10.7" instead of "OS X 10.6". Then your plugin will run under both OS X 10.7 and 10.8. However, if you want to run your plugin under 10.6, then you should use Xcode 4.3.


------------------------------------------
Plugin SDK Samples
------------------------------------------

- To try the sample plugins, copy them to your desired location together with the "include" and "boost_1_43_0" directory. 
- Simply open the sample project and build it.
- If you want to use the sample plugin as your base development, please make sure to change the plugin ID to your unique UUID instead.
- You can find the project file at "sample" directory that included all the sample plugin source codes.

	For Mac OS X: /macosx/plugins/plugins.xcodeproj
	For Windows: /windows/plugins/pluginsamples2008.sln or pluginsamples2010.sln


------------------------------------------
Location of 3rd Party Plugins
------------------------------------------

To use plugins, scripts or widgets from 3rd party with Shade 13, please install at the following location:

Windows Vista / 7
- Default My Documents at C:\Users\[user name]\Documents\Shade 13\plugins or scripts or widgets

Windows XP
- Default My Documents at C:\Documents and Settings\[user name]\My Documents\Shade 13\plugins or scripts or widgets

Mac OS X 10.6 / 10.7 / 10.8
- Local User Documents at /Users/[user name]/Documents/Shade 13/plugins or scripts or widgets


------------------------------------------
Boost License
------------------------------------------

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.


------------------------------------------
Copyright & Trademark Notice
------------------------------------------

Shade 13 Copyright(c) 1986-2012 e frontier, Inc. All rights reserved. Shade 13 is a trademark of e frontier, Inc. All other trademarks or registered trademarks are the property of their respective owners.

