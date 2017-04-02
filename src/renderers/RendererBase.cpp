//
//  RendererBase.cpp
//  DeferredRendererBase
//
//  Created by Hesam Ohadi on 5/12/15.
//
//

#include "renderers/RendererBase.h"

RendererBase::RendererBase() : mType("renderer_base")
{

}

RendererBase::~RendererBase()
{
    
}

void RendererBase::setup()
{
    if (!mParentWindow){
        setParentWindow(ci::app::App::get()->getWindow());
    }
}

void RendererBase::setup(const ci::app::WindowRef& aParentWindow)
{
    setParentWindow(aParentWindow);
}

// setters / getters
void RendererBase::setParentWindow(const ci::app::WindowRef &aParentWindow)
{
    mParentWindow = aParentWindow;
}

void RendererBase::setType(const std::string &aType)
{
    mType = aType;
}

void RendererBase::setClearColor(const ci::ColorAf aClearColor)
{
    mClearColor = aClearColor;
}

const ci::app::WindowRef& RendererBase::getParentWindow() const
{
    return mParentWindow;
}

const std::string& RendererBase::getType() const
{
    return mType;
}

const ci::ColorAf& RendererBase::getClearColor() const
{
    return mClearColor;
}

