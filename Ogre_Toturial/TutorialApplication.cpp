/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------
Function：	 程序主要功能实现
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"
#include "OgreParticleAffector.h"
#include "OgreParticleEmitter.h"
#include "OgreParticleSystemManager.h"

#if defined __CEGUIOGRE__

#include "CEGUI.h"
#include <RendererModules/Ogre/CEGUIOgreTexture.h>
#include <RendererModules/Ogre/CEGUIOgreTextureTarget.h>



//************************************
// Method:    convertButton
// FullName:  convertButton
// Access:    public 
// 返回值:    CEGUI::MouseButton
// 功  能:   将鼠标按钮转换成CEGUI按钮
// 函数参数:  OIS::MouseButtonID buttonID
//************************************
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}
	#ifdef __WOBBLLY_EFFECT__
/*
 *	RenderEffect相关部分
 */

const float MyEffect::tess_x = 8;
const float MyEffect::tess_y = 8;

//----------------------------------------------------------------------------//
MyEffect::MyEffect() :
    initialised(false),
    dragX(0), dragY(0),
    elasX(0), elasY(0)
{
}

//----------------------------------------------------------------------------//
int MyEffect::getPassCount() const
{
    return 1;
}

//----------------------------------------------------------------------------//
void MyEffect::performPreRenderFunctions(const int /*pass*/)
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
void MyEffect::performPostRenderFunctions()
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
bool MyEffect::realiseGeometry(CEGUI::RenderingWindow& window,
                               CEGUI::GeometryBuffer& geometry)
{
    using namespace CEGUI;
    Texture& tex = window.getTextureTarget().getTexture();

    static const CEGUI::colour c(1, 1, 1, 1);

    const float qw = window.getSize().d_width / tess_x;
    const float qh = window.getSize().d_height / tess_y;
    const float tcx = qw * tex.getTexelScaling().d_x;
    const float tcy =
        (window.getTextureTarget().isRenderingInverted() ? -qh : qh) *
            tex.getTexelScaling().d_y;

    for (int j = 0; j < tess_y; ++j)
    {
        for (int i = 0; i < tess_x; ++i)
        {
            int idx = (j * tess_x + i) * 6;

            float top_adj = dragX * ((1.0f / tess_x) * j);
            float bot_adj = dragX * ((1.0f / tess_x) * (j+1));
            top_adj = ((top_adj*top_adj) / 3) * (dragX < 0 ? -1 : 1);
            bot_adj = ((bot_adj*bot_adj) / 3) * (dragX < 0 ? -1 : 1);

            float lef_adj = dragY * ((1.0f / tess_y) * i);
            float rig_adj = dragY * ((1.0f / tess_y) * (i+1));
            lef_adj = ((lef_adj*lef_adj) / 3) * (dragY < 0 ? -1 : 1);
            rig_adj = ((rig_adj*rig_adj) / 3) * (dragY < 0 ? -1 : 1);

            // vertex 0
            vb[idx + 0].position   = Vector3(i * qw - top_adj, j * qh - lef_adj, 0.0f);
            vb[idx + 0].colour_val = c;
            vb[idx + 0].tex_coords = Vector2(i * tcx, j*tcy);

            // vertex 1
            vb[idx + 1].position   = Vector3(i * qw - bot_adj, j * qh + qh - lef_adj, 0.0f);
            vb[idx + 1].colour_val = c;
            vb[idx + 1].tex_coords = Vector2(i*tcx, j*tcy+tcy);

            // vertex 2
            vb[idx + 2].position   = Vector3(i * qw + qw - bot_adj, j * qh + qh - rig_adj, 0.0f);
            vb[idx + 2].colour_val = c;
            vb[idx + 2].tex_coords = Vector2(i*tcx+tcx, j*tcy+tcy);

            // vertex 3
            vb[idx + 3].position   = Vector3(i * qw + qw - bot_adj, j * qh + qh - rig_adj, 0.0f);
            vb[idx + 3].colour_val = c;
            vb[idx + 3].tex_coords = Vector2(i*tcx+tcx, j*tcy+tcy);

            // vertex 4
            vb[idx + 4].position   = Vector3(i * qw + qw - top_adj, j * qh - rig_adj, 0.0f);
            vb[idx + 4].colour_val = c;
            vb[idx + 4].tex_coords = Vector2(i*tcx+tcx, j*tcy);

            // vertex 5
            vb[idx + 5].position   = Vector3(i * qw - top_adj, j * qh - lef_adj, 0.0f);
            vb[idx + 5].colour_val = c;
            vb[idx + 5].tex_coords = Vector2(i * tcx, j*tcy);
        }
    }

    geometry.setActiveTexture(&tex);
    geometry.appendGeometry(vb, buffsize);

    // false, because we do not want the default geometry added!
    return false;
}

//----------------------------------------------------------------------------//
bool MyEffect::update(const float elapsed, CEGUI::RenderingWindow& window)
{
    using namespace CEGUI;
    
    // initialise ourself upon the first update call.
    if (!initialised)
    {
        initialised=true;
        lastX = window.getPosition().d_x;
        lastY = window.getPosition().d_y;
        return true;
    }

    const Vector2 pos(window.getPosition());

    //
    // Set up for X axis animation.
    //
    if (pos.d_x != lastX)
    {
        dragX += (pos.d_x - lastX) * 0.2;
        elasX = 0.05f;
        lastX = pos.d_x;

        if (dragX > 25)
            dragX = 25;
        else if (dragX < -25)
            dragX = -25;
    }

    //
    // Set up for y axis animation
    //
    if (pos.d_y != lastY)
    {
        dragY += (pos.d_y - lastY) * 0.2f;
        elasY = 0.05f;
        lastY = pos.d_y;

        if (dragY > 25)
            dragY = 25;
        else if (dragY < -25)
            dragY = -25;
    }

    //
    // Perform required animation steps
    //
    if ((dragX != 0) || (dragY != 0))
    {
        if (dragX < 0)
        {
            dragX += (elasX * 800 * elapsed);
            elasX += 0.075 * elapsed;
            if (dragX >0)
                dragX = 0;
        }
        else
        {
            dragX -= (elasX * 800 * elapsed);
            elasX += 0.075 * elapsed;
            if (dragX < 0)
                dragX = 0;
        }

        if (dragY < 0)
        {
            dragY += elasY * 800 * elapsed;
            elasY += 0.075 * elapsed;
            if (dragY >0)
                dragY = 0;
        }
        else
        {
            dragY -= elasY * 800 * elapsed;
            elasY += 0.075 * elapsed;
            if (dragY < 0)
                dragY = 0;
        }

        // note we just need system to redraw the geometry; we do not need a
        // full redraw of all window/widget content - which is unchanged.
        System::getSingleton().signalRedraw();
        return false;
    }

    return true;
}

	#endif
#endif

ITutorial01::ITutorial01(void):mRaySceneQuery(0),exFlag(0),mReplay(false)
{
}

ITutorial01::~ITutorial01(void)
{
	mWalkList.clear();
	// 创建射线查询，别忘了释放掉
	if(mRaySceneQuery)
	{
		mSceneMgr->destroyQuery(mRaySceneQuery);
	}
}

void ITutorial01::createScene(void)
{
	// 设置环境光
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 5000);  // set our skybox

	// 世界渲染地形
	/*
	 *	OGRE地形配置文件terrain.cfg简析
	 */
	//http://hi.baidu.com/kingmari/blog/item/5daf3a023531011b4afb5176.html
	mSceneMgr->setWorldGeometry("terrain.cfg");

	// 设置照相机的观测点
	mCamera->setPosition(Ogre::Vector3(2500, 30, 3980));
	//设置照相机的方向
	mCamera->lookAt(Ogre::Vector3(2600, 100, 3615));
	mCamera->setNearClipDistance(0.5);

	// Create the entity
	mEntity = mSceneMgr->createEntity("Robot", "robot.mesh");

	// Create the scene node
	mNode = mSceneMgr->getRootSceneNode()->
		createChildSceneNode("RobotNode", Ogre::Vector3(2680, 30, 4030));
	mNode->attachObject(mEntity);
	mNode->setScale(0.3f,0.3f,0.3f);

	// Create the walking list 创建行走路线 程序启动之后，robot行走路线
	mWalkList.push_back(Ogre::Vector3(2300, 0, 3760));
	mWalkList.push_back(Ogre::Vector3(2700, 0, 3250));
	/*
	 RTT(Render to Texture):渲染到纹理技术
	 普通的图形渲染流程中，最终结果是渲染到帧缓存中，
	 一句话：把渲染目标从帧缓存变成一个纹理
	 最后显示到屏幕上。现在可以利用FBO等技术，把图像渲
	 染到纹理中，然后可以把纹理继续应用到场景绘制中，比
	 如渲染一个场景A到纹理中，在另一个场景B的一个电视屏
	 幕上把刚才的纹理贴上去，就像是在播放A一样，阴影图（shadow mapping）也算是
	 运用了RTT。
	 */
	rttCam = mSceneMgr->createCamera("RTTCam");
	rttCam->setNearClipDistance(1);

	//RTT
	Ogre::TexturePtr tex = mRoot->getTextureManager()->createManual("RTT",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,512,512,0,Ogre::PF_R8G8B8,Ogre::TU_RENDERTARGET);

	rtex = tex->getBuffer()->getRenderTarget();

	Ogre::Viewport *v = rtex->addViewport(mSceneMgr->getCamera("RTTCam"));
	v->setOverlaysEnabled(false);
	v->setClearEveryFrame(true);
	v->setBackgroundColour(Ogre::ColourValue::Black);

	// create a particle system with 200 quota, then set its material and dimensions
	// 创建一个粒子系统 数量为200并设置材质尺寸
	thrusters = mSceneMgr->createParticleSystem("thrusters",25);
	thrusters ->setMaterialName("Examples/Flare");
	thrusters ->setDefaultDimensions(15, 15);

	// create emitter for our thruster particle system
	// 创建一个反射器，来推荐粒子系统
	Ogre::ParticleEmitter* emitter = thrusters ->addEmitter("Point");  // add a point emitter

	Ogre::Vector3 direct = mCamera->getDirection();
	//设置发射属性
	emitter->setAngle(Ogre::Degree(5));
	emitter->setTimeToLive(0.5);//存活时间
	emitter->setEmissionRate(20);
	emitter->setParticleVelocity(10);
	emitter->setDirection(Ogre::Vector3::ZERO);
	emitter->setColour(Ogre::ColourValue::White, Ogre::ColourValue::Red);
	emitter->setPosition(Ogre::Vector3(0,0,0));

	// 场景开始的时候，设置粒子系统的
	mSceneMgr->getRootSceneNode()->createChildSceneNode("emitter")->attachObject(thrusters);
	rttCam->detachFromParent();
	//  先banding到rttCam上
	mSceneMgr->getSceneNode("emitter")->attachObject(rttCam);
	//  然后再设置偏移量
	mSceneMgr->getSceneNode("emitter")->setPosition(mCamera->getPosition()+15*direct-Ogre::Vector3(0,10,0));

	// 创建运动物体，机器人Sphere
	Ogre::Entity *ent;
	Ogre::SceneNode *node;

	ent = mSceneMgr->createEntity("Sphere1", "sphere.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sphere0Node");
	node->attachObject(ent);
	node->setScale(0.02f, 0.02f, 0.02f);

	ent = mSceneMgr->createEntity("Sphere2", "sphere.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sphere1Node");
	node->attachObject(ent);
	node->setScale(0.02f, 0.02f, 0.02f);

	ent = mSceneMgr->createEntity("Sphere3", "sphere.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sphere2Node");
	node->attachObject(ent);
	node->setScale(0.02f, 0.02f, 0.02f);

/*
 *	设置小窗口模块
 */
#if defined __OGREMINISCREEN__
	//Ogre 小窗口 render to texture
	Ogre::Rectangle2D *mMiniScreen = new Ogre::Rectangle2D(true);
	mMiniScreen->setCorners(0.5f, -0.5f, 1.0f, -1.0f);
	mMiniScreen->setBoundingBox(Ogre::AxisAlignedBox(-100000.0f * Ogre::Vector3::UNIT_SCALE, 100000.0f * Ogre::Vector3::UNIT_SCALE));

	Ogre::SceneNode* miniScreenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode");
	miniScreenNode->attachObject(mMiniScreen);
	Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Technique* matTechnique = renderMaterial->createTechnique();
	matTechnique->createPass();
	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("RTT");
	mMiniScreen->setMaterial("RttMat");
#endif

#if defined __CEGUIOGRE__

	// CEGUI启动
	mGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

#ifdef __WOBBLLY_EFFECT__

	// 在系统中注册我们定义的效果
	CEGUI::RenderEffectManager::getSingleton().addEffect<MyEffect>("WobblyWindow");
	
	// From  tikiwiki 设置窗口，鼠标样式

    // Now we make a Falagard mapping for a frame window that uses this effect.
    // We create a type "TaharezLook/WobblyFrameWindow", which is subsequently
    // referenced in the layout file loaded below.  Note that it wold be more
    // usual for this mapping to be specified in the scheme xml file, though
    // here we are doing in manually to save from needing either multiple
    // versions of the schemes or from having demo specific definitions in
    // the schemes.
	
	CEGUI::WindowFactoryManager::getSingleton().addFalagardWindowMapping(
        "TaharezLook/WobblyFrameWindow",    // type to create
        "CEGUI/FrameWindow",                // 'base' widget type
        "TaharezLook/FrameWindow",          // WidgetLook to use.
        "Falagard/FrameWindow",             // WindowRenderer to use.
        "WobblyWindow");                    // effect to use.
#endif

	// Mouse
	CEGUI::SchemeManager::getSingleton().create((CEGUI::utf8*)"TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseArrow");

	/////////////////////// 渲染显示CEGUI logo图标 //////////////////////////////////////////

	// setup required to do direct rendering
	const CEGUI::Size hsz = mGUIRenderer->getDisplaySize() * 0.5f;
	const CEGUI::Rect scrn(CEGUI::Vector2(0, 0), hsz * 2);

	// setup for logo
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("cegui_logo", "logo.png", "Imagesets");
	d_logo_geometry = &mGUIRenderer->createGeometryBuffer();
	d_logo_geometry->setClippingRegion(scrn);
	d_logo_geometry->setPivot(CEGUI::Vector3(hsz.d_width, 34.75f, 0));
	d_logo_geometry->setTranslation(CEGUI::Vector3(0, hsz.d_height * 2 - 80, 0));
	CEGUI::ImagesetManager::getSingleton().get("cegui_logo").getImage("full_image").draw(*d_logo_geometry,
		CEGUI::Rect(hsz.d_width - 50, 0, hsz.d_width + 50, 69.5f), 0);

	// clearing this queue actually makes sure it's created(!)
	mGUIRenderer->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);

	// subscribe handler to render overlay items

	mGUIRenderer->getDefaultRenderingRoot().subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
		CEGUI::Event::Subscriber(&ITutorial01::overlayHandler,this));

	///////////////////////////////////////////////////////////////////////////////////

	CEGUI::Texture &guiTex = mGUIRenderer->createTexture(tex);
	CEGUI::Imageset &imageSet =	CEGUI::ImagesetManager::getSingleton().create("RTTImageset", guiTex);
	imageSet.defineImage("RTTImage",CEGUI::Point(0.0f, 0.0f),CEGUI::Size(guiTex.getSize().d_width,
		guiTex.getSize().d_height),	CEGUI::Point(0.0f, 0.0f));

	// CEGUI GUI sheet setup CEGUI层启动
	CEGUI::Window * sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "Root");
	CEGUI::System::getSingleton().setGUISheet(sheet);

	// RTTFrameWindow
#ifdef __WOBBLLY_EFFECT__
	CEGUI::Window *si = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/WobblyFrameWindow", "RTTFrameWindow");
#else
	CEGUI::Window *si = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", "RTTFrameWindow");
#endif
	/*
	 *	创建声音slider 控件
	 */
	sheet->addChildWindow(si);
	si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
	si->setProperty("CloseButtonEnabled","False");
	si->setProperty("Text","Sound");
	si->setProperty("ClientAreaColour","7F141B38");

	si = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Slider", "slider");
	CEGUI::WindowManager::getSingleton().getWindow("RTTFrameWindow")->addChildWindow(si);
	si->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(1.0f, 0)));
	si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.0f, 0)));
	si->setProperty("MaximumValue", "1");
	si->setProperty("CurrentValue", "0.7");
	si->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&ITutorial01::handleSlider,this));
	si = CEGUI::WindowManager::getSingleton().getWindow("RTTFrameWindow");
	si->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.5f, 0)));
	((CEGUI::FrameWindow*)si)->toggleRollup();

	si = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "RTTWindow");
	si->setProperty("BackgroundEnabled","False");
	si->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.4f, 0)));
	si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.0f, 0)));
	si->setProperty("Image", CEGUI::PropertyHelper::imageToString(&imageSet.getImage("RTTImage")));
	sheet->addChildWindow(si);
	si->setVisible(false);

	#ifdef __CEGUI_RTT__
	// get rendering surface used by FrameWindow.
	CEGUI::RenderingSurface* surface = &si->getTargetRenderingSurface();
    
    // if there's no surface, skip the RTT setup parts!
    if (surface)
    {
        initialiseRTTViewport(surface);

        // Since we set the Ogre RTT above to not be auto updated, we'll use an
        // event subscription to decide when to render that content manually.
        // Basically this needs to be done after other UI content is drawn but
        // before anything gets put on screen.  We need to use an appropriate
		// RenderingSurface for this - not the surface used for the window we
		// will render directly to - but something higher up.  For this demo
		// (and many other scenarios) the RenderingRoot will suffice.  We use
		// the EventRenderQueueStarted because we want to update the content
		// /before/ it's drawn to the surface (here that would be the screen).
		mGUIRenderer->getDefaultRenderingRoot().subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueEnded,
			CEGUI::Event::Subscriber(&ITutorial01::updateRTT, this));

		// subscribe to hear about resize events.  This is needed because
		// when the underlying texture gets resized, all the associated objects
		// must be assumed to be lost also
		si->subscribeEvent(CEGUI::Window::EventSized,CEGUI::Event::Subscriber(&ITutorial01::handleRTTResize, this));
	}
	#endif
#endif

#if defined __OGREOGGSOUNG__
	//OgreOggSound启动
	mSoundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("soundNode",mCamera->getPosition());

	if(mSoundManager->init())
	{
		mSoundNode->attachObject(mSoundManager->getListener());

		//	先 sound指针初始化 为空
		sound = 0;
		if ( sound = mSoundManager->createSound("PointSound", "WA121.wav") )
		{
			sound->disable3D(true);
		}

		sound = 0;
		sound = mSoundManager->createSound("background", "p42.ogg", true, true);
		if ( sound ) 
		{
			sound->disable3D(true);
			sound->play();
		}

		sound = 0;
		sound = mSoundManager->createSound("Fire", "WD366.wav", false, false, true);
		if ( sound ) 
		{
			sound->disable3D(true);
		}

		sound = 0;
		if ( sound = sound = mSoundManager->createSound("Explosion","WD309.wav", false, false, true) )
		{
			sound->setMaxDistance(3000);
			sound->setReferenceDistance(1000);
			mSceneMgr->getSceneNode("emitter")->attachObject(sound);
		}

		sound = 0;
		if ( sound = mSoundManager->createSound("Emitter", "WD299.wav", true, true) )
		{
			sound->setMaxDistance(150);
			sound->setReferenceDistance(50);
			mSceneMgr->getSceneNode("emitter")->attachObject(sound);
			sound->play();
			sound->setLoopOffset(5.f);
		}

		sound = 0;
		if ( sound = mSoundManager->createSound("StepSound", "WF038.wav", false, true) )
		{
			sound->setMaxDistance(300);
			sound->setReferenceDistance(100);
			mNode->attachObject(sound);
		}

		mSoundManager->setMasterVolume(0.7f);
	}
#endif

}

#if defined __CEGUIOGRE__
	#ifdef __CEGUI_RTT__
/*
 *	初始化 RTT视口
 */
void ITutorial01::initialiseRTTViewport(CEGUI::RenderingSurface* const surface)
{
	const Ogre::RenderTexture* const old_rtt = rtex;

	// extract the Ogre render target at for the surface
	rtex = dynamic_cast<CEGUI::OgreTexture&>(dynamic_cast<CEGUI::OgreTextureTarget&>(
		surface->getRenderTarget()).getTexture()).getOgreTexture()->getBuffer()->getRenderTarget();

	// only do set up if target is changed.
	if (old_rtt != rtex)
	{
		// tell Ogre not to draw this target automatically (else you get
		// visible flickering).
		//rtex->setAutoUpdated(false);

		// setup viewport.
		Ogre::Viewport *v = rtex->addViewport(mSceneMgr->getCamera("RTTCam"));
		v->setOverlaysEnabled(false);
		v->setClearEveryFrame(true);
		v->setBackgroundColour(Ogre::ColourValue::Black);
	}
}

bool ITutorial01::updateRTT(const CEGUI::EventArgs& args)
{
	// There are many rendering queues that may (or may not) be present for
	// a RenderingSurface.  For our purposes, we can hook the 'default' of
	// RQ_BASE. - and we need to do our rendering /before/ this queue
	if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_BASE)
		return false;
	// get the target to draw itself.
	rtex->update();
	// Reset the initial CEGUI rendering states.
	mGUIRenderer->initialiseRenderStateSettings();
	// (Re)Activate the appropriate CEGUI render target.
	mGUIRenderer->getDefaultRenderingRoot().getRenderTarget().activate();
	return true;
}

bool ITutorial01::handleRTTResize(const CEGUI::EventArgs& args)
{
	// get rendering surface used by FrameWindow.
	CEGUI::RenderingSurface* surface = static_cast<const CEGUI::WindowEventArgs&>(args).window
		->getRenderingSurface();

	// if there's no surface, skip the RTT setup parts!
	if (surface)
	{
		initialiseRTTViewport(surface);
	}

	return true;
}
	#endif

bool ITutorial01::overlayHandler(const CEGUI::EventArgs& args)
{
	if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_OVERLAY)
		return false;
	d_logo_geometry->draw();
	return true;
}
	// 声音改变回调函数
bool ITutorial01::handleSlider(const CEGUI::EventArgs& args)
{
	float val = static_cast<CEGUI::Slider*>(static_cast<const CEGUI::WindowEventArgs&>(args).window)->getCurrentValue();
	mSoundManager->setMasterVolume(val);
	return true;
}

#endif

void ITutorial01::createFrameListener(void)
{
	// 设置robot空闲时的动作
	mAnimationState = mEntity->getAnimationState("Idle");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);

	// 设置robot的初始参数
	mWalkSpeed = 50.0f;
	mDirection = Ogre::Vector3::ZERO;

	// Setup default variables
	mLMouseDown = false;
	mRMouseDown = false;

	// Reduce rotate speed
	mRotateSpeed =.1f;
	// 创建射线查询
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	BaseApplication::createFrameListener();
}
// 取下一个移动位置
bool ITutorial01::nextLocation(void)
{
	// 如果链表list为空
	if (mWalkList.empty())
		return false;
	// 取第一个节点
	mDestination = mWalkList.front();  // this gets the front of the deque
	// 删除第一个节点
	mWalkList.pop_front();             // this removes the front of the deque

	// 为了简单，设置Y轴值总为0
	mDirection = mDestination - mNode->getPosition();
	mDirection.y = 0;
	mDistance = mDirection.normalise();
	return true;
}
 
/*
 *	每帧渲染查询
 */
bool ITutorial01::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	
#if defined __CEGUIOGRE__
	// 更新CEGUI logo 使其旋转
	static float rot = 0.0f;
	d_logo_geometry->setRotation(CEGUI::Vector3(rot, 0, 0));
	rot += 180.0f * (evt.timeSinceLastFrame);
	if (rot > 360.0f)
		rot -= 360.0f;
#endif

#if defined __OGREOGGSOUNG__
	//设置观察者的位置（听众）
	mSoundNode->setPosition(mCamera->getPosition());
	mSoundManager->update(evt.timeSinceLastFrame);
#endif

	// 启动场景查询
	Ogre::Vector3 nodePos = mNode->getPosition();
	Ogre::Ray nodeRay(Ogre::Vector3(nodePos.x, 5000.0f, nodePos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
	mRaySceneQuery->setRay(nodeRay);

	// Perform the scene query
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	// Get the results, set the RobotNode height
	if (itr != result.end() && itr->worldFragment)
	{
		Ogre::Real terrainHeight = itr->worldFragment->singleIntersection.y;
		if ((terrainHeight) != nodePos.y)
			mNode->setPosition( nodePos.x, terrainHeight, nodePos.z );
	}

	// Setup the scene query. For the camera
	nodePos = mCamera->getPosition();
	nodeRay.setOrigin(Ogre::Vector3(nodePos.x, 5000.0f, nodePos.z));
	mRaySceneQuery->setRay(nodeRay);

	// Perform the scene query
	result = mRaySceneQuery->execute();
	itr = result.begin();

	// Get the results, set the camera height
	// 摄像机不穿过地面设置
	if (itr != result.end() && itr->worldFragment)
	{
		Ogre::Real terrainHeight = itr->worldFragment->singleIntersection.y;
		if ((terrainHeight + 10.0f) >= nodePos.y)
			mCamera->setPosition( nodePos.x, terrainHeight + 10.0f, nodePos.z );
	}

	// Here nodePos is just a vector of direction
	nodePos = thrusters->getEmitter(0)->getDirection();
	if(!nodePos.isZeroLength())
	{
		mSceneMgr->getSceneNode("emitter")->translate(nodePos * 150 * evt.timeSinceLastFrame);
		if(!(exFlag & 0x01))
		{
			nodePos = mSceneMgr->getSceneNode("emitter")->getPosition();
			if((nodePos - mCamera->getPosition()).squaredLength() > 25000000)
			{
				thrusters->getEmitter(0)->setDirection(Ogre::Vector3::ZERO);
				exFlag |= 0x01;
			}
			else
			{
				nodeRay.setOrigin(nodePos);
				nodeRay.setDirection(thrusters->getEmitter(0)->getDirection());
				mRaySceneQuery->setRay(nodeRay);
				// Execute query
				result = mRaySceneQuery->execute();
				
				for(itr = result.begin();itr != result.end();++itr)
				{
					if(itr->distance < 0.5f && itr->movable && !itr->movable->getName().compare("Robot"))
					{
						thrusters->getEmitter(0)->setDirection(Ogre::Vector3::ZERO);
						exFlag |= 0x01;
						exFlag |= 0x02;
						break;
					}
					if (itr->distance < 3.0f && itr->worldFragment)
					{
						thrusters->getEmitter(0)->setDirection(Ogre::Vector3::ZERO);
						exFlag |= 0x01;
						break;
					} // if
				}
			}
		}
	}

	static float size = 15.0f;

#if defined __CEGUIOGRE__
	if(exFlag & 0x04)
	{
		size += evt.timeSinceLastFrame;
		if(size > 15.7f)
		{
			CEGUI::WindowManager::getSingleton().getWindow("RTTWindow")->setVisible(false);
			size = 15.0f;
			exFlag &= ~0x04;
		}
	}
	else
#endif

	if(exFlag & 0x01)
	{

#if defined __OGREOGGSOUNG__
		if(size < 15.001f)
		{
			//发射停止,爆炸声播放
			mSoundManager->getSound("Emitter")->pause();
			mSoundManager->getSound("Explosion")->play();
		}
#endif

		// when it goes into an explosion, we just let its size increase rapidly
		
		size *= (1 + 2.5f * evt.timeSinceLastFrame);
		thrusters->setDefaultDimensions(size,size);
		if(thrusters->getDefaultHeight() >= 900)
		{
			thrusters->setDefaultDimensions(15,15);
			size = 15.0f;
			thrusters->getEmitter(0)->setEnabled(false);
			if(exFlag & 0x02)
			{
				mNode->setVisible(false);
				mDistance = 0.0f;
				exFlag &= ~0x02;
			}
			exFlag &= ~0x01;
			exFlag |= 0x04;
		}
	}

	if (mDirection == Ogre::Vector3::ZERO) 
	{

		//如果下个节点存在，就移动到下一个节点，播放脚步声
		if (nextLocation()) 
		{

#if defined __OGREOGGSOUNG__
			mSoundManager->getSound("StepSound")->play();
#endif
			// Set walking animation
			mAnimationState = mEntity->getAnimationState("Walk");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
			// Rotation Code will go here
			Ogre::Vector3 src = mNode->getOrientation() * Ogre::Vector3::UNIT_X;
			if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
			{
				mNode->yaw(Ogre::Degree(180));
			}
			else
			{
				// 设置移动方向
				Ogre::Quaternion quat = src.getRotationTo(mDirection);
				mNode->rotate(quat);
			} // else
		}
	}
	else
	{
		Ogre::Real move = mWalkSpeed * evt.timeSinceLastFrame;
		mDistance -= move;
		if (mDistance <= 0.0f)
		{
			mNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;
			// Set animation based on if the robot has another point to walk to. 
			if (! nextLocation())
			{

#if defined __OGREOGGSOUNG__
				mSoundManager->getSound("StepSound")->stop();
#endif
				// Set Idle animation                     
				mAnimationState = mEntity->getAnimationState("Idle");
				mAnimationState->setLoop(true);
				mAnimationState->setEnabled(true);
			} 
			else
			{
				// Rotation Code will go here later
				Ogre::Vector3 src = mNode->getOrientation() * Ogre::Vector3::UNIT_X;
				//方向计算，向量的点积  mDirection 是一个Vector3
				if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
				{
					mNode->yaw(Ogre::Degree(180));
				}
				else
				{
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					mNode->rotate(quat);
				} // else
			}
		}
		else if(!(exFlag & 0x02))
		{
			mNode->translate(mDirection * move);
		} // else
	} // if

	mAnimationState->addTime(evt.timeSinceLastFrame);

	return BaseApplication::frameRenderingQueued(evt);
}
/*
*	场景管理时选择 ST_EXTERIOR_CLOSE：优化了室外场景里面的中近距离可视体，
*	比较适合用一个简单模型或者高度场产生的场景地图
*	http://game.chinaitlab.com/program/846984_4.html
*/
void ITutorial01::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
	
#if defined __OGREOGGSOUNG__
	mSoundManager = OgreOggSound::OgreOggSoundManager::getSingletonPtr();
#endif
}

bool ITutorial01::mouseMoved(const OIS::MouseEvent &arg)
{
	if (mRMouseDown)
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * mRotateSpeed));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * mRotateSpeed));
	} // else if
#if defined __CEGUIOGRE__
	else
	{
		// Update CEGUI with the mouse motion
		// 为CEGUI更新鼠标移动
		if (arg.state.Z.rel)
		{
			CEGUI::System::getSingleton().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
		}
		CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	}
#endif

	return true;
}

bool ITutorial01::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
	{
#if defined __CEGUIOGRE__
		CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseTarget");
		CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseTarget");
		// 鼠标右键按下，则鼠标改变形状为一个圆形
		CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(arg.state.width*0.5,arg.state.height*0.5));
#endif
		mRMouseDown = true;
	}
	else
	{
#if defined __CEGUIOGRE__
		CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
#endif
		// Left mouse button down
		if (id == OIS::MB_Left && !(exFlag & 0x05))
		{
			if(mRMouseDown)
			{	
			//如果左键按下，右键也按下，则RTTwindow显示出来
#if defined __CEGUIOGRE__
				CEGUI::WindowManager::getSingleton().getWindow("RTTWindow")->setVisible(true);
#endif

#if defined __OGREOGGSOUNG__
				//播放相应的音乐
				sound = mSoundManager->getSound("Fire");
				if(sound->isPlaying())	sound->stop();
				sound->play();
				mSoundManager->getSound("Emitter")->play();
#endif
				/*
				 *	粒子发射模块，需要调整摄像机
				 */
				Ogre::Vector3 tor = mCamera->getDirection();
				//here we get the boomb flying
				mSceneMgr->getSceneNode("emitter")->setPosition(mCamera->getPosition());
				thrusters->getEmitter(0)->setDirection(tor);
				thrusters->getEmitter(0)->setEnabled(true);
				tor.y = 0.0f;
				// we use a cross product to got a right look of RTTcam
				tor = tor.crossProduct(Ogre::Vector3::UNIT_Y).normalisedCopy();
				//tor.y = 0.4f;
				// we've let the CamraNode move with the emitter
				// so we just set relative position of the rttcam
				// Note: here the emitter have the same position with mCamera
				tor *= 5;
				tor -= mCamera->getDirection() * 55;
				rttCam->setPosition(tor);
				rttCam->lookAt(mCamera->getPosition());
			}
			else
			{
#if defined __CEGUIOGRE__
				// Setup the ray scene query, use CEGUI's mouse position
				CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
				Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/float(arg.state.width), mousePos.d_y/float(arg.state.height));
#else
				Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(0.5,0.5);
#endif
				mRaySceneQuery->setRay(mouseRay);
				// Execute query
				Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
				Ogre::RaySceneQueryResult::iterator itr = result.begin( );

				// Get results, create a node/entity on the position
				if (itr != result.end() && itr->worldFragment)
				{

#if defined __OGREOGGSOUNG__
					sound = mSoundManager->getSound("PointSound");
					if(sound->isPlaying()) sound->stop();
					sound->play();
#endif
					/*
					 *	拾取robot，并且将其置于鼠标按下的位置处
					 */
					if(mNode->getAttachedObject(0)->isVisible())
					{
						char name[16] = {0};
						static size_t i=0;
						sprintf_s(name,sizeof(name)-1,"Sphere%dNode",i++%3);
						mSceneMgr->getSceneNode(name)->setPosition(itr->worldFragment->singleIntersection + Ogre::Vector3(0,2,0));
						mWalkList.push_back(itr->worldFragment->singleIntersection);
					}
					else
					{
						mWalkList.clear();
#if defined __OGREOGGSOUNG__
						mSoundManager->getSound("StepSound")->stop();
#endif
						// Set Idle animation                     
						mAnimationState = mEntity->getAnimationState("Idle");
						mAnimationState->setLoop(true);
						mAnimationState->setEnabled(true);
						mNode->setPosition(itr->worldFragment->singleIntersection);
						mNode->setVisible(true);
					}
				} // if
			}
		} // if
	}
	return true;
}

bool ITutorial01::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
	{
#if defined __CEGUIOGRE__
		CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
		CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseArrow");
#endif
		mRMouseDown = false;
	} // else if
#if defined __CEGUIOGRE__
	else
	{
		CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	}
#endif
	return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {	// extern "c" 实现c,C++ 混合编程
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		ITutorial01 app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif