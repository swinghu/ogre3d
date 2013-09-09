/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
-----------------------------------------------------------------------------
Function��	 Ogre��������Դ���أ�����ͷ�ȵĳ�ʼ����ض���
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
	*	�������
	*	�����ظ������ļ�
	*/
#ifdef OGRE_STATIC_LIB
#  include "OgreOggSoundPlugin.h"
#  include "RenderSystems\Direct3D9\OgreD3D9Plugin.h"
#  include "Plugins\ParticleFX\OgreParticleFXPlugin.h"
#  include "Plugins\OctreeSceneManager\OgreOctreePlugin.h"
#endif
/*
 *	BaseApplication ������
 */
class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
	BaseApplication(void);
	virtual ~BaseApplication(void);
	virtual void go(void);
 
protected:

	/*
	*	����¼�
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
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);// ����֡��Ⱦ����
	/*
	*	���������豸���¼�
	*/	
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
 
	//Adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);

	/*
	*	���ڹر�ʱ���ͷ�OIS�����Ŀ���
	*/	
	virtual void windowClosed(Ogre::RenderWindow* rw);
 
	Ogre::Root *mRoot;					//Ogre���ڵ�
	Ogre::Camera* mCamera;				//�����
	Ogre::SceneManager* mSceneMgr;		//����������
	Ogre::RenderWindow* mWindow;		//��Ⱦ����
	Ogre::String mResourcesCfg;			//��Դ�����ļ�·��
	Ogre::String mPluginsCfg;			//��������ļ�·��
 
	// OgreBites Ogre3D sample framework�������ռ� 
	OgreBites::SdkTrayManager*	mTrayMgr;
	OgreBites::SdkCameraMan* mCameraMan;     	// �������������
	OgreBites::ParamsPanel* mDetailsPanel;   	// sample details panel
	bool mCursorWasVisible;						// �ڶԻ������ʱ����Ƿ�ɼ�
	bool mShutDown;
 

	/*
	*	OIS�����豸��������
	*/	
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;


	/*	
	*	���ز��
	*	�����ظ������ļ�
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