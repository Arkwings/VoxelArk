#include "common.hpp"
#include "objectComponents/dataComponent.hpp"
#include "objectComponents/hitboxComponent.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "object.hpp"

template <unsigned int Dim>
Object<Dim>::Object(const unsigned int& ID)
	: transObj_({ new TransformationComponent() })
	, dataObj_(nullptr)
	, hitObj_(nullptr)
	, ID_(ID)
	, drawSize_(0U)
	, instanceSize_(0U)
	, vao_(0U)
	, vbo_(0U)
	, ebo_(0U)
	, mbo_(0U)
	, transfoOnCamera_(0U)
	, selectable_(false)
	, onCamera_(false)
	, indexed_(false) {}
template Object<2>::Object(const unsigned int& ID);
template Object<3>::Object(const unsigned int& ID);

template <unsigned int Dim>
Object<Dim>::~Object() {
	glDeleteBuffers(1, &mbo_);
	if (indexed_)	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_);

	for (std::vector<TransformationComponent*>::iterator it = transObj_.begin(); it != transObj_.end(); ++it)   delete* it;
	if (hitObj_ != nullptr) delete hitObj_;
	if (dataObj_ != nullptr) delete dataObj_;
}
template Object<2>::~Object();
template Object<3>::~Object();

template <unsigned int Dim>
void Object<Dim>::DrawStatic(const unsigned int& drawPrimitives, const unsigned int& shaderID) {
	glUniform1i(glGetUniformLocation(shaderID, "dynamic"), 0);

	if (selectable_) glStencilFunc(GL_ALWAYS, dataObj_->GetID(), -1);

	glBindVertexArray(vao_);
	if (indexed_)
		glDrawElementsInstanced(drawPrimitives, drawSize_, GL_UNSIGNED_INT, 0, instanceSize_);
	else
		glDrawArraysInstanced(drawPrimitives, 0, drawSize_, instanceSize_);

	glBindVertexArray(0);
}
template void Object<2>::DrawStatic(const unsigned int& drawPrimitives, const unsigned int& shaderID);
template void Object<3>::DrawStatic(const unsigned int& drawPrimitives, const unsigned int& shaderID);

template <unsigned int Dim>
void Object<Dim>::DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID, const glm::mat4& model) {
	if (selectable_) glStencilFunc(GL_ALWAYS, dataObj_->GetID(), -1);

	glBindVertexArray(vao_);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "M"), 1, GL_FALSE, &model[0][0]);
	glUniform1i(glGetUniformLocation(shaderID, "dynamic"), 1);
	if (indexed_)
		glDrawElements(drawPrimitives, drawSize_, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(drawPrimitives, 0, drawSize_);

	glBindVertexArray(0);
}
template void Object<2>::DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID, const glm::mat4& model);
template void Object<3>::DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID, const glm::mat4& model);


template <unsigned int Dim>
void Object<Dim>::DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID) {
	if (selectable_) glStencilFunc(GL_ALWAYS, dataObj_->GetID(), -1);
	glUniform1i(glGetUniformLocation(shaderID, "dynamic"), 1);

	glBindVertexArray(vao_);
	if (indexed_)
		for (unsigned int i = 0U; i < transObjDynamic_.size(); ++i) {
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "M"), 1, GL_FALSE, &transObjDynamic_[i]->GetModel()[0][0]);
			glDrawElements(drawPrimitives, drawSize_, GL_UNSIGNED_INT, 0);
		}
	else
		for (unsigned int i = 0U; i < transObjDynamic_.size(); ++i) {
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "M"), 1, GL_FALSE, &transObjDynamic_[i]->GetModel()[0][0]);
			glDrawArrays(drawPrimitives, 0, drawSize_);
		}

	glBindVertexArray(0);
}
template void Object<2>::DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID);
template void Object<3>::DrawDynamic(const unsigned int& drawPrimitives, const unsigned int& shaderID);

template<unsigned int Dim>
void Object<Dim>::SetData(DataComponent* dataComp) {
	if (dataObj_ != nullptr) delete dataObj_;
	dataObj_ = dataComp;

	const std::vector<Vertex3D>& vertices(dataObj_->GetData());
	const std::vector<unsigned int>& indexes(dataObj_->GetIndexes());
	indexed_ = dataObj_->GetIndexed();
	drawSize_ = indexed_ ? indexes.size() : vertices.size();

	addDatas(vertices, indexes);
}
template void Object<2>::SetData(DataComponent* dataComp);
template void Object<3>::SetData(DataComponent* dataComp);

template<unsigned int Dim>
void Object<Dim>::SetStaticModels() {

	instanceSize_ = transObj_.size();

	models_ = new glm::mat4[instanceSize_];
	for (int i = 0; i < instanceSize_; ++i) {
		models_[i] = transObj_[i]->GetModel();
	}

	G_OGLThreadPool->QueueJob([&]() {

		glBindVertexArray(vao_);

		//tranform mat for instanced rendering
	glGenBuffers(1, &mbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mbo_);
	glBufferData(GL_ARRAY_BUFFER, instanceSize_ * sizeof(glm::mat4), &models_[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(10);
	glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);
	glVertexAttribDivisor(10, 1);

	glBindVertexArray(0);
		});

	while (!G_OGLThreadPool->IsIdle()) {}
}
template void Object<2>::SetStaticModels();
template void Object<3>::SetStaticModels();

template<unsigned int Dim>
void Object<Dim>::CheckOnCamera(unsigned int& transIndex) {
	unsigned int objIndex(0U);
	bool found(false);
	if (selectable_) {

		glReadPixels(G_Window->GetWidth() / 2, G_Window->GetHeight() / 2, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &objIndex);
		if (objIndex == ID_) {
			GLfloat value(std::numeric_limits<GLfloat>::max()), temp_value;
			for (int i = 0; i != transObj_.size(); ++i) {
				transObj_[i]->SetOnCamera(false);
				temp_value = transObj_[i]->GetLookingPointDistance();
				if (temp_value < value) {
					value = temp_value;
					transIndex = i;
					found = true;
				}
			}

			if (found) {
				transObj_[transIndex]->SetOnCamera(true);
				transfoOnCamera_ = transIndex;
				onCamera_ = true;
			}
			else {
				DEBUG_PRINT("CheckOnCamera: unrefered index: " << ID_);
			}
			return;
		}

		for (int i = 0; i != transObj_.size(); ++i)
			transObj_[i]->SetOnCamera(false);
		onCamera_ = false;
	}
	onCamera_ = false;
	return;
}
template void Object<2>::CheckOnCamera(unsigned int& transIndex);
template void Object<3>::CheckOnCamera(unsigned int& transIndex);

template <unsigned int Dim>
void Object<Dim>::addDatas(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indexes) {

	G_OGLThreadPool->QueueJob([&]() {
		glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	if (indexed_) {
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned int), &indexes[0], GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::POS_OFFSET);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::TEX_OFFSET);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::NORM_OFFSET);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::TAN_OFFSET);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::BITAN_OFFSET);

	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, OGL::MAX_BONE_INFLUENCE, GL_UNSIGNED_BYTE, sizeof(Vertex3D), (void*)OGL::BONEIDS_OFFSET);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, OGL::MAX_BONE_INFLUENCE, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::WEIGHTS_OFFSET);

	glBindVertexArray(0);
		});

	while (!G_OGLThreadPool->IsIdle()) {}
}
template void Object<2>::addDatas(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indexes);
template void Object<3>::addDatas(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indexes);
