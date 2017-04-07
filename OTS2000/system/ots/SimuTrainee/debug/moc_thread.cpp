/****************************************************************************
** Meta object code from reading C++ file 'thread.h'
**
** Created: Thu Jan 22 17:27:16 2015
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
static const uint qt_meta_data_HeartBeatThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_HeartBeatThread[] = {
    "HeartBeatThread\0"
};

const QMetaObject HeartBeatThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_HeartBeatThread,
      qt_meta_data_HeartBeatThread, 0 }
};

const QMetaObject *HeartBeatThread::metaObject() const
{
    return &staticMetaObject;
}

void *HeartBeatThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HeartBeatThread))
        return static_cast<void*>(const_cast< HeartBeatThread*>(this));
    return QThread::qt_metacast(_clname);
}

int HeartBeatThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DisplayThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      32,   14,   14,   14, 0x05,
      45,   14,   14,   14, 0x05,
      72,   14,   14,   14, 0x05,
     102,   14,   14,   14, 0x05,
     130,   14,   14,   14, 0x05,
     156,   14,   14,   14, 0x05,
     191,   14,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DisplayThread[] = {
    "DisplayThread\0\0sigForceLogout()\0"
    "sigNewTask()\0sigUpdateSimuProgress(int)\0"
    "sigUpdateCurTaskName(QString)\0"
    "sigUpdateStatusBar(QString)\0"
    "sigTrainmodelchanged(int)\0"
    "sigSimustatechanged(unsigned char)\0"
    "sigGroupinfoChanged()\0"
};

const QMetaObject DisplayThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_DisplayThread,
      qt_meta_data_DisplayThread, 0 }
};

const QMetaObject *DisplayThread::metaObject() const
{
    return &staticMetaObject;
}

void *DisplayThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayThread))
        return static_cast<void*>(const_cast< DisplayThread*>(this));
    return QThread::qt_metacast(_clname);
}

int DisplayThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigForceLogout(); break;
        case 1: sigNewTask(); break;
        case 2: sigUpdateSimuProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: sigUpdateCurTaskName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: sigUpdateStatusBar((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: sigTrainmodelchanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: sigSimustatechanged((*reinterpret_cast< unsigned char(*)>(_a[1]))); break;
        case 7: sigGroupinfoChanged(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void DisplayThread::sigForceLogout()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DisplayThread::sigNewTask()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DisplayThread::sigUpdateSimuProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DisplayThread::sigUpdateCurTaskName(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DisplayThread::sigUpdateStatusBar(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DisplayThread::sigTrainmodelchanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DisplayThread::sigSimustatechanged(unsigned char _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DisplayThread::sigGroupinfoChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
static const uint qt_meta_data_NetstateThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      26,   15,   15,   15, 0x05,
      51,   47,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_NetstateThread[] = {
    "NetstateThread\0\0exitapp()\0"
    "showtryconeectfail()\0i,j\0"
    "showtryconeect(int,bool)\0"
};

const QMetaObject NetstateThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_NetstateThread,
      qt_meta_data_NetstateThread, 0 }
};

const QMetaObject *NetstateThread::metaObject() const
{
    return &staticMetaObject;
}

void *NetstateThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetstateThread))
        return static_cast<void*>(const_cast< NetstateThread*>(this));
    return QThread::qt_metacast(_clname);
}

int NetstateThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void NetstateThread::exitapp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void NetstateThread::showtryconeectfail()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void NetstateThread::showtryconeect(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
