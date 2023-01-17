#pragma once

template <unsigned int Dim> class DataComponent;
//template <unsigned int Dim> class DataComponent {};
class HitboxComponent;
class TransformationComponent;

template <unsigned int Dim>
class Object {
    public:
    Object() = delete;
    Object(const unsigned int& ID);
    ~Object();
    void DrawStatic(const unsigned int& drawPrimitives, const unsigned int& shaderID);
    void DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID);
    void DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID, const glm::mat4& model);
    void CheckOnCamera(unsigned int& transIndex);

    void SetData(DataComponent<Dim>* dataComp);
    void SetStaticModels();
    void SetHitbox(HitboxComponent* hitComp) { if (hitObj_ != nullptr) delete hitObj_; hitObj_ = hitComp; }
    void SetSelectable(const bool& selectable) { selectable_ = selectable; }

    void AddStaticTransfo(TransformationComponent* transComp) { transObj_.push_back(transComp); }
    void AddDynamicTransfo(TransformationComponent* transComp) { transObjDynamic_.push_back(transComp); }
    void RemoveStaticTransfo(const unsigned int& transIndex) { delete transObj_[transIndex];  transObj_.erase(transObj_.begin() + transIndex); }
    void RemoveDynamicTransfo(const unsigned int& transIndex) { delete transObjDynamic_[transIndex];  transObjDynamic_.erase(transObjDynamic_.begin() + transIndex); }

    DataComponent<Dim>* GetData() const { return dataObj_; }
    HitboxComponent* GetHitbox() const { return hitObj_; }
    TransformationComponent* GetTransfoStatic(const unsigned int& transfoID) const { return transObj_[transfoID]; }
    TransformationComponent* GetTransfoDynamic(const unsigned int& transfoID) const { return transObjDynamic_[transfoID]; }
    TransformationComponent* GetTransfoOnCamera() const { return onCamera_ == 1 ? transObj_[transfoOnCamera_] : onCamera_ == 0 ? nullptr : transObjDynamic_[transfoOnCamera_]; }

    const bool& IsSelectable() const { return selectable_; }
    const bool& IsOnCamera() const { return onCamera_; }
    const bool& IsIndexed() const { return indexed_; }
    private:

    void addDatas(const std::vector<Vertex<Dim>>& vertices, const std::vector<unsigned int>& indexes);

    std::vector<TransformationComponent*> transObj_;
    std::vector<TransformationComponent*> transObjDynamic_;

    DataComponent<Dim>* dataObj_;
    HitboxComponent* hitObj_;

    glm::mat4* models_;

    unsigned int ID_;
    unsigned int drawSize_;
    unsigned int instanceSize_;
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
    unsigned int mbo_;
    unsigned int transfoOnCamera_;

    bool selectable_;
    int onCamera_;
    bool indexed_;
};
