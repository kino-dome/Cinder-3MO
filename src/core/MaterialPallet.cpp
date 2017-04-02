//
//  MaterialPallet.cpp
//  SimpleRenderer
//
//  Created by Hesam Ohadi on 4/12/15.
//
//

#include "MaterialPallet.h"

/////////////////MaterialPallet//////////////////

MaterialPallet::MaterialPallet() : mMaxNumMaterials(256)
{
    
}

MaterialPallet::~MaterialPallet()
{
    
}

////////////////ClassicMaterialPallet///////////////////////

ClassicMaterialPallet::ClassicMaterialPallet() : MaterialPallet()
{
    
}

ClassicMaterialPallet::~ClassicMaterialPallet()
{
    
}
void ClassicMaterialPallet::addMaterial(const ClassicMaterialRef &aMaterial)
{
    mMaterials.push_back(aMaterial);
    calcIds();
}

void ClassicMaterialPallet::removeMaterial(const ClassicMaterialRef &aMaterial)
{
    for (auto iter = mMaterials.begin(); iter!=mMaterials.end(); ++iter){
        MaterialRef material = *iter;
        if (aMaterial == material){
            iter = mMaterials.erase(iter);
            break;
        }
    }
    calcIds();
}

const std::list<ClassicMaterialRef>& ClassicMaterialPallet::getMaterials() const
{
    return mMaterials;
}

const ClassicMaterialRef& ClassicMaterialPallet::getMaterialByIndex(int aIndex) const
{
    auto iter = mMaterials.begin();
    std::advance(iter, aIndex);
    return *iter;
}

void ClassicMaterialPallet::calcIds()
{
    for (auto iter = mMaterials.begin(); iter != mMaterials.end(); ++iter){
        int index = std::distance(mMaterials.begin(), iter);
        MaterialRef material = *iter;
        material->setId(index);
    }
}

////////////////PbrMaterialPallet///////////////////////

PbrMaterialPallet::PbrMaterialPallet() : MaterialPallet()
{
    
}

PbrMaterialPallet::~PbrMaterialPallet()
{
    
}
void PbrMaterialPallet::addMaterial(const PbrMaterialRef &aMaterial)
{
    mMaterials.push_back(aMaterial);
    calcIds();
}

void PbrMaterialPallet::removeMaterial(const PbrMaterialRef &aMaterial)
{
    for (auto iter = mMaterials.begin(); iter!=mMaterials.end(); ++iter){
        MaterialRef material = *iter;
        if (aMaterial== material){
            iter = mMaterials.erase(iter);
            break;
        }
    }
    calcIds();
}

const std::list<PbrMaterialRef>& PbrMaterialPallet::getMaterials() const
{
    return mMaterials;
}

const PbrMaterialRef& PbrMaterialPallet::getMaterialByIndex(int aIndex) const
{
    auto iter = mMaterials.begin();
    std::advance(iter, aIndex);
    return *iter;
}

void PbrMaterialPallet::calcIds()
{
    for (auto iter = mMaterials.begin(); iter != mMaterials.end(); ++iter){
        int index = std::distance(mMaterials.begin(), iter);
        MaterialRef material = *iter;
        material->setId(index);
    }
}

