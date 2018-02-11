
#include "LivingRoomGameState.h"
#include "CameraController.h"
#include "GraphicsSystem.h"

#include "OgreSceneManager.h"
#include "OgreItem.h"

#include "OgreMeshManager.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2.h"

#include "OgreCamera.h"
#include "OgreRenderWindow.h"

#include "OgreHlmsPbsDatablock.h"
#include "OgreHlmsSamplerblock.h"

#include "OgreRoot.h"
#include "OgreHlmsManager.h"
#include "OgreHlmsTextureManager.h"
#include "OgreHlmsPbs.h"

#include "OgreSceneFormatImporter.h"
#include "OgreForwardPlusBase.h"

#include "Cubemaps/OgreParallaxCorrectedCubemap.h"
#include "InstantRadiosity/OgreInstantRadiosity.h"
#include "OgreIrradianceVolume.h"

#include "Utils/HdrUtils.h"

using namespace Demo;

namespace Demo
{
    LivingRoomGameState::LivingRoomGameState( const Ogre::String &helpDescription ) :
        TutorialGameState( helpDescription ),
        mInstantRadiosity( 0 ),
        mIrradianceVolume( 0 ),
        mParallaxCorrectedCubemap( 0 )
	{
    }
    //-----------------------------------------------------------------------------------
    void LivingRoomGameState::createScene01(void)
    {
        HdrUtils::init( (Ogre::uint8)mGraphicsSystem->getRenderWindow()->getFSAA() );

        mCameraController = new CameraController( mGraphicsSystem, false );

        Ogre::SceneManager *sceneManager = mGraphicsSystem->getSceneManager();
        sceneManager->setForwardClustered( true, 16, 8, 24, 96, 2, 50 );
        sceneManager->getForwardPlus()->setEnableVpls( true );

        for( size_t i=0; i<Ogre::NUM_SCENE_MEMORY_MANAGER_TYPES; ++i )
        {
            mSceneRoot[i] = sceneManager->getRootSceneNode()->
                            createChildSceneNode( static_cast<Ogre::SceneMemoryMgrTypes>(i) );
        }

        for( size_t i=0; i<Ogre::NUM_SCENE_MEMORY_MANAGER_TYPES; ++i )
            mSceneRoot[i]->pitch( Ogre::Radian( -Ogre::Math::HALF_PI ) );

        Ogre::Matrix4 fullPccTransform = mSceneRoot[0]->_getFullTransformUpdated();

        Ogre::SceneFormatImporter importer( mGraphicsSystem->getRoot(),
                                            mGraphicsSystem->getSceneManager(),
                                            Ogre::BLANKSTRING );
        importer.setRootNodes( mSceneRoot[Ogre::SCENE_DYNAMIC], mSceneRoot[Ogre::SCENE_STATIC] );
        importer.setSceneComponentTransform( fullPccTransform );
        importer.importSceneFromFile( mGraphicsSystem->getResourcePath() + "living_room.scenefolder" );
        importer.getInstantRadiosity( true, &mInstantRadiosity, &mIrradianceVolume );
        mParallaxCorrectedCubemap = importer.getParallaxCorrectedCubemap( true );

        //The import process resetted this value, we need to set it again.
        for( size_t i=0; i<Ogre::NUM_SCENE_MEMORY_MANAGER_TYPES; ++i )
            mSceneRoot[i]->pitch( Ogre::Radian( -Ogre::Math::HALF_PI ) );

        //Hide the proxy geometry created by PCC.
        sceneManager->setVisibilityMask( ~mParallaxCorrectedCubemap->getProxyReservedVisibilityMask() );

        const float exposure = 0.83f;
        const float minAutoExposure = -1.0f;
        const float maxAutoExposure = 1.0f;
        const float bloomThreshold = 6.85f;
        HdrUtils::setExposure( exposure, minAutoExposure, maxAutoExposure );
        HdrUtils::setBloomThreshold( Ogre::max( bloomThreshold - 2.0f, 0.0f ),
                                     Ogre::max( bloomThreshold, 0.01f ) );

        TutorialGameState::createScene01();
    }
    //-----------------------------------------------------------------------------------
    void LivingRoomGameState::update( float timeSinceLast )
	{
        TutorialGameState::update( timeSinceLast );
    }
    //-----------------------------------------------------------------------------------
    void LivingRoomGameState::generateDebugText( float timeSinceLast, Ogre::String &outText )
	{
		TutorialGameState::generateDebugText( timeSinceLast, outText );
	}
    //-----------------------------------------------------------------------------------
    void LivingRoomGameState::keyReleased( const SDL_KeyboardEvent &arg )
    {
        if( (arg.keysym.mod & ~(KMOD_NUM|KMOD_CAPS)) != 0 )
        {
            TutorialGameState::keyReleased( arg );
            return;
        }

		TutorialGameState::keyReleased( arg );
    }
}
