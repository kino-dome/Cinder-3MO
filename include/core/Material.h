#pragma once

#define MATERIAL_CLONE(MyType) \
MaterialRef                 cloneSelf() override\
{\
    return std::make_shared<MyType>(*this);\
}\
std::shared_ptr<MyType> clone() \
{ \
    std::shared_ptr<Material> material = cloneSelf(); \
    material->setGuid(GuidGenerator().newGuid());\
    if (material->getName() != ""){\
        material->setName(material->getName()+"_clone");\
    }\
    return  std::dynamic_pointer_cast<MyType>(material);\
}\

#include "cinder/Color.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "../core/guid.h"
#include "../core/MaterialData.h"
#include "../objects/Mesh.h"
#include "../objects/Light.h"


typedef std::shared_ptr<class MaterialPallet> MaterialPalletRef;

////////////////Material////////////////////////////
typedef std::shared_ptr<class Material> MaterialRef;

class Material : public std::enable_shared_from_this<Material>
{
public:
    
	Material();
    virtual ~Material()=0;
    virtual MaterialData&               getMaterialData() =0;
    virtual const MaterialData&         getMaterialData() const =0;
    virtual void                        attachAndDraw(const MeshRef& aMesh){};
    virtual bool                        getIsPbr()=0;
    
    virtual MaterialRef                 cloneSelf()=0;
    std::shared_ptr<Material>           clone(){return std::dynamic_pointer_cast<Material>(cloneSelf());}
	
    //setters/getters
    Material&                           shader(const ci::gl::GlslProgRef& aShader);
    Material&                           type(const std::string& aType);
    Material&                           name(const std::string& aName);
    Material&                           id(int aId);
    Material&                           guid(const Guid& aGuid);
    Material&                           parentPallet(const MaterialPalletRef& aParentPallet);
    
    const ci::gl::GlslProgRef&          getShader() const;
    const std::string&                  getType() const;
    const std::string&                  getName() const;
    int                                 getId() const;
    const ci::gl::GlslProg::Format&     getShaderFormat() const;
    const Guid&                         getGuid() const;
    const MaterialPalletRef&            getParentPallet() const;
	
    void                                setShader(const ci::gl::GlslProgRef& aShader);
    void                                setType(const std::string& aType);
    void                                setName(const std::string& aName);
    void                                setId(int aId);
    void                                setGuid(const Guid& aGuid);
    void                                setParentPallet(const MaterialPalletRef& aParentPallet);
    
    //Maps setters/getters
    void                                setDiffuseMap(const ci::gl::Texture2dRef& aDiffuseMap);
    void                                setSpecularMap(const ci::gl::Texture2dRef& aSpecularMap);
    void                                setEmissionMap(const ci::gl::Texture2dRef& aEmissionMap);
    void                                setDisplacementMap(const ci::gl::Texture2dRef& aDisplacementMap);
    void                                setNormalMap(const ci::gl::Texture2dRef& aNormalMap);
    void                                setOcclusionMap(const ci::gl::Texture2dRef& aOcclusionMap);
    const ci::gl::Texture2dRef&         getDiffuseMap() const;
    const ci::gl::Texture2dRef&         getSpecularMap() const;
    const ci::gl::Texture2dRef&         getEmissionMap() const;
    const ci::gl::Texture2dRef&         getDisplacementMap() const;
    const ci::gl::Texture2dRef&         getNormalMap() const;
    const ci::gl::Texture2dRef&         getOcclusuionMap() const;
    
    template <class T>
    std::shared_ptr<T>                  cast()
    {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }
    
    
public:
    MaterialPalletRef                   mParentPallet;
    ci::gl::GlslProgRef                 mShader;
    Guid                                mGuid;
    std::string                         mType, mName;
    int                                 mId;
    ci::gl::GlslProg::Format            mShaderFormat; //we keep this so we can use it to combine the material(shader) with others
    ci::gl::Texture2dRef                mDisplacementMap, mDiffuseMap, mEmissionMap, mNormalMap, mOcclusionMap, mSpecularMap ;
};

//////////////////ClassicMaterial///////////////////
typedef std::shared_ptr<class ClassicMaterial> ClassicMaterialRef;

class ClassicMaterial : public Material
{
public:
    static ClassicMaterialRef create(){
        return ClassicMaterialRef (new ClassicMaterial());
    }
    
    ClassicMaterial();
    virtual ~ClassicMaterial();
    MATERIAL_CLONE(ClassicMaterial);
    
    ClassicMaterialData& getMaterialData() override;
    const ClassicMaterialData& getMaterialData() const override;
    bool getIsPbr() override {return false;}
    
    
    
public:
    ClassicMaterialData                 mMaterialData;
};


//////////////////PbrMaterial///////////////////////
typedef std::shared_ptr<class PbrMaterial> PbrMaterialRef;

class PbrMaterial : public Material
{
public:
    static PbrMaterialRef create(){
        return PbrMaterialRef (new PbrMaterial());
    }
    
    PbrMaterial();
    virtual ~PbrMaterial();
    MATERIAL_CLONE(PbrMaterial);
    
    PbrMaterialData& getMaterialData() override;
    const PbrMaterialData& getMaterialData() const override;
    bool getIsPbr() override {return true;}
    
public:
    PbrMaterialData                     mMaterialData;
};

///////////////MaterialColor////////////////

typedef std::shared_ptr<class MaterialColor> MaterialColorRef;

class MaterialColor : public ClassicMaterial
{
public:
    static MaterialColorRef create(){
        return MaterialColorRef (new MaterialColor());
    }
    
    MaterialColor();
    ~MaterialColor();
    
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialColor)
};

///////////////MaterialTexture////////////////

typedef std::shared_ptr<class MaterialTexture> MaterialTextureRef;
class MaterialTexture : public ClassicMaterial
{
public:
    static MaterialTextureRef create(){
        return MaterialTextureRef (new MaterialTexture());
    }
    
    MaterialTexture();
    ~MaterialTexture();
    
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialTexture)
    
public:
};

///////////////MaterialLambert////////////////

typedef std::shared_ptr<class MaterialLambert> MaterialLambertRef;
class MaterialLambert : public ClassicMaterial
{
public:
    static MaterialLambertRef create(){
        return MaterialLambertRef (new MaterialLambert());
    }
    
    MaterialLambert();
    ~MaterialLambert();
    
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialLambert)
    
    
public:
};

///////////////////////MaterialNormaliViz/////////////////////////

typedef std::shared_ptr<class MaterialNormalViz> MaterialNormalVizRef;
class MaterialNormalViz : public ClassicMaterial
{
public:
    static MaterialNormalVizRef create(){
        return MaterialNormalVizRef (new MaterialNormalViz());
    }
    
    MaterialNormalViz();
    ~MaterialNormalViz();
    
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialNormalViz)
    
public:

};

///////////////////////MaterialWireframe/////////////////////////

typedef std::shared_ptr<class MaterialWireframe> MaterialWireframeRef;
class MaterialWireframe : public ClassicMaterial
{
public:
    static MaterialWireframeRef create(){
        return MaterialWireframeRef (new MaterialWireframe());
    }
    
    MaterialWireframe();
    ~MaterialWireframe();
    
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialWireframe)
    
    //setters/getters
    MaterialWireframe&          lineWidth(float aLineWidth);
    float                       getLineWidth() const;
    void                        setLineWidth(float aLineWidth);
    
public:
    float                       mLineWidth;
};

///////////////////////MaterialPhong/////////////////////////

typedef std::shared_ptr<class MaterialPhong> MaterialPhongRef;
class MaterialPhong : public ClassicMaterial
{
public:
    static MaterialPhongRef create(){
        return MaterialPhongRef (new MaterialPhong());
    }
    
    MaterialPhong();
    ~MaterialPhong();
    
    void                        attachAndDraw(const MeshRef& aMesh) override;
    //MaterialRef                 cloneSelf() override;
    MATERIAL_CLONE(MaterialPhong)
    
    MaterialPhong&              mode(int aMode);
    int                         getMode() const;
    void                        setMode(int aMode);
    
public:
    int                         mMode;
    
};
