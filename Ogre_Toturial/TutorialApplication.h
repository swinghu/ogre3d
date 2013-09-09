/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------
Function：	Ogre启动加载框架头文件
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

/* If we use OgreOggSound libs to provides sound support for the programe. */
/*	添加声音效果，加载OggSound库文件
*	避免重复包含文件
*/
#if defined __OGREOGGSOUNG__
#include "OgreOggSound.h"
#endif

/* Use CEGUI system to render independent GUI and deal with some graphic inputs. */
/*	如果我们想要添加CEGUI效果，加载CEGUI的OGRE渲染库库文件
*	避免重复包含文件
*/
#if defined __CEGUIOGRE__
#include <CEGUISystem.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUISchemeManager.h>

	#ifdef __WOBBLLY_EFFECT__
	/*
	 *	加载CEGUI渲染效果模块相关头文件
	 */
	#include <CEGUIRenderEffect.h>
	#include <CEGUIVertex.h>
	// 自定义一个子类继承自CEGUI::RenderEffect类
	/*	
	* CEGUI::RenderEffect为抽象类,子类若实例化，必须重写方法
	*/

	class MyEffect : public CEGUI::RenderEffect
	{
	public:
		MyEffect();

		// CEGUI::RenderEffect接口 
		int getPassCount() const;

		void performPreRenderFunctions(const int pass);
		void performPostRenderFunctions();

		/* GUI渲染过程：  
			1 计算RenderWindow信息  
			2 写入GeometryBuffer图像信息（Vecotr[6]的两个三角形，以及纹理什么的）  
			3 GeometryBuffer Draw  
			这一步实际就是在1，2之间时的调用  
		*/

		bool realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry);

		//************************************
		// Method:    update
		// FullName:  MyEffect::update
		// Access:    public 
		// 返回值:    bool
		// 功  能:   每帧调用	
		// 函数参数:  const float elapsed
		// 函数参数:  CEGUI::RenderingWindow & window
		//************************************
		bool update(const float elapsed, CEGUI::RenderingWindow& window);
	
	protected:
	    static const float tess_x;
	    static const float tess_y;
		// tess_x * tess_y * vertex_per_quad
		static const int buffsize = (8 * 8 * 6);
	
		bool initialised;
	
		float lastX, lastY;
		float dragX, dragY;
		float elasX, elasY;
	
		CEGUI::Vertex vb[buffsize];
	};

	#endif

#endif
 
	/*	加载robot，deque管理robot的路径
	*	避免重复包含文件
	*/

#include <deque>
/*
 *	如果是win32 平台就要添加如下代码
 */
/*  #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#include "../res/resource.h"
	#endif
 */

/*
 *	继承自BaseApplication
 */
class ITutorial01 : public BaseApplication
{
public:
	/*
	*	构造函数
	*/
    ITutorial01(void);
	/*
	*	析构函数
	*/
    virtual ~ITutorial01(void);
 
protected:
     virtual void createScene(void);
     virtual void createFrameListener(void);
	 // find the next location for the robot to walk to. 
	 // 定位到机器人下一个位置
     virtual bool nextLocation(void);
	 virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	 // because we need to query terrain, so choose sceneManager of type ST_EXTERIOR_CLOSE

	 virtual void chooseSceneManager(void);

	 //mouse listener
	 virtual bool mouseMoved(const OIS::MouseEvent &arg);
	 virtual bool mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
	 virtual bool mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
	/*
	*	CEGUI模块
	*/
#if defined __CEGUIOGRE__
	 virtual bool handleSlider(const CEGUI::EventArgs&);	// slider the voice control
	 virtual bool overlayHandler(const CEGUI::EventArgs&);	// handler to the logo overlay
	#ifdef __WOBBLLY_EFFECT__
	 virtual bool updateRTT(const CEGUI::EventArgs&);
	 virtual bool handleRTTResize(const CEGUI::EventArgs&);
	 virtual void initialiseRTTViewport(CEGUI::RenderingSurface* const);
	#endif
	 CEGUI::OgreRenderer *mGUIRenderer;			// CEGUI renderer
	 CEGUI::GeometryBuffer* d_logo_geometry;
#endif
	/*
	*	射线查询模块，参数定义
	*/
	 Ogre::RaySceneQuery *mRaySceneQuery;	// 射线查询
	 bool mLMouseDown, mRMouseDown;			// True if the mouse buttons are down
	 float mRotateSpeed;

	 Ogre::Real mDistance;                  // object剩下所需移动的距离
	 Ogre::Vector3 mDirection;              // object移动的方向
	 Ogre::Vector3 mDestination;            // object将要移动到的目的地

	 Ogre::AnimationState *mAnimationState; // object的当前运动状态，死亡，行走，跑
	
	 Ogre::Entity *mEntity;                 // 运动的实体
	 Ogre::SceneNode *mNode,*mSoundNode;	// 实体 附加到场景中的点
	 std::deque<Ogre::Vector3> mWalkList;   // object移动点的 链表

	 Ogre::Real mWalkSpeed;                 // object移动的速度

	 Ogre::RenderTexture *rtex;
	/*
	*	OpenAL ，OggSound声音模块
	*/
#if defined __OGREOGGSOUNG__
	 bool mReplay;										// bool 声音是否播放当中
	 OgreOggSound::OgreOggSoundManager * mSoundManager;	// 指针指向sound manager
	 OgreOggSound::OgreOggISound *sound;				// 指针 sound
#endif

	 char exFlag;						//  状态标志
	/*
	*	粒子系统
	*/
	 Ogre::ParticleSystem* thrusters;	// 指针指向ParticleSystem,显示爆炸.
	/*
	*	Render to texture 照相机
	*/
	 Ogre::Camera *rttCam;				// 指向 RTT camera
};

#endif // #ifndef __TutorialApplication_h_
