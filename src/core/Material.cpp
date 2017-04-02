#include "Material.h"

///////////////////////Material////////////////////////

Material::Material() : mType("material"), mName(""), mId(0), mGuid(GuidGenerator().newGuid())
{
    
}

Material::~Material()
{
    
}

//setters/getters
Material& Material::shader(const ci::gl::GlslProgRef &aShader)
{
    mShader = aShader;
    return *this;
}

Material& Material::type(const std::string &aType)
{
    mType = aType;
    return *this;
}

Material& Material::name(const std::string &aName)
{
    mName = aName;
    return *this;
}

Material& Material::id(int aId)
{
    mId = aId;
    return  *this;
}

Material& Material::guid(const Guid &aGuid)
{
    mGuid = aGuid;
    return *this;
}

Material& Material::parentPallet(const MaterialPalletRef &aParentPallet)
{
    mParentPallet = aParentPallet;
    return *this;
}


const ci::gl::GlslProgRef& Material::getShader() const
{
    return mShader;
}

const std::string& Material::getType() const
{
    return mType;
}

const std::string& Material::getName() const
{
    return mName;
}

int Material::getId() const
{
    return mId;
}

const ci::gl::GlslProg::Format& Material::getShaderFormat() const
{
    return mShaderFormat;
}

const Guid& Material::getGuid() const
{
    return mGuid;
}

const MaterialPalletRef& Material::getParentPallet() const
{
    return mParentPallet;
}

void Material::setShader(const ci::gl::GlslProgRef &aShader)
{
    mShader = aShader;
}

void Material::setType(const std::string &aType)
{
    mType = aType;
}

void Material::setName(const std::string &aName)
{
    mName = aName;
}

void Material::setId(int aId)
{
    mId = aId;
}

void Material::setGuid(const Guid &aGuid)
{
    mGuid = aGuid;
}

void Material::setParentPallet(const MaterialPalletRef &aParentPallet)
{
    mParentPallet = aParentPallet;
}

//Maps setters/gettters
void Material::setDiffuseMap(const ci::gl::Texture2dRef &aDiffuseMap)
{
    mDiffuseMap = aDiffuseMap;
}

void Material::setSpecularMap(const ci::gl::Texture2dRef &aSpecularMap)
{
    mSpecularMap = aSpecularMap;
}

void Material::setEmissionMap(const ci::gl::Texture2dRef &aEmissionMap)
{
    mEmissionMap = aEmissionMap;
}

void Material::setDisplacementMap(const ci::gl::Texture2dRef &aDisplacementMap)
{
    mDisplacementMap = aDisplacementMap;
}

void Material::setNormalMap(const ci::gl::Texture2dRef &aNormalMap)
{
    mNormalMap = aNormalMap;
}

void Material::setOcclusionMap(const ci::gl::Texture2dRef &aOcclusionMap)
{
    mOcclusionMap = aOcclusionMap;
}

const ci::gl::Texture2dRef& Material::getDiffuseMap() const
{
    return mDiffuseMap;
}

const ci::gl::Texture2dRef& Material::getSpecularMap() const
{
    return mSpecularMap;
}

const ci::gl::Texture2dRef& Material::getEmissionMap() const
{
    return mEmissionMap;
}


const ci::gl::Texture2dRef& Material::getDisplacementMap() const
{
    return mDisplacementMap;
}

const ci::gl::Texture2dRef& Material::getNormalMap() const
{
    return mNormalMap;
}

const ci::gl::Texture2dRef& Material::getOcclusuionMap() const
{
    return mOcclusionMap;
}

///////////////ClassicMaterial//////////////

ClassicMaterial::ClassicMaterial() : mMaterialData(ClassicMaterialData())
{
    mType = "classic_material";
}

ClassicMaterial::~ClassicMaterial()
{
    
}

ClassicMaterialData& ClassicMaterial::getMaterialData()
{
    return mMaterialData;
}

const ClassicMaterialData& ClassicMaterial::getMaterialData() const
{
    return mMaterialData;
}

///////////////PbrMaterial//////////////////

PbrMaterial::PbrMaterial() : Material(), mMaterialData(PbrMaterialData())
{
    mType = "pbr_material";
}

PbrMaterial::~PbrMaterial()
{
    
}

PbrMaterialData& PbrMaterial::getMaterialData()
{
    return mMaterialData;
}

const PbrMaterialData& PbrMaterial::getMaterialData() const
{
    return mMaterialData;
}


///////////////MaterialColor////////////////

MaterialColor::MaterialColor() : ClassicMaterial()
{
    mShader = ci::gl::getStockShader( ci::gl::ShaderDef().color());
    mType = "material_color";
}

MaterialColor::~MaterialColor()
{
    
}

void MaterialColor::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    
    const ci::gl::ScopedColor scopedColor;
    ci::gl::color(mMaterialData.getDiffuseColor());
    batch->draw();
}

//MaterialRef MaterialColor::cloneSelf()
//{
//    return std::make_shared<MaterialColor>(*this);;
//}

///////////////MaterialTexture////////////////

MaterialTexture::MaterialTexture() : ClassicMaterial()
{
    mShader = ci::gl::getStockShader( ci::gl::ShaderDef().color().texture());
    mType = "material_texture";
}

MaterialTexture::~MaterialTexture()
{
    
}

void MaterialTexture::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    
    const ci::gl::ScopedColor scopedColor;
    ci::gl::color(mMaterialData.getDiffuseColor());
    const ci::gl::ScopedTextureBind scopedTextureBind(mDiffuseMap);
    batch->draw();
}
//
//MaterialRef MaterialTexture::cloneSelf()
//{
//    return std::make_shared<MaterialTexture>(*this);;
//}



///////////////MaterialLambert////////////////

MaterialLambert::MaterialLambert() : ClassicMaterial()
{
    mShader = ci::gl::getStockShader( ci::gl::ShaderDef().color().texture().lambert());
    mType = "material_lambert";
}

MaterialLambert::~MaterialLambert()
{
    
}

void MaterialLambert::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    
    const ci::gl::ScopedColor scopedColor;
    ci::gl::color(mMaterialData.getDiffuseColor());
    const ci::gl::ScopedTextureBind scopedTextureBind(mDiffuseMap);
    batch->draw();
}

//MaterialRef MaterialLambert::cloneSelf()
//{
//    return std::make_shared<MaterialLambert>(*this);;
//}

///////////////MaterialNormalViz////////////////

MaterialNormalViz::MaterialNormalViz() : ClassicMaterial()
{
    ci::gl::GlslProg::Format mShaderFormat;
    mShaderFormat.vertex(CI_GLSL( 150,
                                uniform mat4	ciModelViewProjection;
                                in vec4			ciPosition;
                                in vec3			ciNormal;
                                out vec4		vColor;
                                
                                void main( void ) {
                                    gl_Position	= ciModelViewProjection * ciPosition;
                                    vColor = vec4(ciNormal, 1.0);
                                }
                                ) );
    mShaderFormat.fragment(CI_GLSL( 150,
                                  in vec4            vColor;
                                  out vec4           oColor;
                                  
                                  void main( void ) {
                                      oColor = vColor;
                                  }
                                  ) );
    mShader = ci::gl::GlslProg::create(mShaderFormat);
    mType = "material_normal_viz";
}

MaterialNormalViz::~MaterialNormalViz()
{
    
}

void MaterialNormalViz::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    batch->draw();
}


//MaterialRef MaterialNormalViz::cloneSelf()
//{
//    return std::make_shared<MaterialNormalViz>(*this);;
//}

///////////////MaterialWireframe////////////////

MaterialWireframe::MaterialWireframe() : ClassicMaterial()
{
    mShader = ci::gl::getStockShader(ci::gl::ShaderDef().color());
    mType = "material_wireframe";
}

MaterialWireframe::~MaterialWireframe()
{
    
}

void MaterialWireframe::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    ci::gl::color(mMaterialData.getDiffuseColor());
    glLineWidth(mLineWidth);
    ci::gl::polygonMode(GL_FRONT_AND_BACK, GL_LINE);
    batch->draw();
    ci::gl::polygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ci::gl::lineWidth(1.0);
}


//MaterialRef MaterialWireframe::cloneSelf()
//{
//    return std::make_shared<MaterialWireframe>(*this);;
//}

//setters/getters
MaterialWireframe& MaterialWireframe::lineWidth(float aLineWidth)
{
    mLineWidth = aLineWidth;
    return *this;
}

float MaterialWireframe::getLineWidth() const
{
    return mLineWidth;
}

void MaterialWireframe::setLineWidth(float aLineWidth){
    mLineWidth = aLineWidth;
}

///////////////MaterialWireframe////////////////

MaterialPhong::MaterialPhong() : ClassicMaterial()
{
    const ci::DataSourceRef phongVert = ci::app::loadAsset("../assets/materials/phong/phong.vert");
    const ci::DataSourceRef phongFrag = ci::app::loadAsset("../assets/materials/phong/phong.frag");
    mShader = ci::gl::GlslProg::create(phongVert, phongFrag);
//    mShader = ci::gl::getStockShader(ci::gl::ShaderDef().color());
    mType = "material_wireframe";
    mMode = 1;
}

MaterialPhong::~MaterialPhong()
{
    
}

void MaterialPhong::attachAndDraw(const MeshRef& aMesh)
{
    ci::gl::BatchRef batch = aMesh->getBatch();
    if (batch->getGlslProg() != mShader){
        batch->replaceGlslProg(mShader);
    }
    mShader->uniform("uNormalMat", aMesh->getModelWorld().getNormalMatrix());
    mShader->uniform("uMode", mMode);
    batch->draw();
}


//MaterialRef MaterialPhong::cloneSelf()
//{
//    return std::make_shared<MaterialPhong>(*this);;
//}

//setters/getters

MaterialPhong& MaterialPhong::mode(int aMode)
{
    mMode = aMode;
    return *this;
}

int MaterialPhong::getMode() const
{
    return mMode;
}

void MaterialPhong::setMode(int aMode)
{
    mMode = aMode;
}

