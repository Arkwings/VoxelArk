#pragma once

template <unsigned int Dim> class Object;
//template <unsigned int Dim> class Object {};

class ObjectHandler2D {
    public:
    ObjectHandler2D();
    ~ObjectHandler2D();
    ObjectHandler2D(const ObjectHandler2D&) = delete;
    ObjectHandler2D& operator=(const ObjectHandler2D&) = delete;

    void SetObjectOnCamera();
    Object<2>* operator [](const unsigned int& ID) { return objects_[ID]; }
    Object<2>* GetObj(const unsigned int& ID) { return objects_[ID]; }
    const unsigned int& GetObjOnCam() const { return objOnCamera_; }
    const unsigned int& GetTransfoOnCam() const { return transfoOnCamera_; }

    private:
    std::vector<Object<2>*> objects_;
    unsigned int objOnCamera_;
    unsigned int transfoOnCamera_;
};

class ObjectHandler3D {
    public:
    ObjectHandler3D();
    ~ObjectHandler3D();
    ObjectHandler3D(const ObjectHandler3D&) = delete;
    ObjectHandler3D& operator=(const ObjectHandler3D&) = delete;

    void SetObjectOnCamera();
    Object<3>* operator [](const unsigned int& ID) { return objects_[ID]; }
    Object<3>* GetObj(const unsigned int& ID) { return objects_[ID]; }
    const unsigned int& GetObjOnCam() const { return objOnCamera_; }
    const unsigned int& GetTransfoOnCam() const { return transfoOnCamera_; }

    private:
    std::vector<Object<3>*> objects_;
    unsigned int objOnCamera_;
    unsigned int transfoOnCamera_;
};
