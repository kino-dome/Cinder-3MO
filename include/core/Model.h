//
//  Model.h
//  iPhotoWall
//
//  Created by Hesam Ohadi on 15/11/15.
//
//

#pragma once

#include "cinder/Matrix.h"

class Model
{
public:
    
    Model();
    Model           clone();
    
    //setters/getters
    Model&			modelMatrix( const ci::mat4& m ) {mModelMatrix = m; decomposeModel(); return *this;}
    Model&			normalMatrix( const ci::mat3& m ) {mNormalMatrix = m; return *this;}
    Model&			modelTranslation( const ci::vec3& aTranslation ) {mModelTranslation = aTranslation; calcModel(); return *this;}
    Model&			modelOrientation( const ci::quat& aOrientation ) {mModelOrientation = aOrientation; calcModel(); return *this;}
    Model&			modelScale( const ci::vec3& aScale ) {mModelScale = aScale; calcModel(); return *this;}
    void			setModelMatrix( const ci::mat4& m ) {mModelMatrix = m; decomposeModel();}
    void			setNormalMatrix( const ci::mat3& m ) {mNormalMatrix = m;}
    void            setModelTranslation(const ci::vec3& aTranslation) {mModelTranslation = aTranslation; calcModel();}
    void            setModelOrientation(const ci::quat& aOrientation) {mModelOrientation = aOrientation; calcModel();}
    void            setModelScale(const ci::vec3& aScale) {mModelScale = aScale; calcModel();}
    const ci::mat4&	getModelMatrix() const {return mModelMatrix;}
    const ci::mat3&	getNormalMatrix() const {return mNormalMatrix;}
    const ci::vec3& getModelTranslation() const {return mModelTranslation;}
    const ci::quat& getModelOrientation() const {return mModelOrientation;}
    const ci::vec3& getModelScale() const {return mModelScale;}
    
protected:
    void            calcModel();
    void            decomposeModel();
    
    ci::mat4		mModelMatrix;
    ci::mat3		mNormalMatrix;
    //decomposed components
    ci::vec3        mModelTranslation, mModelScale;
    ci::quat        mModelOrientation;
};