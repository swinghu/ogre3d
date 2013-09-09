/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------
Function��	Ogre�������ؿ��ͷ�ļ�
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

/* If we use OgreOggSound libs to provides sound support for the programe. */
/*	�������Ч��������OggSound���ļ�
*	�����ظ������ļ�
*/
#if defined __OGREOGGSOUNG__
#include "OgreOggSound.h"
#endif

/* Use CEGUI system to render independent GUI and deal with some graphic inputs. */
/*	���������Ҫ���CEGUIЧ��������CEGUI��OGRE��Ⱦ����ļ�
*	�����ظ������ļ�
*/
#if defined __CEGUIOGRE__
#include <CEGUISystem.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUISchemeManager.h>

	#ifdef __WOBBLLY_EFFECT__
	/*
	 *	����CEGUI��ȾЧ��ģ�����ͷ�ļ�
	 */
	#include <CEGUIRenderEffect.h>
	#include <CEGUIVertex.h>
	// �Զ���һ������̳���CEGUI::RenderEffect��
	/*	
	* CEGUI::RenderEffectΪ������,������ʵ������������д����
	*/

	class MyEffect : public CEGUI::RenderEffect
	{
	public:
		MyEffect();

		// CEGUI::RenderEffect�ӿ� 
		int getPassCount() const;

		void performPreRenderFunctions(const int pass);
		void performPostRenderFunctions();

		/* GUI��Ⱦ���̣�  
			1 ����RenderWindow��Ϣ  
			2 д��GeometryBufferͼ����Ϣ��Vecotr[6]�����������Σ��Լ�����ʲô�ģ�  
			3 GeometryBuffer Draw  
			��һ��ʵ�ʾ�����1��2֮��ʱ�ĵ���  
		*/

		bool realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry);

		//************************************
		// Method:    update
		// FullName:  MyEffect::update
		// Access:    public 
		// ����ֵ:    bool
		// ��  ��:   ÿ֡����	
		// ��������:  const float elapsed
		// ��������:  CEGUI::RenderingWindow & window
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
 
	/*	����robot��deque����robot��·��
	*	�����ظ������ļ�
	*/

#include <deque>
/*
 *	�����win32 ƽ̨��Ҫ������´���
 */
/*  #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#include "../res/resource.h"
	#endif
 */

/*
 *	�̳���BaseApplication
 */
class ITutorial01 : public BaseApplication
{
public:
	/*
	*	���캯��
	*/
    ITutorial01(void);
	/*
	*	��������
	*/
    virtual ~ITutorial01(void);
 
protected:
     virtual void createScene(void);
     virtual void createFrameListener(void);
	 // find the next location for the robot to walk to. 
	 // ��λ����������һ��λ��
     virtual bool nextLocation(void);
	 virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	 // because we need to query terrain, so choose sceneManager of type ST_EXTERIOR_CLOSE

	 virtual void chooseSceneManager(void);

	 //mouse listener
	 virtual bool mouseMoved(const OIS::MouseEvent &arg);
	 virtual bool mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
	 virtual bool mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
	/*
	*	CEGUIģ��
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
	*	���߲�ѯģ�飬��������
	*/
	 Ogre::RaySceneQuery *mRaySceneQuery;	// ���߲�ѯ
	 bool mLMouseDown, mRMouseDown;			// True if the mouse buttons are down
	 float mRotateSpeed;

	 Ogre::Real mDistance;                  // objectʣ�������ƶ��ľ���
	 Ogre::Vector3 mDirection;              // object�ƶ��ķ���
	 Ogre::Vector3 mDestination;            // object��Ҫ�ƶ�����Ŀ�ĵ�

	 Ogre::AnimationState *mAnimationState; // object�ĵ�ǰ�˶�״̬�����������ߣ���
	
	 Ogre::Entity *mEntity;                 // �˶���ʵ��
	 Ogre::SceneNode *mNode,*mSoundNode;	// ʵ�� ���ӵ������еĵ�
	 std::deque<Ogre::Vector3> mWalkList;   // object�ƶ���� ����

	 Ogre::Real mWalkSpeed;                 // object�ƶ����ٶ�

	 Ogre::RenderTexture *rtex;
	/*
	*	OpenAL ��OggSound����ģ��
	*/
#if defined __OGREOGGSOUNG__
	 bool mReplay;										// bool �����Ƿ񲥷ŵ���
	 OgreOggSound::OgreOggSoundManager * mSoundManager;	// ָ��ָ��sound manager
	 OgreOggSound::OgreOggISound *sound;				// ָ�� sound
#endif

	 char exFlag;						//  ״̬��־
	/*
	*	����ϵͳ
	*/
	 Ogre::ParticleSystem* thrusters;	// ָ��ָ��ParticleSystem,��ʾ��ը.
	/*
	*	Render to texture �����
	*/
	 Ogre::Camera *rttCam;				// ָ�� RTT camera
};

#endif // #ifndef __TutorialApplication_h_
