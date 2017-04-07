/****************************************************************************
** Meta object code from reading C++ file 'excelobject.h'
**
** Created: Thu Jan 22 17:27:49 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SimuTutor/src/excelobject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'excelobject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExcelObjectODBC[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ExcelObjectODBC[] = {
    "ExcelObjectODBC\0\0row\0exportedRowCount(int)\0"
    "cancel()\0"
};

const QMetaObject ExcelObjectODBC::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ExcelObjectODBC,
      qt_meta_data_ExcelObjectODBC, 0 }
};

const QMetaObject *ExcelObjectODBC::metaObject() const
{
    return &staticMetaObject;
}

void *ExcelObjectODBC::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExcelObjectODBC))
        return static_cast<void*>(const_cast< ExcelObjectODBC*>(this));
    return QObject::qt_metacast(_clname);
}

int ExcelObjectODBC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: exportedRowCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: cancel(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ExcelObjectODBC::exportedRowCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_ExportExcelObject[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      23,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   18,   18,   18, 0x0a,
      54,   19,   18,   18, 0x0a,
      79,   18,   18,   18, 0x0a,
     109,  102,   18,   18, 0x0a,
     137,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ExportExcelObject[] = {
    "ExportExcelObject\0\0row\0exportedRowCount(int)\0"
    "cancel()\0resizeRowToContents(int)\0"
    "resizeRowsToContents()\0column\0"
    "resizeColumnToContents(int)\0"
    "resizeColumnsToContents()\0"
};

const QMetaObject ExportExcelObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ExportExcelObject,
      qt_meta_data_ExportExcelObject, 0 }
};

const QMetaObject *ExportExcelObject::metaObject() const
{
    return &staticMetaObject;
}

void *ExportExcelObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExportExcelObject))
        return static_cast<void*>(const_cast< ExportExcelObject*>(this));
    return QObject::qt_metacast(_clname);
}

int ExportExcelObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: exportedRowCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: cancel(); break;
        case 2: resizeRowToContents((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: resizeRowsToContents(); break;
        case 4: resizeColumnToContents((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: resizeColumnsToContents(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ExportExcelObject::exportedRowCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
