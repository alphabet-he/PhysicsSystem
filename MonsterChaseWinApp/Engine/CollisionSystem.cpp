#include "CollisionSystem.h"
#include "TimeSystem.h"
#include <iostream>

namespace Engine {
	namespace CollisionSystem {

        std::vector<std::weak_ptr<GameObject>> AllCollidables;


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
            for (std::weak_ptr<GameObject> Current : AllCollidables) {
                std::shared_ptr<GameObject> temp = Current.lock();
                if (temp) {
                    CollisionComponent* collision = static_cast<CollisionComponent*>(temp->GetComponent("CollisionComponent"));
                    if (collision != nullptr) {
                        
                    }
                }

            }

            std::vector<std::weak_ptr<GameObject>>().swap(AllCollidables);
        }

        void CheckAllCollision()
        {
            for (int i = 0; i < AllCollidables.size() - 1; i++) {
                for (int j = i + 1; j < AllCollidables.size(); j++) {
                    CollisionInfo* collisionInfo = DetectCollision(AllCollidables[i], AllCollidables[j]);
                    if (collisionInfo) {
                        // there is collision
                        std::cout << "Collision!" << std::endl;
                    }
                    
                }
            }
        }

        CollisionInfo* DetectCollision(std::weak_ptr<GameObject> objA, std::weak_ptr<GameObject> objB)
        {
            // check whether the objects exist
            std::shared_ptr<GameObject> tempA = objA.lock();
            if (!tempA) {
                TrimCollidableList();
                return nullptr;
            }
            std::shared_ptr<GameObject> tempB = objB.lock();
            if (!tempB) {
                TrimCollidableList();
                return nullptr;
            }

            // make sure objA and objB both have collision
            CollisionComponent* collisionA = static_cast<CollisionComponent*>(tempA->GetComponent("CollisionComponent"));
            CollisionComponent* collisionB = static_cast<CollisionComponent*>(tempB->GetComponent("CollisionComponent"));
            if (!collisionA || !collisionB) {
                return nullptr;
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
                Engine::TimeSystem::GetFrameTime());
            if (AInB) {
                // not collided
                return nullptr;
            }

            // B In A check
            bool BInA = CalculateCloseOpenTime(collisionB, collisionA,
                MatrixBToA, VelBInA,
                t_close, t_open,
                Engine::TimeSystem::GetFrameTime());
            if (BInA) {
                // not collided
                return nullptr;
            }

            // check t_close and t_open

            // ascending
            std::sort(t_close.begin(), t_close.end());
            // ascending
            std::sort(t_open.begin(), t_open.end());
            
            if (t_close[t_close.size()-1] >= t_open[0]) {
                // no collision
                return nullptr;
            }

            else {
                // collision
                CollisionInfo ret;
                ret.objA = objA;
                ret.objB = objB;
                // find collision time
                for (float closeTime : t_close) {
                    if (closeTime < t_open[0]) {
                        ret.collisionTime = closeTime;
                        return &ret;
                    }
                }
            }
        }

        bool CalculateCloseOpenTime(CollisionComponent* currCollision, CollisionComponent* targetCollision,
            Matrix4 matrixCurrToTarget, Vector4 velInTargetCs, 
            std::vector<float>& t_close, std::vector<float>& t_open,
            float frameTime)
        {
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
            }

            float t_close_x = (BLeft - ACenterInB.x) / velInTargetCs.x;
            float t_open_x = (BRight - ACenterInB.x) / velInTargetCs.x;

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
            }

            float t_close_y = (BDown - ACenterInB.y) / velInTargetCs.y;
            float t_open_y = (BUp - ACenterInB.y) / velInTargetCs.y;

            if (t_close_y > t_open_y) {
                float temp = t_close_y;
                t_close_y = t_open_y;
                t_open_y = temp;
            }

            if (t_close_x > frameTime || t_close_y > frameTime
                || t_open_x < 0 || t_open_y < 0 )
                return true;

            

            t_close.push_back(t_close_x);
            t_close.push_back(t_close_y);
            t_open.push_back(t_open_x);
            t_open.push_back(t_open_y);


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

	}
}
