/****************************************************************************
** Meta object code from reading C++ file 'mdichild.h'
**
** Created: Thu Jan 22 17:27:40 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mdichild.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mdichild.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MdiChild[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      25,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MdiChild[] = {
    "MdiChild\0\0switchWindow()\0setMenu()\0"
};

const QMetaObject MdiChild::staticMetaObject = {
    { &QMdiSubWindow::staticMetaObject, qt_meta_stringdata_MdiChild,
      qt_meta_data_MdiChild, 0 }
};

const QMetaObject *MdiChild::metaObject() const
{
    return &staticMetaObject;
}

void *MdiChild::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MdiChild))
        return static_cast<void*>(const_cast< MdiChild*>(this));
    return QMdiSubWindow::qt_metacast(_clname);
}

int MdiChild::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMdiSubWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: switchWindow(); break;
        case 1: setMenu(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_MdiArea[] = {

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

static const char qt_meta_stringdata_MdiArea[] = {
    "MdiArea\0"
};

const QMetaObject MdiArea::staticMetaObject = {
    { &QMdiArea::staticMetaObject, qt_meta_stringdata_MdiArea,
      qt_meta_data_MdiArea, 0 }
};

const QMetaObject *MdiArea::metaObject() const
{
    return &staticMetaObject;
}

void *MdiArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MdiArea))
        return static_cast<void*>(const_cast< MdiArea*>(this));
    return QMdiArea::qt_metacast(_clname);
}

int MdiArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMdiArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
