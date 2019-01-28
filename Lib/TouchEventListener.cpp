#include "TouchEventListener.h"
#include "OVR.h"


namespace kvs
{

namespace oculus
{

TouchEventListener::TouchEventListener( kvs::oculus::Screen* screen ):
    kvs::oculus::EventListener( screen ),
    m_is_grabbed( false ),
    m_both_is_grabbed( false ),
    m_touch_distance( 0.0f ),
    m_rotation_factor( 5000.0f ),
    m_translation_factor( 5000.0f ),
    m_scaling_factor( 30000.0f )
{
    if ( !controller().hasRight() ) { std::cout << "Oculus Touch (R): Disable" << std::endl; }
    if ( !controller().hasLeft() ) { std::cout << "Oculus Touch (L): Disable" << std::endl; }
}

void TouchEventListener::initializeEvent()
{
    std::cout << "TouchEventListener::initializeEvent()" << std::endl;
    if ( !controller().hasRight() ) { std::cout << "Oculus Touch (R): Disable" << std::endl; }
    if ( !controller().hasLeft() ) { std::cout << "Oculus Touch (L): Disable" << std::endl; }
}

void TouchEventListener::frameEvent()
{
    std::cout << "TouchEventListener::frameEvent()" << std::endl;
    if ( !controller().hasRight() ) { std::cout << "Oculus Touch (R): Disable" << std::endl; }
    if ( !controller().hasLeft() ) { std::cout << "Oculus Touch (L): Disable" << std::endl; }

    const ovrTrackingState ts = controller().trackingState( 0 );
    ovrPosef hands[2] = { ts.HandPoses[ovrHand_Left].ThePose, ts.HandPoses[ovrHand_Right].ThePose };

    std::cout << "  Position (R): " << kvs::oculus::ToVec3( hands[ovrHand_Left].Position ) << std::endl;
    std::cout << "  Position (L): " << kvs::oculus::ToVec3( hands[ovrHand_Right].Position ) << std::endl;

    ovrInputState input_state = controller().inputState( ovrControllerType_Touch );
    if ( input_state.Buttons & ovrButton_A ) { std::cout << "Pressed A" << std::endl; }
    if ( input_state.Buttons & ovrButton_B ) { std::cout << "Pressed B" << std::endl; }
    if ( input_state.Buttons & ovrButton_X ) { std::cout << "Pressed X" << std::endl; }
    if ( input_state.Buttons & ovrButton_Y ) { std::cout << "Pressed Y" << std::endl; }
    if ( input_state.HandTrigger[ovrHand_Left] > 0.5f ) { std::cout << "Grip Left HandTrigger" << std::endl; }
    if ( input_state.HandTrigger[ovrHand_Right] > 0.5f ) { std::cout << "Grip Right HandTrigger" << std::endl; }
    if ( input_state.IndexTrigger[ovrHand_Left] > 0.5f ) { std::cout << "Grip Left IndexTrigger" << std::endl; }
    if ( input_state.IndexTrigger[ovrHand_Right] > 0.5f ) { std::cout << "Grip Right IndexTrigger" << std::endl; }

    {
        const kvs::Vec3 right_p = kvs::oculus::ToVec3( hands[ovrHand_Right].Position );
        const kvs::Vec2i right_m( kvs::Vec2( right_p.x(), -right_p.y() ) * m_rotation_factor );

        const kvs::Vec3 left_p = kvs::oculus::ToVec3( hands[ovrHand_Left].Position );
        const kvs::Vec2i left_m( kvs::Vec2( left_p.x(), -left_p.y() ) * m_translation_factor );

        const bool right_grabbed = input_state.HandTrigger[ovrHand_Right] > 0.5;
        const bool left_grabbed = input_state.HandTrigger[ovrHand_Left] > 0.5;

        // Rotation.
        if ( right_grabbed && !left_grabbed )
        {
            if ( m_is_grabbed )
            {
                scene()->mouseMoveFunction( right_m.x(), right_m.y() );
            }
            else
            {
                m_is_grabbed = true;
                kvs::Mouse::OperationMode mode = kvs::Mouse::Rotation;
                scene()->mousePressFunction( right_m.x(), right_m.y(), mode );

                m_both_is_grabbed = false;
                if ( scene()->mouse()->operationMode() == kvs::Mouse::Scaling )
                {
                    m_touch_distance = 0.0f;
                }
            }
        }

        // Translation.
        else if ( left_grabbed && !right_grabbed )
        {
            if ( m_is_grabbed )
            {
                scene()->mouseMoveFunction( left_m.x(), left_m.y() );
            }
            else
            {
                m_is_grabbed = true;
                kvs::Mouse::OperationMode mode = kvs::Mouse::Translation;
                scene()->mousePressFunction( left_m.x(), left_m.y(), mode );

                m_both_is_grabbed = false;
                if ( scene()->mouse()->operationMode() == kvs::Mouse::Scaling )
                {
                    m_touch_distance = 0.0f;
                }
            }
        }

        // Scaling.
        else if ( right_grabbed && left_grabbed )
        {
            if ( m_both_is_grabbed )
            {
                const double touch_distance = ( right_p - left_p ).length();
                const double d = touch_distance - m_touch_distance;
                scene()->wheelFunction( static_cast<int>( d * m_scaling_factor ) );
                scene()->updateXform();
                m_touch_distance = static_cast<float>( touch_distance );
            }
            else
            {
                m_both_is_grabbed = true;
                m_touch_distance = static_cast<float>( ( right_p - left_p ).length() );

                m_is_grabbed = false;
                if ( scene()->mouse()->operationMode() == kvs::Mouse::Rotation )
                {
                    scene()->mouseReleaseFunction( right_m.x(), right_m.y() );
                }
                else if ( scene()->mouse()->operationMode() == kvs::Mouse::Translation )
                {
                    scene()->mouseReleaseFunction( left_m.x(), left_m.y() );
                }
            }
        }

        // Released.
        else
        {
            m_is_grabbed = false;
            m_both_is_grabbed = false;
            if ( scene()->mouse()->operationMode() == kvs::Mouse::Rotation )
            {
                scene()->mouseReleaseFunction( right_m.x(), right_m.y() );
            }
            else if ( scene()->mouse()->operationMode() == kvs::Mouse::Translation )
            {
                scene()->mouseReleaseFunction( left_m.x(), left_m.y() );
            }
            else if ( scene()->mouse()->operationMode() == kvs::Mouse::Scaling )
            {
                m_touch_distance = 0.0f;
            }
        }
    }
}

} // end of namespace oculus

} // end of namespace kvs
