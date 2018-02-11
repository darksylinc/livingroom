
#ifndef _Demo_LivingRoomGameState_H_
#define _Demo_LivingRoomGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Ogre
{
    class InstantRadiosity;
    class IrradianceVolume;
    class ParallaxCorrectedCubemap;
}

namespace Demo
{
    class LivingRoomGameState : public TutorialGameState
    {
        Ogre::InstantRadiosity  *mInstantRadiosity;
        Ogre::IrradianceVolume  *mIrradianceVolume;
        Ogre::ParallaxCorrectedCubemap *mParallaxCorrectedCubemap;

        Ogre::SceneNode         *mSceneRoot[2];

        virtual void generateDebugText( float timeSinceLast, Ogre::String &outText );

    public:
        LivingRoomGameState( const Ogre::String &helpDescription );

        virtual void createScene01(void);

        virtual void update( float timeSinceLast );

        virtual void keyReleased( const SDL_KeyboardEvent &arg );
    };
}

#endif
