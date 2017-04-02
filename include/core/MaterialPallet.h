//
//  MaterialPallet.h
//  SimpleRenderer
//
//  Created by Hesam Ohadi on 4/12/15.
//
//

#pragma once

#include "../core/Material.h"

////////////////////MaterialPallet//////////////////
class Scene;
typedef std::shared_ptr<class MaterialPallet> MaterialPalletRef;

class MaterialPallet : public std::enable_shared_from_this<MaterialPallet>
{
public:
    MaterialPallet();
    virtual ~MaterialPallet()=0;
    
    //setters/getters
    
    MaterialPallet&                             maxNumMaterials(int aMaxNumMaterials) {mMaxNumMaterials = aMaxNumMaterials; return *this;}
    void                                        setMaxNumMaterials(int aMaxNumMaterials) {mMaxNumMaterials = aMaxNumMaterials;}
    int                                         getMaxNumMaterials() const {return mMaxNumMaterials;}
    
public:
    int                                         mMaxNumMaterials;
};

////////////////////ClassicMaterialPallet//////////////////
typedef std::shared_ptr<class ClassicMaterialPallet> ClassicMaterialPalletRef;

class ClassicMaterialPallet : public MaterialPallet
{
public:
    ClassicMaterialPallet();
    ~ClassicMaterialPallet();
    
    void                                        addMaterial(const ClassicMaterialRef& aMaterial);
    void                                        removeMaterial(const ClassicMaterialRef& aMaterial);
    const std::list<ClassicMaterialRef>&        getMaterials() const;
    const ClassicMaterialRef&                   getMaterialByIndex(int aIndex) const;
    
public:
    std::list<ClassicMaterialRef>               mMaterials;
    
protected:
    void                                        calcIds();

};

////////////////////PbrMaterialPallet//////////////////
typedef std::shared_ptr<class PbrMaterialPallet> PbrMaterialPalletRef;

class PbrMaterialPallet : public MaterialPallet
{
public:
    PbrMaterialPallet();
    ~PbrMaterialPallet();
    
    void                                        addMaterial(const PbrMaterialRef& aMaterial);
    void                                        removeMaterial(const PbrMaterialRef& aMaterial);
    const std::list<PbrMaterialRef>&            getMaterials() const;
    const PbrMaterialRef&                       getMaterialByIndex(int aIndex) const;
    
public:
    std::list<PbrMaterialRef>                   mMaterials;
    
protected:
    void                                        calcIds();
    
};
