/*------------------------------------------------------------------------------
 * JCCL - Component :: Object Factory
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    ObjectFactory.h
 * Purpose: Object Facotry, dynamic create object by class name
 *----------------------------------------------------------------------------*/
 
#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_

#include "JCCLTypes.h"

#include <string>
#include <map>

typedef void* (*Constructor)();

class CObjectFactory
{
public:
    static void registerClass(std::string className, Constructor constructor)
    {
        constructors()[className] = constructor;
    }

    static void* createObject(const std::string& className)
    {
        Constructor constructor = NULL;

        if(constructors().find(className) != constructors().end())
            constructor = constructors().find(className)->second;

        if ( constructor == NULL )
            return NULL;

        return (*constructor)();
    }

private:
    inline static std::map<std::string, Constructor>& constructors()
    {
        static std::map<std::string, Constructor> instance;
        return instance;
    }
};


#define REGISTER_CLASS(class_name) \
class class_name##Helper { \
public: \
    class_name##Helper() \
    { \
        CObjectFactory::registerClass(#class_name, class_name##Helper::creatObjFunc); \
    } \
    static void* creatObjFunc() \
    { \
        return new class_name; \
    } \
}; \
class_name##Helper class_name##helper;

#define CreateClassObject(class_name) ( static_cast<CObjBase*>(CObjectFactory::createObject(strClassName.GetBuffer())) )

#endif // _OBJECT_FACTORY_H_

