#include "CollisionSystem.h"
#include "TimeSystem.h"
#include "MovableSystem.h"
#include <iostream>

namespace Engine {
	namespace CollisionSystem {

        std::vector<std::weak_ptr<GameObject>> AllCollidables;

        float MaxProcessingTime = 2.0f;

        void Init() {
            GameObjectFactory::RegisterComponentCreatorFunc("collision", CreateCollisionFromJSON);
        }

        void CreateCollisionFromJSON(std::shared_ptr<GameObject> gameObject, nlohmann::json& jsonData)
        {
            //assert(jsonData["sprite_texture_source"].is_string());
            //std::string TextureSource = jsonData["sprite_texture_source"];
            //CreateCollisionComponent(gameObject, TextureSource.c_str());
            assert(jsonData["center"].is_array());
            Point2D center = Point2D{ jsonData["center"][0], jsonData["center"][1] };
            assert(jsonData["extents"].is_array());
            Point2D extents = Point2D{ jsonData["extents"][0], jsonData["extents"][1] };
            CreateCollisionComponent(gameObject, center, extents);
        };

        void CreateCollisionComponent(std::shared_ptr<GameObject> go, Point2D center, Point2D extents)
        {
            //RenderComponent* render = new RenderComponent;
            //render->Sprite = Engine::RenderSystem::CreateSprite(pFilename);
            //go->AddComponent("RenderComponent", render);

            CollisionComponent* collision = new CollisionComponent;
            collision->center = center;
            collision->extents = extents;
            go->AddComponent("CollisionComponent", collision);
            AllCollidables.push_back(go);
        }

        void ReleaseCollidableList() {
            std::vector<std::weak_ptr<GameObject>>().swap(AllCollidables);
        }

        void CheckAllCollision(_LARGE_INTEGER currTime)
        {
            _LARGE_INTEGER TimeStartProcessing, CurrProcessingTime, ElapsedProcessingTime;
            QueryPerformanceCounter(&TimeStartProcessing);

            _LARGE_INTEGER FrameTime;
            FrameTime.QuadPart = currTime.QuadPart - Engine::TimeSystem::GetLastRecordedTime().QuadPart;
            float frameSeconds = static_cast<float>(FrameTime.QuadPart) / Engine::TimeSystem::GetFrequency().QuadPart;

            
            float calculatedTimeInFrame = 0;
            bool moved = false;
            // loop the checking collision process,
            // until there is no collision found,
            // or the processing time is too long
            while (true) {

                CollisionInfo EarliestCollision;
                float earliestCollisionTime = 100.0f;

                for (int i = 0; i < AllCollidables.size() - 1; i++) {
                    for (int j = i + 1; j < AllCollidables.size(); j++) {

                        CollisionInfo collisionInfo = DetectCollision(AllCollidables[i], AllCollidables[j], calculatedTimeInFrame, frameSeconds);

                        // if there is collision
                        if (collisionInfo.collisionTime > -1.0f) {

                            // if the collision is earlier than recorded
                            if (collisionInfo.collisionTime < earliestCollisionTime) {
                                EarliestCollision.copy(collisionInfo);
                                earliestCollisionTime = collisionInfo.collisionTime;
                            }

                        }

                    }
                }

                // if there is collision found
                if (EarliestCollision.collisionTime > -1.0f) {

                    moved = true;
                   
                    /*
                    * process to the earliest collision
                    */

                    // move everything to the earliest collided time
                    for (int i = 0; i < AllCollidables.size(); i++) {
                        MovableSystem::Move(AllCollidables[i], earliestCollisionTime);
                    }
                    // set calculated time
                    calculatedTimeInFrame += earliestCollisionTime;
                    
                    // calculate velocity
                    UpdateVelocityAfterCollision(EarliestCollision.objA, EarliestCollision.objB);

                    // check whether we spent too much time processing the collision
                    QueryPerformanceCounter(&CurrProcessingTime);
                    ElapsedProcessingTime.QuadPart = CurrProcessingTime.QuadPart - TimeStartProcessing.QuadPart;
                    float ElapsedSeconds = static_cast<float>(ElapsedProcessingTime.QuadPart) / Engine::TimeSystem::GetFrequency().QuadPart;
                    if (ElapsedSeconds >= MaxProcessingTime) {
                        break;
                    }
                }

                // if there is NO collision found in left seconds of the frame
                else {

                    // if it is first check
                    if (!moved) {
                        // do nothing,
                        // let the movable system deal with all them
                        // by UpdateAll()
                    }
                    // if it is not the first check
                    // in other words, the objects have moved already
                    else {
                        for (int i = 0; i < AllCollidables.size(); i++) {
                            float secondsLeftInFrame = frameSeconds - calculatedTimeInFrame;
                            MovableSystem::Move(AllCollidables[i], secondsLeftInFrame);
                        }
                    }
                    

                    break;
                }

                
            }
            

        }

        CollisionInfo DetectCollision(std::weak_ptr<GameObject> objA, std::weak_ptr<GameObject> objB,
            float currTimeInFrame, float frameEndTime)
        {
            CollisionInfo ret;
            // check whether the objects exist
            std::shared_ptr<GameObject> tempA = objA.lock();
            if (!tempA) {
                TrimCollidableList();
                return ret;
            }
            std::shared_ptr<GameObject> tempB = objB.lock();
            if (!tempB) {
                TrimCollidableList();
                return ret;
            }

            // make sure objA and objB both have collision
            CollisionComponent* collisionA = static_cast<CollisionComponent*>(tempA->GetComponent("CollisionComponent"));
            CollisionComponent* collisionB = static_cast<CollisionComponent*>(tempB->GetComponent("CollisionComponent"));
            if (!collisionA || !collisionB) {
                return ret;
            }

            // check whether two objects are too far away
            if (Point2D::GetDist(tempA->Position, tempB->Position) >
                std::fmax(350.0f,
                    (collisionA->extents.Length() + collisionB->extents.Length()) * 3
                )
                ) {
                return ret;
            }

            /*
            * prepare all the values
            */
            // matrices
            
            // M_AToWorld
            Matrix4 MatrixAToWorld = Matrix4::CreateTranslationCV(Vector3(tempA->Position, 0)) * Matrix4::CreateZRotationCV(tempA->Rotation.z);
            // M_WorldToA
            Matrix4 MatrixWorldToA = MatrixAToWorld.GetInverseRotTransCV();
            // M_BToWorld
            Matrix4 MatrixBToWorld = Matrix4::CreateTranslationCV(Vector3(tempB->Position, 0)) * Matrix4::CreateZRotationCV(tempB->Rotation.z);
            // M_WorldToB
            Matrix4 MatrixWorldToB = MatrixBToWorld.GetInverseRotTransCV();
            // M_AToB
            Matrix4 MatrixAToB = MatrixWorldToB * MatrixAToWorld;
            // M_BToA
            Matrix4 MatrixBToA; 
            MatrixAToB.GetInverse(MatrixBToA);

            // velocity
            
            // World
            Point2D VelocityA = GetVelocity(tempA);
            Point2D VelocityB = GetVelocity(tempB);
            // Relative
            Point2D VelocityAToB = VelocityA - VelocityB;
            Point2D VelocityBToA = VelocityB - VelocityA;
            // A In B
            Vector4 VelAInB = MatrixWorldToB * Vector4(VelocityAToB, 0.0f, 0.0f);
            // B In A
            Vector4 VelBInA = MatrixWorldToA * Vector4(VelocityBToA, 0.0f, 0.0f);

            // release shared ptr
            tempA = nullptr;
            tempB = nullptr;

            /*
            * check
            */
            std::vector<float> t_close;
            std::vector<float> t_open;

            // A In B check
            bool AInB = CalculateCloseOpenTime(collisionA, collisionB,
                MatrixAToB, VelAInB,
                t_close, t_open,
                frameEndTime - currTimeInFrame);
            if (AInB) {
                // not collided
                return ret;
            }

            // B In A check

            bool BInA = CalculateCloseOpenTime(collisionB, collisionA,
                MatrixBToA, VelBInA,
                t_close, t_open,
                frameEndTime - currTimeInFrame);
            if (BInA) {
                // not collided
                return ret;
            }

            // check t_close and t_open

            // ascending
            std::sort(t_close.begin(), t_close.end());
            // ascending
            std::sort(t_open.begin(), t_open.end());
            
            if (t_close[t_close.size()-1] >= t_open[0]) {
                // no collision
                return ret;
            }

            else {
                // collision
                
                ret.objA = objA;
                ret.objB = objB;
                // find collision time
                for (float closeTime : t_close) {
                    if (closeTime < t_open[0]) {
                        ret.collisionTime = closeTime;
                        return ret;
                    }
                }
            }
        }

        bool CalculateCloseOpenTime(CollisionComponent* currCollision, CollisionComponent* targetCollision,
            Matrix4 matrixCurrToTarget, Vector4 velInTargetCs, 
            std::vector<float>& t_close, std::vector<float>& t_open,
            float leftFrameTime)
        {

            float t_open_x, t_open_y, t_close_x, t_close_y;

            Vector4 AExtentsXInB = matrixCurrToTarget * Vector4(currCollision->extents.x, 0.0f, 0.0f, 0.0f);
            Vector4 AExtentsYInB = matrixCurrToTarget * Vector4(0.0f, currCollision->extents.y, 0.0f, 0.0f);

            // A In B
            Vector4 ACenterInB = matrixCurrToTarget * Vector4(currCollision->center, 0.0f, 1.0f);

            // x axis
            float AProjectionOntoB_X = fabs(AExtentsXInB.x) + fabs(AExtentsYInB.x);

            float BVirtualExtents_X = targetCollision->extents.x + AProjectionOntoB_X;
            float BLeft = targetCollision->center.x - BVirtualExtents_X;
            float BRight = targetCollision->center.x + BVirtualExtents_X;

            if (velInTargetCs.x == 0) {
                if (ACenterInB.x < BLeft || ACenterInB.x > BRight) {
                    return true;
                }
                else {
                    t_close_x = 0;
                    t_open_x = leftFrameTime;
                }
            }
            else {
                t_close_x = (BLeft - ACenterInB.x) / velInTargetCs.x;
                t_open_x = (BRight - ACenterInB.x) / velInTargetCs.x;
            }
            

            if (t_close_x > t_open_x) {
                float temp = t_close_x;
                t_close_x = t_open_x;
                t_open_x = temp;
            }

            // y axis

            float AProjectionOntoB_Y = fabs(AExtentsXInB.y) + fabs(AExtentsYInB.y);

            float BVirtualExtents_Y = targetCollision->extents.y + AProjectionOntoB_Y;
            float BDown = targetCollision->center.y - BVirtualExtents_Y;
            float BUp = targetCollision->center.y + BVirtualExtents_Y;

            if (velInTargetCs.y == 0) {
                if (ACenterInB.y < BDown || ACenterInB.y > BUp) {
                    return true;
                }
                else {
                    t_close_y = 0;
                    t_open_y = leftFrameTime;
                }
            }
            else {
                t_close_y = (BDown - ACenterInB.y) / velInTargetCs.y;
                t_open_y = (BUp - ACenterInB.y) / velInTargetCs.y;
            }
            

            if (t_close_y > t_open_y) {
                float temp = t_close_y;
                t_close_y = t_open_y;
                t_open_y = temp;
            }

            if (t_close_x >= leftFrameTime || t_close_y >= leftFrameTime
                || t_open_x <= 0 || t_open_y <= 0 )
                return true;

            

            t_close.push_back(std::fmax(t_close_x, 0.0f));
            t_close.push_back(std::fmax(t_close_y, 0.0f));
            t_open.push_back(std::fmin(t_open_x, leftFrameTime));
            t_open.push_back(std::fmin(t_open_y, leftFrameTime));


            return false;
        }

        Point2D GetVelocity(std::shared_ptr<GameObject> obj)
        {
            MovableComponent* movable = static_cast<MovableComponent*>(obj->GetComponent("MovableComponent"));
            if (movable) {
                return movable->Velocity;
            }
            return Point2D{ 0, 0 };
        }

        void TrimCollidableList()
        {
            for (auto it = AllCollidables.begin(); it != AllCollidables.end(); ) {
                std::shared_ptr<GameObject> temp = it->lock();
                if (temp) {
                    ++it;
                }
                else {
                    it = AllCollidables.erase(it);
                }
            }
        }

        void UpdateVelocityAfterCollision(std::weak_ptr<GameObject> objA, std::weak_ptr<GameObject> objB)
        {
            // get all values
            std::shared_ptr<GameObject> tempA = objA.lock();
            std::shared_ptr<GameObject> tempB = objB.lock();

            PhysicsComponent* physicsA = static_cast<PhysicsComponent*>(tempA->GetComponent("PhysicsComponent"));
            MovableComponent* movableA = static_cast<MovableComponent*>(tempA->GetComponent("MovableComponent"));
            PhysicsComponent* physicsB = static_cast<PhysicsComponent*>(tempB->GetComponent("PhysicsComponent"));
            MovableComponent* movableB = static_cast<MovableComponent*>(tempB->GetComponent("MovableComponent"));
            assert(physicsA && movableA && physicsB && movableB);
            float massA = physicsA->Mass;
            float massB = physicsB->Mass;
            Point2D velA = movableA->Velocity;
            Point2D velB = movableB->Velocity;

            // calculation
            Point2D velANew = velA * (massA - massB) / (massA + massB) + velB * (2 * massB) / (massA + massB);
            Point2D velBNew = velB * (massB - massA) / (massA + massB) + velA * (2 * massA) / (massA + massB);

            // update velocity
            movableA->Velocity = velANew;
            movableB->Velocity = velBNew;
        }

	}
}
