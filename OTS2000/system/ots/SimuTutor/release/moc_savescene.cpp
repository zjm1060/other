/****************************************************************************
** Meta object code from reading C++ file 'savescene.h'
**
** Created: Thu Jan 15 10:27:25 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/savescene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'savescene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSaveSceneTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      49,   16,   16,   16, 0x08,
      68,   16,   16,   16, 0x08,
      85,   16,   16,   16, 0x08,
     101,   16,   16,   16, 0x08,
     117,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSaveSceneTable[] = {
    "CSaveSceneTable\0\0buttonClicked(QAbstractButton*)\0"
    "radioClicked(bool)\0displayStation()\0"
    "displayDev(int)\0judgeFinished()\0"
    "setLanguage(int)\0"
};

const QMetaObject CSaveSceneTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSaveSceneTable,
      qt_meta_data_CSaveSceneTable, 0 }
};

const QMetaObject *CSaveSceneTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSaveSceneTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSaveSceneTable))
        return static_cast<void*>(const_cast< CSaveSceneTable*>(this));
    if (!strcmp(_clname, "Ui_CSaveSceneTable"))
        return static_cast< Ui_CSaveSceneTable*>(const_cast< CSaveSceneTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSaveSceneTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: buttonClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 1: radioClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: displayStation(); break;
        case 3: displayDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: judgeFinished(); break;
        case 5: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
