/****************************************************************************
** Meta object code from reading C++ file 'SceneReset.h'
**
** Created: Thu Jan 15 10:27:15 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/SceneReset.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SceneReset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SceneReset[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      43,   11,   11,   11, 0x08,
      56,   11,   11,   11, 0x08,
      66,   11,   11,   11, 0x08,
      74,   11,   11,   11, 0x08,
      86,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SceneReset[] = {
    "SceneReset\0\0ItemSelected(QTreeWidgetItem*)\0"
    "on_confirm()\0on_init()\0on_ok()\0"
    "on_cancel()\0setLanguage(int)\0"
};

const QMetaObject SceneReset::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SceneReset,
      qt_meta_data_SceneReset, 0 }
};

const QMetaObject *SceneReset::metaObject() const
{
    return &staticMetaObject;
}

void *SceneReset::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SceneReset))
        return static_cast<void*>(const_cast< SceneReset*>(this));
    if (!strcmp(_clname, "Ui_SceneReset"))
        return static_cast< Ui_SceneReset*>(const_cast< SceneReset*>(this));
    return QDialog::qt_metacast(_clname);
}

int SceneReset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ItemSelected((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: on_confirm(); break;
        case 2: on_init(); break;
        case 3: on_ok(); break;
        case 4: on_cancel(); break;
        case 5: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
