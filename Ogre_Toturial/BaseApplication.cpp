/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------
Function��	 Ogre��������Դ���أ�����ͷ�ȵĳ�ʼ�����ʵ��
-----------------------------------------------------------------------------
*/
#include "BaseApplication.h"
 
//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
	: mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mTrayMgr(0),
	mCameraMan(0),
	mDetailsPanel(0),
	mCursorWasVisible(false),
	mShutDown(false),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0)
{
}
 
//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;

	/*	
	*	������������Ҫ��ȫ�ͷ���Դ
	*	ж�ص����в�� uninstallPlugin
	*	�����ظ������ļ�
	*/
#ifdef OGRE_STATIC_LIB
	mRoot->uninstallPlugin(mOgreOggSoundPlugin);
#endif
 
	/*
	*	������ڼ���banding
	*/	
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;

#ifdef OGRE_STATIC_LIB
	OGRE_DELETE mParticleFXPlugin;
	OGRE_DELETE mD3D9Plugin;
	OGRE_DELETE mOctreePlugin;
	#if defined __OGREOGGSOUNG__
	OGRE_DELETE mOgreOggSoundPlugin;
	#endif
#endif
}
 
//-------------------------------------------------------------------------------------

bool BaseApplication::configure(void)
{
	/*
	 *	��ʾ���öԻ��򣬲��ҳ�ʾ��ϵͳ������Ҳ����������һ��
	 */
	if(mRoot->showConfigDialog())
	{
		/*
		 *	��ʼ��ϵͳ������һ��Ĭ�ϵ���Ⱦ����
		 */
		mWindow = mRoot->initialise(true, "TutorialApplication Render Window");
 
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
	// ���������
	mCamera = mSceneMgr->createCamera("PlayerCam");
 
	// �����������Z����  80 ��λmCamera->setPosition(Ogre::Vector3(0,0,80));
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// ����Ϊ-z����
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);
 
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // ����һ��Ĭ�ϵ������������
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
 
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
	mInputManager = OIS::InputManager::createInputSystem( pl );
 
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
	/*
	 *	������꣬���̻ص�����
	 */
	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
 
	//Set initial mouse clipping size
	windowResized(mWindow);
 
	/*
	 *	ע��һ�������� ��ע��C++�е�thisָ��
	 */
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
	/*
	 *	�����ȥ��Ogre��֡����ʾͼ�꣬����ͨ���������ʵ��
	 */
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mTrayMgr->toggleAdvancedFrameStats();
	mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	mTrayMgr->hideCursor();
 
	/*
	 *	����camera������壬�Ա���ʾ��ϸ��Ϣ
	 */
	Ogre::StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("");
	items.push_back("Filtering");
	items.push_back("Poly Mode");
 
	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(9, "Bilinear");
	mDetailsPanel->setParamValue(10, "Solid");
	mDetailsPanel->hide();
 
	mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------

// ��  ��:    ʵ��ExampleApplication���󷽷�
// ��������:  void
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
//************************************
// Method:    createViewports
// FullName:  BaseApplication::createViewports
// Access:    virtual protected 
// ����ֵ:    void
// ��  ��:   �����ӿ�
// ��������:  void
//************************************
void BaseApplication::createViewports(void)
{
	// Create one viewport, entire window
	// ����һ���ӿڲ�����banding�������
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
	// Alter the camera aspect ratio to match the viewport
	// ��������������ʺ��ӿ�
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
	// Load resource paths from config file
	/*
	 *	�������ļ��м�����Դ
	 */
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);
 
	/*
	 *	��������ѡ�����Ӧ����
	 */
	
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{
 
}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif
 
	if (!setup())
		return;
 
	mRoot->startRendering();
 
	// clean up
	destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
	/*
	If we need to give a static link, then we should manully new an object of
	the plugins and install them. And since we create them with new method, 
	do not forget to delete them before the programe exit. 
	*/
#ifdef OGRE_STATIC_LIB
	mRoot = OGRE_NEW Ogre::Root("", "ogre.cfg", "ogre.log");
	mParticleFXPlugin = OGRE_NEW Ogre::ParticleFXPlugin();
	mRoot->installPlugin(mParticleFXPlugin);
	mD3D9Plugin = OGRE_NEW Ogre::D3D9Plugin();
	mRoot->installPlugin(mD3D9Plugin);
	mOctreePlugin = OGRE_NEW Ogre::OctreePlugin();
	mRoot->installPlugin(mOctreePlugin);
	#if defined __OGREOGGSOUNG__
	mOgreOggSoundPlugin = OGRE_NEW OgreOggSound::OgreOggSoundPlugin();
	mRoot->installPlugin(mOgreOggSoundPlugin);
	#endif
#else
	mRoot = new Ogre::Root(mPluginsCfg);
#endif
 
	setupResources();
 
	bool carryOn = configure();
	if (!carryOn) return false;
 
	chooseSceneManager();
	createCamera();
	createViewports();
 
	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
 
	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();
 
	// Create the scene
	createScene();
 
	createFrameListener();
 
	return true;
};
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;
 
	if(mShutDown)
		return false;
 
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
 
	mTrayMgr->frameRenderingQueued(evt);
 
	if (!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
			mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
			mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
			mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
			mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
			mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
		}
	}
 
	return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
	// ����Ի��򵯳����Ͳ�������
	if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
 
	if (arg.key == OIS::KC_F)  // ��ʾ��ϸ֡����Ϣ 
	{
		mTrayMgr->toggleAdvancedFrameStats();
	}
	else if (arg.key == OIS::KC_G)  // ��ʾ������Ĳ������߶ȣ�λ����Ϣ 
	{
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
	}
	else if (arg.key == OIS::KC_T)   // ���ö������Ⱦģʽ
		
	{
		Ogre::String newVal;
		Ogre::TextureFilterOptions tfo;
		unsigned int aniso;
 
		switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
		{
		case 'B':
			newVal = "Trilinear";//�����������ѡ�� �鿴DirectX9 ʹ���θ���ƽ��
			tfo = Ogre::TFO_TRILINEAR;
			aniso = 1;
			break;
		case 'T':
			newVal = "Anisotropic";
			tfo = Ogre::TFO_ANISOTROPIC;
			aniso = 8;
			break;
		case 'A':
			newVal = "None";
			tfo = Ogre::TFO_NONE;
			aniso = 1;
			break;
		default:
			newVal = "Bilinear";
			tfo = Ogre::TFO_BILINEAR;
			aniso = 1;
		}
 
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
		mDetailsPanel->setParamValue(9, newVal);
	}
	/*
	 *	 cycle polygon rendering mode,������Ⱦģ��
	 */
	else if (arg.key == OIS::KC_R)  
		
	{
		Ogre::String newVal;
		Ogre::PolygonMode pm;
 
		switch (mCamera->getPolygonMode())
		{
		case Ogre::PM_SOLID:
			newVal = "Wireframe";
			pm = Ogre::PM_WIREFRAME;
			break;
		case Ogre::PM_WIREFRAME:
			newVal = "Points";
			pm = Ogre::PM_POINTS;
			break;
		default:
			newVal = "Solid";
			pm = Ogre::PM_SOLID;
		}
 
		mCamera->setPolygonMode(pm);
		mDetailsPanel->setParamValue(10, newVal);
	}
	else if(arg.key == OIS::KC_F5)   // F5ˢ�����в���
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_SYSRQ)   // PrintScreen
	{
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");//�������
	}
	else if (arg.key == OIS::KC_ESCAPE)//�˳�
	{
		mShutDown = true;
	}
 
	mCameraMan->injectKeyDown(arg);
	return true;
}
 
bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	mCameraMan->injectKeyUp(arg);
	return true;
}
 
bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
	mCameraMan->injectMouseMove(arg);
	return true;
}
 
bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
	mCameraMan->injectMouseDown(arg, id);
	return true;
}
 
bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
	mCameraMan->injectMouseUp(arg, id);
	return true;
}
 
//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);
 
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}
 
// ��window���ر�ʱ���ͷ�OIS�Ŀ���
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{

	//����ֻ�Կ�����OIS�Ĵ��� ���йرգ�û�п��ٵĲ���ر�
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );
 
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}
