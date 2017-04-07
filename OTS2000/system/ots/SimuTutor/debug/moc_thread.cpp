/****************************************************************************
** Meta object code from reading C++ file 'thread.h'
**
** Created: Fri Jan 23 13:43:34 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/thread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserLoginStateThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      36,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UserLoginStateThread[] = {
    "UserLoginStateThread\0\0user_id,state\0"
    "sigUserStateChanged(int,short)\0"
    "readPendingDatagrams()\0"
};

const QMetaObject UserLoginStateThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_UserLoginStateThread,
      qt_meta_data_UserLoginStateThread, 0 }
};

const QMetaObject *UserLoginStateThread::metaObject() const
{
    return &staticMetaObject;
}

void *UserLoginStateThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserLoginStateThread))
        return static_cast<void*>(const_cast< UserLoginStateThread*>(this));
    return QThread::qt_metacast(_clname);
}

int UserLoginStateThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigUserStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 1: readPendingDatagrams(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void UserLoginStateThread::sigUserStateChanged(int _t1, short _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_SaveSceneThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      26,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SaveSceneThread[] = {
    "SaveSceneThread\0\0failed()\0finished()\0"
};

const QMetaObject SaveSceneThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SaveSceneThread,
      qt_meta_data_SaveSceneThread, 0 }
};

const QMetaObject *SaveSceneThread::metaObject() const
{
    return &staticMetaObject;
}

void *SaveSceneThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SaveSceneThread))
        return static_cast<void*>(const_cast< SaveSceneThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SaveSceneThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: failed(); break;
        case 1: finished(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SaveSceneThread::failed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SaveSceneThread::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_SaveSceneRunnable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SaveSceneRunnable[] = {
    "SaveSceneRunnable\0\0finished()\0"
};

const QMetaObject SaveSceneRunnable::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SaveSceneRunnable,
      qt_meta_data_SaveSceneRunnable, 0 }
};

const QMetaObject *SaveSceneRunnable::metaObject() const
{
    return &staticMetaObject;
}

void *SaveSceneRunnable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SaveSceneRunnable))
        return static_cast<void*>(const_cast< SaveSceneRunnable*>(this));
    if (!strcmp(_clname, "QRunnable"))
        return static_cast< QRunnable*>(const_cast< SaveSceneRunnable*>(this));
    return QObject::qt_metacast(_clname);
}

int SaveSceneRunnable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finished(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SaveSceneRunnable::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_NetstateThreadTutor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,
      31,   20,   20,   20, 0x05,
      56,   52,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_NetstateThreadTutor[] = {
    "NetstateThreadTutor\0\0exitapp()\0"
    "showtryconeectfail()\0i,j\0"
    "showtryconeect(int,bool)\0"
};

const QMetaObject NetstateThreadTutor::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_NetstateThreadTutor,
      qt_meta_data_NetstateThreadTutor, 0 }
};

const QMetaObject *NetstateThreadTutor::metaObject() const
{
    return &staticMetaObject;
}

void *NetstateThreadTutor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetstateThreadTutor))
        return static_cast<void*>(const_cast< NetstateThreadTutor*>(this));
    return QThread::qt_metacast(_clname);
}

int NetstateThreadTutor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: exitapp(); break;
        case 1: showtryconeectfail(); break;
        case 2: showtryconeect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NetstateThreadTutor::exitapp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void NetstateThreadTutor::showtryconeectfail()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void NetstateThreadTutor::showtryconeect(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
