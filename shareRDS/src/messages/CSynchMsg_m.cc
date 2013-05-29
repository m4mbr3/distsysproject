//
// Generated file, do not edit! Created by opp_msgc 4.3 from messages/CSynchMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CSynchMsg_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(CSynchMsg);

CSynchMsg::CSynchMsg(const char *name, int kind) : cMessage(name,kind)
{
    this->timestamp_var = 0;
    this->skewValue_var = 0;
    this->typeOperation_var = 0;
}

CSynchMsg::CSynchMsg(const CSynchMsg& other) : cMessage(other)
{
    copy(other);
}

CSynchMsg::~CSynchMsg()
{
}

CSynchMsg& CSynchMsg::operator=(const CSynchMsg& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void CSynchMsg::copy(const CSynchMsg& other)
{
    this->timestamp_var = other.timestamp_var;
    this->skewValue_var = other.skewValue_var;
    this->typeOperation_var = other.typeOperation_var;
}

void CSynchMsg::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->timestamp_var);
    doPacking(b,this->skewValue_var);
    doPacking(b,this->typeOperation_var);
}

void CSynchMsg::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->timestamp_var);
    doUnpacking(b,this->skewValue_var);
    doUnpacking(b,this->typeOperation_var);
}

double CSynchMsg::getTimestamp() const
{
    return timestamp_var;
}

void CSynchMsg::setTimestamp(double timestamp)
{
    this->timestamp_var = timestamp;
}

int CSynchMsg::getSkewValue() const
{
    return skewValue_var;
}

void CSynchMsg::setSkewValue(int skewValue)
{
    this->skewValue_var = skewValue;
}

int CSynchMsg::getTypeOperation() const
{
    return typeOperation_var;
}

void CSynchMsg::setTypeOperation(int typeOperation)
{
    this->typeOperation_var = typeOperation;
}

class CSynchMsgDescriptor : public cClassDescriptor
{
  public:
    CSynchMsgDescriptor();
    virtual ~CSynchMsgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(CSynchMsgDescriptor);

CSynchMsgDescriptor::CSynchMsgDescriptor() : cClassDescriptor("CSynchMsg", "cMessage")
{
}

CSynchMsgDescriptor::~CSynchMsgDescriptor()
{
}

bool CSynchMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CSynchMsg *>(obj)!=NULL;
}

const char *CSynchMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CSynchMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int CSynchMsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *CSynchMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "timestamp",
        "skewValue",
        "typeOperation",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int CSynchMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamp")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "skewValue")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "typeOperation")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CSynchMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *CSynchMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CSynchMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CSynchMsg *pp = (CSynchMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CSynchMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CSynchMsg *pp = (CSynchMsg *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getTimestamp());
        case 1: return long2string(pp->getSkewValue());
        case 2: return long2string(pp->getTypeOperation());
        default: return "";
    }
}

bool CSynchMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CSynchMsg *pp = (CSynchMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setTimestamp(string2double(value)); return true;
        case 1: pp->setSkewValue(string2long(value)); return true;
        case 2: pp->setTypeOperation(string2long(value)); return true;
        default: return false;
    }
}

const char *CSynchMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *CSynchMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CSynchMsg *pp = (CSynchMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


