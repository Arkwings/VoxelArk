#include "common.hpp"
#include "algorithms/equation.hpp"
#include "objectComponents/dataComponent.hpp"
#include "objectComponents/hitboxComponent.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "handlers/objectHandler.hpp"

ObjectHandler2D::ObjectHandler2D()
    : objOnCamera_(OBJ2D::NULLOBJ)
    , transfoOnCamera_(-1)
    , objects_(256, nullptr) {

    objects_[OBJ2D::NULLOBJ] = new Object<2>(OBJ2D::NULLOBJ);
    objects_[OBJ2D::POINTER] = new Object<2>(OBJ2D::POINTER);

    objects_[OBJ2D::POINTER]->SetData(new Pointer2DDataComponent());

    for (unsigned int i = 0; i < objects_.size(); ++i) {
        if (objects_[i] != nullptr && objects_[i]->GetData() != nullptr)  objects_[i]->SetStaticModels();
    }
}

ObjectHandler2D::~ObjectHandler2D() {
    for (std::vector<Object<2>*>::iterator it = objects_.begin(); it != objects_.end(); ++it) {
        if (*it != nullptr)   delete* it;
    }
}

void ObjectHandler2D::SetObjectOnCamera() {
    //not implemente yet
}

ObjectHandler3D::ObjectHandler3D()
    : objOnCamera_(OBJ3D::NULLOBJ)
    , transfoOnCamera_(-1)
    , objects_(256, nullptr) {
    //insertion order MUST be the same than definition value
    objects_[OBJ3D::NULLOBJ] = new Object<3>(OBJ3D::NULLOBJ);
    objects_[OBJ3D::SQUARE] = new Object<3>(OBJ3D::SQUARE);
    objects_[OBJ3D::CIRCLE] = new Object<3>(OBJ3D::CIRCLE);
    objects_[OBJ3D::CUBE] = new Object<3>(OBJ3D::CUBE);
    objects_[OBJ3D::SPHERE] = new Object<3>(OBJ3D::SPHERE);
    objects_[OBJ3D::CUBESPHERE] = new Object<3>(OBJ3D::CUBESPHERE);

    objects_[OBJ3D::SQUARE]->SetData(new SquareDataComponent(0));
    objects_[OBJ3D::CIRCLE]->SetData(new CircleDataComponent(15, 0));
    objects_[OBJ3D::CUBE]->SetData(new CubeDataComponent(0, false));
    objects_[OBJ3D::SPHERE]->SetData(new SphereDataComponent(16, 8, 0, false));
    objects_[OBJ3D::CUBESPHERE]->SetData(new CubeDataComponent(4, true));

    objects_[OBJ3D::SQUARE]->SetSelectable(true);
    objects_[OBJ3D::CIRCLE]->SetSelectable(true);
    objects_[OBJ3D::CUBE]->SetSelectable(true);
    objects_[OBJ3D::SPHERE]->SetSelectable(true);
    objects_[OBJ3D::CUBESPHERE]->SetSelectable(true);

    objects_[OBJ3D::SQUARE]->AddDynamicTransfo(new TransformationComponent(glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 2.0f, 0.0f)));
    objects_[OBJ3D::CUBE]->AddDynamicTransfo(new TransformationComponent(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    objects_[OBJ3D::SPHERE]->AddDynamicTransfo(new TransformationComponent(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-14.0f, 0.0f, 0.0f)));
    objects_[OBJ3D::SPHERE]->AddDynamicTransfo(new TransformationComponent(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-15.0f, 0.0f, 0.0f)));
    objects_[OBJ3D::SPHERE]->GetTransfoDynamic(0)->SetScale(glm::vec3(0.5f, 1.0f, 2.0f));
    objects_[OBJ3D::SPHERE]->GetTransfoDynamic(0)->SetTranslate(glm::vec3(-13.0, 0.0, 10.0));
    objects_[OBJ3D::SPHERE]->GetTransfoDynamic(1)->SetTranslate(glm::vec3(-8.0, 0.0, 10.0));
    objects_[OBJ3D::SPHERE]->SetHitbox(new SphereHitboxComponent(new Equation<Eq::Circle>()));

    objects_[OBJ2D::POINTER]->AddStaticTransfo(new TransformationComponent(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

    for (unsigned int i = 0; i < objects_.size(); ++i) {
        if (objects_[i] != nullptr && objects_[i]->GetData() != nullptr)  objects_[i]->SetStaticModels();
    }
}

ObjectHandler3D::~ObjectHandler3D() {
    for (std::vector<Object<3>*>::iterator it = objects_.begin(); it != objects_.end(); ++it) {
        if (*it != nullptr)   delete* it;
    }
}

void ObjectHandler3D::SetObjectOnCamera() {
    if (G_CameraHandler->GetActive()->getLookingDistance() < OPT::LOOKING_DISTANCE) {
        unsigned int result = std::numeric_limits<unsigned int>::max();
        for (unsigned int i = 0; i != objects_.size(); ++i) {
            if (objects_[i] != nullptr) {
                objects_[i]->CheckOnCamera(result);
                if (result != std::numeric_limits<unsigned int>::max()) {
                    objOnCamera_ = i;
                    transfoOnCamera_ = result;
                    return;
                }
            }
        }
    }
    objOnCamera_ = OBJ3D::NULLOBJ;
}