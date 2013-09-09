/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
-----------------------------------------------------------------------------
Function：	 Ogre启动，资源加载，摄像头等的初始化相关定义
-----------------------------------------------------------------------------
*/
#ifndef __BaseApplication_h_
#define __BaseApplication_h_
 
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
 
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
 
#include <SdkTrays.h>
#include <SdkCameraMan.h>
	/*	
	*	包含插件
	*	避免重复包含文件
	*/
#ifdef OGRE_STATIC_LIB
#  include "OgreOggSoundPlugin.h"
#  include "RenderSystems\Direct3D9\OgreD3D9Plugin.h"
#  include "Plugins\ParticleFX\OgreParticleFXPlugin.h"
#  include "Plugins\OctreeSceneManager\OgreOctreePlugin.h"
#endif
/*
 *	BaseApplication 纯虚类
 */
class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
	BaseApplication(void);
	virtual ~BaseApplication(void);
	virtual void go(void);
 
protected:

	/*
	*	框架事件
	*/	
	virtual bool setup();
	virtual bool configure(void);
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createScene(void) = 0; // Override me!
	virtual void destroyScene(void);
	virtual void createViewports(void);
	virtual void setupResources(void);
	virtual void createResourceListener(void);
	virtual void loadResources(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);// 进入帧渲染队列
	/*
	*	处理输入设备的事件
	*/	
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
 
	//Adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);

	/*
	*	窗口关闭时，释放OIS对鼠标的控制
	*/	
	virtual void windowClosed(Ogre::RenderWindow* rw);
 
	Ogre::Root *mRoot;					//Ogre根节点
	Ogre::Camera* mCamera;				//摄像机
	Ogre::SceneManager* mSceneMgr;		//场景管理器
	Ogre::RenderWindow* mWindow;		//渲染窗口
	Ogre::String mResourcesCfg;			//资源配置文件路径
	Ogre::String mPluginsCfg;			//插件配置文件路径
 
	// OgreBites Ogre3D sample framework的命名空间 
	OgreBites::SdkTrayManager*	mTrayMgr;
	OgreBites::SdkCameraMan* mCameraMan;     	// 基本的摄像控制
	OgreBites::ParamsPanel* mDetailsPanel;   	// sample details panel
	bool mCursorWasVisible;						// 在对话框出现时鼠标是否可见
	bool mShutDown;
 

	/*
	*	OIS输入设备管理及定义
	*/	
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;


	/*	
	*	加载插件
	*	避免重复包含文件
	*/
#ifdef OGRE_STATIC_LIB
	Ogre::ParticleFXPlugin* mParticleFXPlugin;
	Ogre::D3D9Plugin* mD3D9Plugin;
	Ogre::OctreePlugin* mOctreePlugin;
	#if defined __OGREOGGSOUNG__
	OgreOggSound::OgreOggSoundPlugin* mOgreOggSoundPlugin;
	#endif
#endif

};
 
#endif // #ifndef __BaseApplication_h_