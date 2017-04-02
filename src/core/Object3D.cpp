//
//  Object3D.cpp
//  iPhotoWall
//
//  Created by Hesam Ohadi on 15/11/15.
//
//

#include "Object3D.h"
#include "Scene.h"

using namespace ci;
using namespace std;

Object3D::Object3D() : mModel(Model()), mModelWorld(Model()), mWorldUp(vec3(0.0, 1.0, 0.0)), mViewDirection(vec3(0.0, 0.0, -1.0)), mType("object_3d"), mName(""), mId(0), mGuid(GuidGenerator().newGuid()), mIsVisible(true), mShouldLookAtTargetObject(false) ,mIsActive(true), mDoesCastShadow(true), mDoesReceiveShadow(true), mShouldUpdate(true), mShouldUpdateSelf(true), mShouldNotifyTopParent(false)
{
    addTag("object_3d");
    mName = "object_3d";
}


void Object3D::drawSelf()
{
    
}

void Object3D::updateSelf()
{
    
}

void Object3D::onUpdate()
{
    
}

void Object3D::onUpdateSelf()
{
    setShouldUpdateSelf(true);
}

void Object3D::draw()
{
    if (mIsActive){
        drawChildren();
        if (mIsVisible){
            drawSelf();
            onDrawSelf();
        }
        onDraw();
    }
}

void Object3D::update()
{
    if (mIsActive){
        
        if (mShouldUpdate){
            
            //update mModelWorld
            if (auto parent = mParent.lock()){
                mModelWorld.setModelMatrix(parent->getModelWorld().getModelMatrix() * mModel.getModelMatrix());
            }
            else {
                mModelWorld.setModelMatrix(mModel.getModelMatrix());
            }
            const auto& worldMat = getModelWorld().getModelMatrix();
            mViewDirection = normalize(vec3(worldMat[0][2], worldMat[1][2], worldMat[2][2]));
            setShouldUpdate(false);
            onUpdate();
        }
        
        updateChildren();
        if (mShouldUpdateSelf) {
            updateSelf();
            setShouldUpdateSelf(false);
            onUpdateSelf();
        }
        
        
        if (mShouldLookAtTargetObject){
            if (auto targetObject = mTargetObject.lock()){
                lookAt(targetObject->getModelWorld().getModelTranslation());
            }
        }
        
    }
}

void Object3D::setup()
{
    onSetup();
    setupSelf();
    onSetupSelf();
}

void Object3D::drawChildren()
{
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        (*iter)->draw();
    }
}

void Object3D::updateChildren()
{
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        (*iter)->update();
    }
}

//convinience methods for transforming the model

Object3D& Object3D::translate(const vec3 &aTranslation)
{
    vec3 translation = mModel.getModelTranslation();
    translation += aTranslation;
    mModel.setModelTranslation(translation);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::rotate(const quat &aRotation)
{
    quat orientation = mModel.getModelOrientation();
    orientation *= aRotation;
    mModel.setModelOrientation(orientation);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::scale(const vec3 &aScale)
{
    vec3 scale = mModel.getModelScale();
    scale *= aScale;
    mModel.setModelScale(scale);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::rotateX(float aAngle)
{
    quat orientation = mModel.getModelOrientation();
    orientation *= glm::rotate(quat(), toRadians(aAngle), vec3(1.0, 0.0, 0.0));
    mModel.setModelOrientation(orientation);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::rotateY(float aAngle)
{
    quat orientation = mModel.getModelOrientation();
    orientation *= glm::rotate(quat(), toRadians(aAngle), vec3(0.0, 1.0, 0.0));
    mModel.setModelOrientation(orientation);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::rotateZ(float aAngle)
{
    quat orientation = mModel.getModelOrientation();
    orientation *= glm::rotate(quat(), toRadians(aAngle), vec3(0.0, 0.0, 1.0));
    mModel.setModelOrientation(orientation);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::setTranslation(const vec3 &aTranslation)
{
    mModel.setModelTranslation(aTranslation);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::setRotation(const quat &aRotation)
{
    mModel.setModelOrientation(aRotation);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::setScale(const vec3 &aScale)
{
    mModel.setModelScale(aScale);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::setRotationX(float aAngle)
{
    vec3 rotation = glm::eulerAngles(mModel.getModelOrientation());
    rotation.x = toRadians(aAngle);
    mModel.setModelOrientation(quat(rotation));
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::setRotationY(float aAngle)
{
    vec3 rotation = glm::eulerAngles(mModel.getModelOrientation());
    rotation.y = toRadians(aAngle);
    mModel.setModelOrientation(quat(rotation));
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::setRotationZ(float aAngle)
{
    vec3 rotation = glm::eulerAngles(mModel.getModelOrientation());
    rotation.z = toRadians(aAngle);
    mModel.setModelOrientation(quat(rotation));
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::lookAt(const vec3 &aTarget)
{
    mViewDirection = normalize( aTarget - mModelWorld.getModelTranslation() );
    quat orientation = glm::toQuat( alignZAxisWithTarget( -mViewDirection, mWorldUp ) );
    vec3 mW = - normalize( mViewDirection );
    vec3 mU = glm::rotate( orientation, vec3( 1, 0, 0 ) );
    vec3 mV = glm::rotate( orientation, vec3( 0, 1, 0 ) );
    vec3 trans= mModel.getModelTranslation();
    vec3 scale= mModel.getModelScale();
    mat4 m;
    m[0][0] = mU.x * scale.x; m[1][0] = mU.y; m[2][0] = mU.z; m[3][0] =  trans.x;
    m[0][1] = mV.x; m[1][1] = mV.y * scale.y; m[2][1] = mV.z; m[3][1] =  trans.y;
    m[0][2] = mW.x; m[1][2] = mW.y; m[2][2] = mW.z * scale.z; m[3][2] =  trans.z;
    m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f; m[3][3] =  1.0f;
    mModel.setModelMatrix(m);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::lookAt(const vec3 &aEyePoint, const vec3 &aTarget)
{
    translate(aEyePoint - getModelWorld().getModelTranslation());
    lookAt(aTarget);
    return *this;
}

Object3D& Object3D::lookAt(const vec3 &aEyePoint, const vec3 &aTarget, const vec3 &aWorldUp)
{
    setWorldUp(aWorldUp);
    translate(aEyePoint - getModelWorld().getModelTranslation());
    lookAt(aTarget);
    return *this;
}

//methods related to parent/children

void Object3D::addChild(const Object3DRef &aChild)
{
    aChild->setShouldUpdate(true);
    aChild->setParent(shared_from_this());
    mChildren.push_back(aChild);
    aChild->notifyTopParentAdded();
}

void Object3D::addChildren(const std::list<Object3DRef> &aChildren)
{
    for (auto iter=aChildren.begin(); iter != aChildren.end(); ++iter){
        addChild(*iter);
    }
}

void Object3D::removeChild(const Object3DRef &aChild)
{
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        if (aChild == *iter){
            if(aChild->hasChildren()){
                aChild->removeAllChildren();
            }
            aChild->notifyTopParentRemoved();
            iter = mChildren.erase(iter);
            break;
        }
    }
}

void Object3D::removeChildren(const std::list<Object3DRef> &aChildren)
{
    for (auto iter=aChildren.begin(); iter != aChildren.end(); ++iter){
        removeChild(*iter);
    }
}

void Object3D::removeAllChildren()
{
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        Object3DRef child = *iter;
        removeChild(child);
    }
    mChildren.clear();
}

void Object3D::clearChildren()
{
    mChildren.clear();
}

void Object3D::setParent(const Object3DRef &aParent)
{
    mParent = aParent;
}

Object3DRef Object3D::getParent()
{
    return mParent.lock();
}

Object3DRef Object3D::getTopParent()
{
    Object3DRef parent = shared_from_this();
    while (parent->hasParent()){
        parent = parent->getParent();
    }
    return parent;
}

Object3DRef Object3D::getChildByName(const std::string &aName, bool aIsRecursive) const
{
    //first search the first level, then get recursive
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->getName() == aName){
            return object;
        }
    }
    
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (aIsRecursive && object->hasChildren()){
            const Object3DRef& childObject = object->getChildByName(aName, true);
            if (childObject) return childObject;
        }
    }
    return M_NULLPTR;
}

Object3DRef Object3D::getChildByType(const std::string &aType, bool aIsRecursive) const
{
    //first search the first level, then get recursive
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->getType() == aType){
            return object;
        }
    }
    
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (aIsRecursive && object->hasChildren()){
            const Object3DRef& childObject = object->getChildByType(aType, true);
            if (childObject) return childObject;
        }
    }
    return M_NULLPTR;
}

Object3DRef Object3D::getChildById(int aId, bool aIsRecursive) const
{
    //first search the first level, then get recursive
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->getId() == aId){
            return object;
        }
    }
    
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (aIsRecursive && object->hasChildren()){
            const Object3DRef& childObject = object->getChildById(aId , true);
            if (childObject) return childObject;
        }
    }
    
    return M_NULLPTR;
}

Object3DRef Object3D::getChildByGuid(const Guid &aGuid, bool aIsRecursive) const
{
    //first search the first level, then get recursive
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->getGuid() == aGuid){
            return object;
        }
    }
    
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (aIsRecursive && object->hasChildren()){
            const Object3DRef& childObject = object->getChildByGuid(aGuid, true);
            if (childObject) return childObject;
        }
    }
    
    return M_NULLPTR;
}

Object3DRef Object3D::getChildByTag(const std::string &aTag, bool aIsRecursive) const
{
    //first search the first level, then get recursive
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->hasTag(aTag)){
            return object;
        }
    }
    
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (aIsRecursive && object->hasChildren()){
            const Object3DRef& childObject = object->getChildByTag(aTag, true);
            if (childObject) return childObject;
        }
    }
    
    return M_NULLPTR;
}

std::vector<Object3DRef> Object3D::getChildrenByTag(const std::string &aTag, bool aIsRecursive) const
{
    std::vector<Object3DRef> list;
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->hasTag(aTag)){
            list.push_back(object);
        }
        if (aIsRecursive && object->hasChildren()){
            std::vector<Object3DRef> childList = object->getChildrenByTag(aTag, true);
            list.insert(list.end(), childList.begin(), childList.end());
        }
    }
    return list;
}

std::vector<Object3DRef> Object3D::getChildrenByName(const std::string &aName, bool aIsRecursive) const
{
    std::vector<Object3DRef> list;
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->getName() == aName){
            list.push_back(object);
        }
        if (aIsRecursive && object->hasChildren()){
            std::vector<Object3DRef> childList = object->getChildrenByName(aName, true);
            list.insert(list.end(), childList.begin(), childList.end());
        }
    }
    return list;
}

std::vector<Object3DRef> Object3D::getChildrenByType(const std::string &aType, bool aIsRecursive) const
{
    std::vector<Object3DRef> list;
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->getType() == aType){
            list.push_back(object);
        }
        if (aIsRecursive && object->hasChildren()){
            std::vector<Object3DRef> childList = object->getChildrenByType(aType, true);
            list.insert(list.end(), childList.begin(), childList.end());
        }
    }
    return list;
}

std::vector<Object3DRef> Object3D::getChildrenById(int aId, bool aIsRecursive) const
{
    std::vector<Object3DRef> list;
    for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
        const Object3DRef& object = *iter;
        if (object->getId() == aId){
            list.push_back(object);
        }
        if (aIsRecursive && object->hasChildren()){
            std::vector<Object3DRef> childList = object->getChildrenById(aId, true);
            list.insert(list.end(), childList.begin(), childList.end());
        }
    }
    return list;
}

bool Object3D::hasChildren() const
{
    if (mChildren.size()>0) return true;
    else return false;
}

bool Object3D::hasParent() const
{
    if (auto parent = mParent.lock()) return true;
    else return false;
}

void Object3D::notifyTopParentAdded()
{
    Object3DRef topParent = getTopParent();
    if (topParent->getType()=="scene" && mShouldNotifyTopParent){
        SceneRef parentScene = std::dynamic_pointer_cast<Scene>(topParent);
        parentScene->onNotifyAdded(shared_from_this());
        if (parentScene->getShouldCheckChildrenNames()){
            mName = parentScene->verifyName(mName);
            parentScene->addNameToMap(mName);
        }
    }
    
    //this is for the case when a mother along with her children is added in one place to the scene, after a clone for instance
    if (hasChildren()){
        auto children = getChildren();
        for (auto child : children){
            child->notifyTopParentAdded();
        }
    }
}

void Object3D::notifyTopParentRemoved()
{
    Object3DRef topParent = getTopParent();
    if (topParent->getType()=="scene" && mShouldNotifyTopParent){
        SceneRef parentScene = std::dynamic_pointer_cast<Scene>(topParent);
        parentScene->onNotifyRemoved(shared_from_this());
    }
}

void Object3D::renewGuid()
{
    mGuid = GuidGenerator().newGuid();
}


void Object3D::addTag(const std::string &aTag)
{
    mTags.push_back(aTag);
}

void Object3D::removeTag(const std::string &aTag)
{
    for (auto iter = mTags.begin(); iter != mTags.end(); ++iter){
        if ((*iter) == aTag){
            iter = mTags.erase(iter);
        }
    }
}

bool Object3D::hasTag(const std::string &aTag)
{
    for (auto tag : mTags){
        if (tag==aTag) return true;
    }
    return false;
}

// setters/getters

Object3D& Object3D::model(const Model &aModel)
{
    mModel = aModel;
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::worldUp(const vec3 &aWorldUp)
{
    mWorldUp = normalize(aWorldUp);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::viewDirection(const vec3 &aViewDirection)
{
    mViewDirection = normalize(aViewDirection);
    setShouldUpdate(true);
    return *this;
}

Object3D& Object3D::type(const std::string &aType)
{
    mType = aType;
    return *this;
}

Object3D& Object3D::tags(const std::list<std::string> &aTags)
{
    mTags = aTags;
    return *this;
}

Object3D& Object3D::targetObject(const Object3DRef &aTargetObject)
{
    mTargetObject = aTargetObject;
    setShouldLookAtTargetObject(true);
    return *this;
}

Object3D& Object3D::shouldLookAtTargetObject(bool aShouldLookAtTargetObject)
{
    mShouldLookAtTargetObject = aShouldLookAtTargetObject;
    return *this;
}

Object3D& Object3D::name(const std::string &aName)
{
    string verifiedName = aName;
    if (getTopParent()->getType() == "scene") { // this means that the child was already part of the scene so we need to use removeNameFromMap()
        auto scene = getTopParent()->cast<Scene>();
        if (scene->getShouldCheckChildrenNames()){
            verifiedName = scene->verifyName(aName);
            scene->removeNameFromMap(mName);
        }
    }
    mName = verifiedName;
    return *this;
}

Object3D& Object3D::id(int aId)
{
    mId = aId;
    return *this;
}

Object3D& Object3D::guid(const Guid &aGuid)
{
    mGuid = aGuid;
    return *this;
}

Object3D& Object3D::isVisible(bool aIsVisible)
{
    mIsVisible = aIsVisible;
    return *this;
}

Object3D& Object3D::isActive(bool aIsActive)
{
    mIsActive = aIsActive;
    return *this;
}

Object3D& Object3D::doesCastShadow(bool aDoesCastShadow)
{
    mDoesCastShadow = aDoesCastShadow;
    return *this;
}

Object3D& Object3D::doesReceiveShadow(bool aDoesReceiveShadow)
{
    mDoesReceiveShadow = aDoesReceiveShadow;
    return *this;
}

Object3D& Object3D::shouldUpdate(bool aShouldUpdate)
{
    setShouldUpdate(aShouldUpdate);
    return *this;
}

Object3D& Object3D::shouldUpdateSelf(bool aShouldUpdateSelf)
{
    setShouldUpdateSelf(aShouldUpdateSelf);
    return *this;
}

Object3D& Object3D::shouldNotifyTopParent(bool aShouldNotifyTopParent)
{
    mShouldNotifyTopParent = aShouldNotifyTopParent;
    return *this;
}

const Model& Object3D::getModel() const
{
    return mModel;
}

const Model& Object3D::getModelWorld() const
{
    return mModelWorld;
}

const vec3& Object3D::getWorldUp() const
{
    return mWorldUp;
}

const vec3& Object3D::getViewDirection() const
{
    return mViewDirection;
}

const std::string& Object3D::getType() const
{
    return mType;
}

const std::list<std::string>& Object3D::getTags() const
{
    return mTags;
}

Object3DRef Object3D::getTargetObject() const
{
    return mTargetObject.lock();
}

bool Object3D::getShouldLookAtTargetObject() const
{
    return mShouldLookAtTargetObject;
}

const std::list<Object3DRef>& Object3D::getChildren() const
{
    return mChildren;
}

const std::string& Object3D::getName() const
{
    return mName;
}

int Object3D::getId() const
{
    return mId;
}

const Guid& Object3D::getGuid() const
{
    return mGuid;
}

bool Object3D::getIsVisible() const
{
    return mIsVisible;
}

bool Object3D::getIsActive() const
{
    return mIsActive;
}

bool Object3D::getDoesCastShadow() const
{
    return mDoesCastShadow;
}

bool Object3D::getDoesReceiveShadow() const
{
    return mDoesReceiveShadow;
}

bool Object3D::getShouldUpdate() const
{
    return mShouldUpdate;
}

bool Object3D::getShouldUpdateSelf() const
{
    return mShouldUpdateSelf;
}

bool Object3D::getShouldNotifyTopParent() const
{
    return mShouldNotifyTopParent;
}

void Object3D::setModel(const Model &aModel)
{
    mModel = aModel;
    setShouldUpdate(true);
}

void Object3D::setModelWorld(const Model &aModelWorld)
{
    mModelWorld = aModelWorld;
    setShouldUpdate(true);
}

void Object3D::setWorldUp(const vec3 &aWorldUp)
{
    mWorldUp = normalize(aWorldUp);
    setShouldUpdate(true);
}

void Object3D::setViewDirection(const vec3 &aViewDirection)
{
    mViewDirection = normalize(aViewDirection);
    mModel.setModelOrientation(glm::rotation( mViewDirection, vec3( 0, 0, 1 ) ));
    setShouldUpdate(true);
}

void Object3D::setType(const std::string &aType)
{
    mType = aType;
}

void Object3D::setTags(const std::list<std::string> &aTags)
{
    mTags = aTags;
}

void Object3D::setTargetObject(const Object3DRef &aTargetObject)
{
    mTargetObject = aTargetObject;
    setShouldLookAtTargetObject(true);
}

void Object3D::setShouldLookAtTargetObject(bool aShouldLookAtTargetObject)
{
    mShouldLookAtTargetObject = aShouldLookAtTargetObject;
}

void Object3D::setName(const std::string &aName)
{
    string verifiedName = aName;
    if (getTopParent()->getType() == "scene") { // this means that the child was already part of the scene so we need to use removeName()
        auto scene = getTopParent()->cast<Scene>();
        if (scene->getShouldCheckChildrenNames()){
            verifiedName = scene->verifyName(aName);
            scene->removeNameFromMap(mName);
        }
    }
    mName = verifiedName;
}

void Object3D::setId(int aId)
{
    mId = aId;
}

void Object3D::setGuid(const Guid &aGuid)
{
    mGuid = aGuid;
}

void Object3D::setIsVisible(bool aIsVisible)
{
    mIsVisible = aIsVisible;
}

void Object3D::setIsActive(bool aIsActive)
{
    mIsActive = aIsActive;
}

void Object3D::setDoesCastShadow(bool aDoesCastShadow)
{
    mDoesCastShadow = aDoesCastShadow;
}

void Object3D::setDoesReceiveShadow(bool aDoesReceiveShadow)
{
    mDoesReceiveShadow = aDoesReceiveShadow;
}

void Object3D::setShouldUpdate(bool aShouldUpdate)
{
    mShouldUpdate = aShouldUpdate;
    //if object has to update (meaning the models) then all it's children should too
    if (mShouldUpdate){
        for (auto iter = mChildren.begin(); iter!=mChildren.end(); ++iter){
            (*iter)->setShouldUpdate(aShouldUpdate);
        }
    }
}

void Object3D::setShouldUpdateSelf(bool aShouldUpdateSelf)
{
    mShouldUpdateSelf = true;
}

void Object3D::setShouldNotifyTopParent(bool aShouldNotifyTopParent)
{
    mShouldNotifyTopParent = aShouldNotifyTopParent;
}
