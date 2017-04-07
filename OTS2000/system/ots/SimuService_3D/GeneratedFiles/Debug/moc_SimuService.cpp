/****************************************************************************
** Meta object code from reading C++ file 'SimuService.h'
**
** Created: Tue May 27 14:03:06 2014
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SimuService.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SimuService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReadPointThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ReadPointThread[] = {
    "ReadPointThread\0\0setInitValue()\0"
};

const QMetaObject ReadPointThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ReadPointThread,
      qt_meta_data_ReadPointThread, 0 }
};

const QMetaObject *ReadPointThread::metaObject() const
{
    return &staticMetaObject;
}

void *ReadPointThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReadPointThread))
        return static_cast<void*>(const_cast< ReadPointThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ReadPointThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setInitValue(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_SimuService[] = {

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

static const char qt_meta_stringdata_SimuService[] = {
    "SimuService\0"
};

const QMetaObject SimuService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SimuService,
      qt_meta_data_SimuService, 0 }
};

const QMetaObject *SimuService::metaObject() const
{
    return &staticMetaObject;
}

void *SimuService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimuService))
        return static_cast<void*>(const_cast< SimuService*>(this));
    return QObject::qt_metacast(_clname);
}

int SimuService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
