//
//  Object3D.h
//  iPhotoWall
//
//  Created by Hesam Ohadi on 15/11/15.
//
//
#pragma once

#define OBJECT3D_CLONE(MyType) \
std::shared_ptr<Object3D>                     cloneSelf() override\
{\
    return std::make_shared<MyType>(*this);\
}\
std::shared_ptr<MyType> clone(bool aIncludeChildren, bool aShouldReattachToParent = true) \
{ \
    std::shared_ptr<Object3D> motherObject = cloneSelf(); \
    motherObject->renewGuid();\
    std::string name = motherObject->getName();\
    if (name != ""){\
        motherObject->setName(name + "_clone");\
    }\
    motherObject->clearChildren();\
    auto parent = motherObject->getParent();\
    motherObject->setParent(nullptr);\
    if (aShouldReattachToParent){\
        parent->addChild(motherObject);\
    }\
    if (aIncludeChildren){\
        for (auto iter=mChildren.begin(); iter!= mChildren.end(); ++iter){\
            motherObject->addChild((*iter)->clone(aIncludeChildren, false));\
        }\
    }\
    return std::dynamic_pointer_cast<MyType>(motherObject); \
}\

#include <type_traits>

#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"

#include "../core/guid.h"
#include "../core/Model.h"

typedef std::shared_ptr<class Object3D> Object3DRef;
typedef std::weak_ptr<class Object3D> Object3DWeakRef;
typedef std::unique_ptr<class Object3D> Object3DUniqueRef;

static const std::shared_ptr<Object3D> M_NULLPTR = std::shared_ptr<Object3D>(nullptr);


//Observer
class Observer
{
public:
    enum Event
    {
        ADD, REMOVE, NAME_CHANGE
    };
    
    virtual ~Observer() {}
protected:
    virtual void onNotify(const Object3D& aObject3D, Event aEvent);
};


//Base class for scenegraph objects, everything is a Object3D which means it has transformation in space, setup/update/draw and name/tag/guid

class Object3D : public std::enable_shared_from_this<Object3D>, public Observer {
//    friend class Scene;
public:
    static Object3DRef                      create() {return Object3DRef(new Object3D());}

    Object3D();
    virtual ~Object3D(){}
    
    void                                    setup();
    void                                    update();
    void                                    draw();

    
    std::shared_ptr<Object3D> clone(bool aIncludeChildren, bool aShouldReattachToParent = true)
    {
        std::shared_ptr<Object3D> motherObject = cloneSelf();
        motherObject->renewGuid();
        if (motherObject->getName() != ""){
            motherObject->setName(motherObject->getName()+ "_clone");
        }
        motherObject->clearChildren();
        auto parent = motherObject->getParent();
        motherObject->setParent(nullptr);
        if (aShouldReattachToParent){
            parent->addChild(motherObject);
        }
        if (aIncludeChildren){
            for (auto iter=mChildren.begin(); iter!= mChildren.end(); ++iter){
                motherObject->addChild((*iter)->clone(true, false));
            }
        }
        return std::dynamic_pointer_cast<Object3D>(motherObject);
    }
    void                                    renewGuid();
    void                                    checkName();
    void                                    clearChildren();//just clears the mChildren array, used for cloning
    void                                    setParent(const Object3DRef& aParent); // used when adding children to parents
    
    //convinience methods for transforming the model
    Object3D&                               lookAt(const ci::vec3& aTarget);
    Object3D&                               lookAt(const ci::vec3& aEyePoint, const ci::vec3& aTarget );
    Object3D&                               lookAt(const ci::vec3& aEyePoint, const ci::vec3& aTarget, const ci::vec3& aWorldUp );
        //incremental
    Object3D&                               translate(const ci::vec3& aTranslation);
    Object3D&                               rotate(const ci::quat& aRotation);
    Object3D&                               rotateX(float aAngle);
    Object3D&                               rotateY(float aAngle);
    Object3D&                               rotateZ(float aAngle);
    Object3D&                               scale(const ci::vec3& aScale);
        //global
    Object3D&                               setTranslation(const ci::vec3& aTranslation);
    Object3D&                               setRotation(const ci::quat& aRotation);
    Object3D&                               setRotationX(float aAngle);
    Object3D&                               setRotationY(float aAngle);
    Object3D&                               setRotationZ(float aAngle);
    Object3D&                               setScale(const ci::vec3& aScale);
    
    //methods related to parent/children
    void                                    addChild(const Object3DRef& aChild);
    void                                    addChildren(const std::list<Object3DRef>& aChildren);
    void                                    removeChild(const Object3DRef& aChild);
    void                                    removeChildren(const std::list<Object3DRef>& aChildren);
    void                                    removeAllChildren();
    Object3DRef                             getParent();
    Object3DRef                             getTopParent();//usually the scene
    Object3DRef                             getChildByName(const std::string& aName, bool aIsRecursive=true) const; //returns the first child by name
    Object3DRef                             getChildByType(const std::string& aType, bool aIsRecursive=true) const; //returns the first child by type
    Object3DRef                             getChildById(int aId, bool aIsRecursive=true) const;// returns the first child from the id
    Object3DRef                             getChildByGuid(const Guid& aGuid, bool aIsRecursive=true) const; // returns the unique child by GUID
    Object3DRef                             getChildByTag(const std::string& aTag, bool aIsRecursive=true) const;
    std::vector<Object3DRef>                getChildrenByTag(const std::string& aTag, bool aIsRecursive=true) const;
    std::vector<Object3DRef>                getChildrenByName(const std::string& aName, bool aIsRecursive=true) const;
    std::vector<Object3DRef>                getChildrenByType(const std::string& aType, bool aIsRecursive=true) const;
    std::vector<Object3DRef>                getChildrenById(int aId, bool aIsRecursive=true) const;
    const std::list<Object3DRef>&           getChildren() const;
    bool                                    hasChildren() const;
    bool                                    hasParent() const;
    void                                    addTag(const std::string& aTag);
    void                                    removeTag(const std::string& aTag);
    bool                                    hasTag(const std::string& aTag);
    
    template <class T>
    std::shared_ptr<T>                      cast()
    {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }
    
    template <class T>
    std::shared_ptr<T>                      getCastedChildByName(const std::string& aName, bool aIsRecursive) const
    {
        //first search the first level, then get recursive
        const Object3DRef& outObject = getChildByName(aName, aIsRecursive);
        
        if (outObject){
            return std::dynamic_pointer_cast<T>(outObject);
        }
        else {
            return nullptr;
        }
    }
    
    template <class T>
    std::shared_ptr<T>                      getCastedChildByType(const std::string& aType, bool aIsRecursive) const
    {
        //first search the first level, then get recursive
        const Object3DRef& outObject = getChildByType(aType, aIsRecursive);
        
        if (outObject){
            return std::dynamic_pointer_cast<T>(outObject);
        }
        else {
            return nullptr;
        }
    }
    
    template <class T>
    std::shared_ptr<T>                      getCastedChildByTag(const std::string& aTag, bool aIsRecursive) const
    {
        //first search the first level, then get recursive
        const Object3DRef& outObject = getChildByTag(aTag, aIsRecursive);
        
        if (outObject){
            return std::dynamic_pointer_cast<T>(outObject);
        }
        else {
            return nullptr;
        }
    }
    
    template <class T>
    std::shared_ptr<T>                      getCastedChildById(int aId, bool aIsRecursive) const
    {
        //first search the first level, then get recursive
        const Object3DRef& outObject = getChildById(aId, aIsRecursive);
        
        if (outObject){
            return std::dynamic_pointer_cast<T>(outObject);
        }
        else {
            return nullptr;
        }
    }
    
    template <class T>
    std::vector<std::shared_ptr<T>>         getCastedChildrenByName(const std::string& aName, bool aIsRecursive) const
    {
        std::vector<std::shared_ptr<T>> list;
        for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
            const Object3DRef& object = *iter;
            if (object->getName() == aName){
                list.push_back(std::dynamic_pointer_cast<T>(object));
            }
            if (aIsRecursive && object->hasChildren()){
                std::vector<std::shared_ptr<T>> childList = object->getCastedChildrenByName<T>(aName, true);
                list.insert(list.end(), childList.begin(), childList.end());
            }
        }
        return list;
    }
    
    template <class T>
    std::vector<std::shared_ptr<T>>         getCastedChildrenByType(const std::string& aType, bool aIsRecursive) const
    {
        std::vector<std::shared_ptr<T>> list;
        for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
            const Object3DRef& object = *iter;
            if (object->getType() == aType){
                list.push_back(std::dynamic_pointer_cast<T>(object));
            }
            if (aIsRecursive && object->hasChildren()){
                std::vector<std::shared_ptr<T>> childList = object->getCastedChildrenByType<T>(aType, true);
                list.insert(list.end(), childList.begin(), childList.end());
            }
        }
        return list;
    }
    
    template <class T>
    std::vector<std::shared_ptr<T>>         getCastedChildrenById(int aId, bool aIsRecursive) const
    {
        std::vector<std::shared_ptr<T>> list;
        for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
            const Object3DRef& object = *iter;
            if (object->getId() == aId){
                list.push_back(std::dynamic_pointer_cast<T>(object));
            }
            if (aIsRecursive && object->hasChildren()){
                std::vector<std::shared_ptr<T>> childList = object->getCastedChildrenById<T>(aId, true);
                list.insert(list.end(), childList.begin(), childList.end());
            }
        }
        return list;
    }
    
    template <class T>
    std::vector<std::shared_ptr<T>>         getCastedChildrenByTag(const std::string& aTag, bool aIsRecursive) const
    {
        std::vector<std::shared_ptr<T>> list;
        for (auto iter=mChildren.begin(); iter != mChildren.end(); ++iter){
            const Object3DRef& object = *iter;
            if (object->hasTag(aTag)){
                list.push_back(std::dynamic_pointer_cast<T>(object));
            }
            if (aIsRecursive && object->hasChildren()){
                std::vector<std::shared_ptr<T>> childList = object->getCastedChildrenByTag<T>(aTag, true);
                list.insert(list.end(), childList.begin(), childList.end());
            }
        }
        return list;
    }
    
    // setters/getters
    Object3D&                               model(const Model& aModel);
    Object3D&                               worldUp(const ci::vec3& aWorldUp);
    Object3D&                               viewDirection(const ci::vec3& aViewDirection);
    Object3D&                               type(const std::string& aType);
    Object3D&                               tags(const std::list<std::string>& aTags);
    Object3D&                               targetObject(const Object3DRef& aTargetObject);
    Object3D&                               shouldLookAtTargetObject(bool aShouldLookAtTargetObject);
    Object3D&                               name(const std::string& aName);
    Object3D&                               id(int aId);
    Object3D&                               guid(const Guid& aGuid);
    Object3D&                               isVisible(bool aIsVisible);
    Object3D&                               isActive(bool aIsActive);
    Object3D&                               doesCastShadow(bool aDoesCastShadow);
    Object3D&                               doesReceiveShadow(bool aDoesReceiveShadow);
    Object3D&                               shouldUpdate(bool aShouldUpdate);
    Object3D&                               shouldUpdateSelf(bool aShouldUpdateSelf);
    Object3D&                               shouldNotifyTopParent(bool aShouldNotifyTopParent);
    
    const Model&                            getModel() const;
    const Model&                            getModelWorld() const;
    const ci::vec3&                         getWorldUp() const;
    const ci::vec3&                         getViewDirection() const;
    Object3DRef                             getTargetObject() const;
    bool                                    getShouldLookAtTargetObject() const;
    const std::string&                      getType() const;
    const std::list<std::string>&           getTags() const;
    const std::string&                      getName() const;
    int                                     getId() const;
    const Guid&                             getGuid() const;
    bool                                    getIsVisible() const;
    bool                                    getIsActive() const;
    bool                                    getDoesCastShadow() const;
    bool                                    getDoesReceiveShadow() const;
    bool                                    getShouldUpdate() const;
    bool                                    getShouldUpdateSelf() const;
    bool                                    getShouldNotifyTopParent() const;
    
    void                                    setModel(const Model& aModel);
    void                                    setModelWorld(const Model& aModelWorld);
    void                                    setWorldUp(const ci::vec3& aWorldUp);
    void                                    setViewDirection(const ci::vec3& aViewDirection);
    void                                    setTargetObject(const Object3DRef& aTargetObject);
    void                                    setShouldLookAtTargetObject(bool aShouldLookAtTargetObject);
    void                                    setType(const std::string& aType);
    void                                    setTags(const std::list<std::string>& aTags);
    void                                    setName(const std::string& aName);
    void                                    setId(int aId);
    void                                    setGuid(const Guid& aGuid);
    void                                    setIsVisible(bool aIsVisible);
    void                                    setIsActive(bool aIsActive);
    void                                    setDoesCastShadow(bool aDoesCastShadow);
    void                                    setDoesReceiveShadow(bool aDoesReceiveShadow);
    void                                    setShouldUpdate(bool aShouldUpdate);
    void                                    setShouldUpdateSelf(bool aShouldUpdateSelf);
    void                                    setShouldNotifyTopParent(bool aShouldNotifyTopParent);
    
protected:
    virtual Object3DRef                     cloneSelf()
    {
        return std::make_shared<Object3D>(*this);
    }
    
    // no use in mind for setup now but it might come handy
    virtual void                            setupSelf() {}
    virtual void                            onSetup() {}// with every update this is called, see CameraObject
    virtual void                            onSetupSelf() {}// this is in case one needs to extend setupSelf behaviour based on events
    virtual void                            updateSelf();
    virtual void                            onUpdate();// with every update this is called, see CameraObject
    virtual void                            onUpdateSelf();// this is in case one needs to extend updateSelf behaviour based on events
    virtual void                            drawSelf();
    virtual void                            onDraw(){} // with every draw this is called
    virtual void                            onDrawSelf(){}// this is in case one needs to extend drawSelf behaviour based on events
    void                                    updateChildren();
    void                                    drawChildren();
    
    //notification and observer related
    void                                    onNotify(const Object3DRef& aObject3D, Observer::Event aEvent) override {};
    void                                    notify(const std::shared_ptr<Observer>& aObserver, Observer::Event aEvent);
    void                                    notifyTopParentAdded();
    void                                    notifyTopParentRemoved();

    
protected:
    
    //mModelWorld is for when the object is a child and it needs to calculate its transform based on the parent
    Model                                   mModel, mModelWorld;
    ci::vec3                                mWorldUp, mViewDirection;
    Object3DWeakRef                         mTargetObject;
    std::string                             mType, mName;
    std::list<std::string>                  mTags;
    int                                     mId;
    Guid                                    mGuid;// this is unique === UUID
    bool                                    mIsVisible, mIsActive, mDoesCastShadow, mDoesReceiveShadow, mShouldLookAtTargetObject;
    bool                                    mShouldUpdate, mShouldUpdateSelf;
    bool                                    mShouldNotifyTopParent;//for certain objects like lights we want the scene to know when they are added/deleted so it can keep a refrence list in case renderers need it
    Object3DWeakRef                         mParent;
    std::list<Object3DRef>                  mChildren;
};

