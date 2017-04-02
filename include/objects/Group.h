//
//  Group.h
//  BasicScene
//
//  Created by Hesam Ohadi on 17/10/16.
//
//

#pragma once

#include "../core/Object3D.h"

typedef std::shared_ptr<class Group> GroupRef;
class Group : public Object3D
{
public:
    static GroupRef                      create() {return GroupRef(new Group());}
    
    Group() {}
    ~Group(){}
};
