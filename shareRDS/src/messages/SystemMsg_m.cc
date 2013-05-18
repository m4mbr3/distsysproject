//
// Generated file, do not edit! Created by opp_msgc 4.2 from messages/SystemMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SystemMsg_m.h"

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




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("OperationType");
    if (!e) enums.getInstance()->add(e = new cEnum("OperationType"));
    e->insert(READ, "READ");
    e->insert(WRITE, "WRITE");
    e->insert(COMMIT, "COMMIT");
    e->insert(ROLLBACK, "ROLLBACK");
    e->insert(UPDATE, "UPDATE");
    e->insert(ACK, "ACK");
    e->insert(DELETE, "DELETE");
    e->insert(REINC, "REINC");
);

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("ReplyCodeType");
    if (!e) enums.getInstance()->add(e = new cEnum("ReplyCodeType"));
    e->insert(FAIL, "FAIL");
    e->insert(SUCCESS, "SUCCESS");
    e->insert(SUCC_CLIENT, "SUCC_CLIENT");
    e->insert(REINCAR, "REINCAR");
    e->insert(NONE, "NONE");
);

Register_Class(SystemMsg);

SystemMsg::SystemMsg(const char *name, int kind) : cMessage(name,kind)
{
    this->clientID_var = -1;
    this->replicaID_var = -1;
    this->replicaOwnerID_var = -1;
    this->lamportClock_var = -1;
    this->replyCode_var = -1;
    this->operation_var = -1;
    this->dataID_var = 0;
    this->data_var = 0;
}

SystemMsg::SystemMsg(const SystemMsg& other) : cMessage(other)
{
    copy(other);
}

SystemMsg::~SystemMsg()
{
}

SystemMsg& SystemMsg::operator=(const SystemMsg& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void SystemMsg::copy(const SystemMsg& other)
{
    this->clientID_var = other.clientID_var;
    this->replicaID_var = other.replicaID_var;
    this->replicaOwnerID_var = other.replicaOwnerID_var;
    this->lamportClock_var = other.lamportClock_var;
    this->replyCode_var = other.replyCode_var;
    this->operation_var = other.operation_var;
    this->dataID_var = other.dataID_var;
    this->data_var = other.data_var;
}

void SystemMsg::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->clientID_var);
    doPacking(b,this->replicaID_var);
    doPacking(b,this->replicaOwnerID_var);
    doPacking(b,this->lamportClock_var);
    doPacking(b,this->replyCode_var);
    doPacking(b,this->operation_var);
    doPacking(b,this->dataID_var);
    doPacking(b,this->data_var);
}

void SystemMsg::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->clientID_var);
    doUnpacking(b,this->replicaID_var);
    doUnpacking(b,this->replicaOwnerID_var);
    doUnpacking(b,this->lamportClock_var);
    doUnpacking(b,this->replyCode_var);
    doUnpacking(b,this->operation_var);
    doUnpacking(b,this->dataID_var);
    doUnpacking(b,this->data_var);
}

int SystemMsg::getClientID() const
{
    return clientID_var;
}

void SystemMsg::setClientID(int clientID)
{
    this->clientID_var = clientID;
}

int SystemMsg::getReplicaID() const
{
    return replicaID_var;
}

void SystemMsg::setReplicaID(int replicaID)
{
    this->replicaID_var = replicaID;
}

int SystemMsg::getReplicaOwnerID() const
{
    return replicaOwnerID_var;
}

void SystemMsg::setReplicaOwnerID(int replicaOwnerID)
{
    this->replicaOwnerID_var = replicaOwnerID;
}

int SystemMsg::getLamportClock() const
{
    return lamportClock_var;
}

void SystemMsg::setLamportClock(int lamportClock)
{
    this->lamportClock_var = lamportClock;
}

int SystemMsg::getReplyCode() const
{
    return replyCode_var;
}

void SystemMsg::setReplyCode(int replyCode)
{
    this->replyCode_var = replyCode;
}

int SystemMsg::getOperation() const
{
    return operation_var;
}

void SystemMsg::setOperation(int operation)
{
    this->operation_var = operation;
}

const char * SystemMsg::getDataID() const
{
    return dataID_var.c_str();
}

void SystemMsg::setDataID(const char * dataID)
{
    this->dataID_var = dataID;
}

int SystemMsg::getData() const
{
    return data_var;
}

void SystemMsg::setData(int data)
{
    this->data_var = data;
}

class SystemMsgDescriptor : public cClassDescriptor
{
  public:
    SystemMsgDescriptor();
    virtual ~SystemMsgDescriptor();

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

Register_ClassDescriptor(SystemMsgDescriptor);

SystemMsgDescriptor::SystemMsgDescriptor() : cClassDescriptor("SystemMsg", "cMessage")
{
}

SystemMsgDescriptor::~SystemMsgDescriptor()
{
}

bool SystemMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SystemMsg *>(obj)!=NULL;
}

const char *SystemMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SystemMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount(object) : 8;
}

unsigned int SystemMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *SystemMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "clientID",
        "replicaID",
        "replicaOwnerID",
        "lamportClock",
        "replyCode",
        "operation",
        "dataID",
        "data",
    };
    return (field>=0 && field<8) ? fieldNames[field] : NULL;
}

int SystemMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientID")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "replicaID")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "replicaOwnerID")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "lamportClock")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "replyCode")==0) return base+4;
    if (fieldName[0]=='o' && strcmp(fieldName, "operation")==0) return base+5;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataID")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+7;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SystemMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "string",
        "int",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : NULL;
}

const char *SystemMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 4:
            if (!strcmp(propertyname,"enum")) return "ReplyCodeType";
            return NULL;
        case 5:
            if (!strcmp(propertyname,"enum")) return "OperationType";
            return NULL;
        default: return NULL;
    }
}

int SystemMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SystemMsg *pp = (SystemMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SystemMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SystemMsg *pp = (SystemMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getClientID());
        case 1: return long2string(pp->getReplicaID());
        case 2: return long2string(pp->getReplicaOwnerID());
        case 3: return long2string(pp->getLamportClock());
        case 4: return long2string(pp->getReplyCode());
        case 5: return long2string(pp->getOperation());
        case 6: return oppstring2string(pp->getDataID());
        case 7: return long2string(pp->getData());
        default: return "";
    }
}

bool SystemMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SystemMsg *pp = (SystemMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setClientID(string2long(value)); return true;
        case 1: pp->setReplicaID(string2long(value)); return true;
        case 2: pp->setReplicaOwnerID(string2long(value)); return true;
        case 3: pp->setLamportClock(string2long(value)); return true;
        case 4: pp->setReplyCode(string2long(value)); return true;
        case 5: pp->setOperation(string2long(value)); return true;
        case 6: pp->setDataID((value)); return true;
        case 7: pp->setData(string2long(value)); return true;
        default: return false;
    }
}

const char *SystemMsgDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<8) ? fieldStructNames[field] : NULL;
}

void *SystemMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SystemMsg *pp = (SystemMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


