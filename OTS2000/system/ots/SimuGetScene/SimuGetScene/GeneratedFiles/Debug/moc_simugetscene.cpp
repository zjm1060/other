/****************************************************************************
** Meta object code from reading C++ file 'simugetscene.h'
**
** Created: Thu Jan 9 14:16:04 2014
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../simugetscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simugetscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimuGetSceneTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      36,   19,   18,   18, 0x08,
      96,   84,   18,   18, 0x08,
     139,  134,   18,   18, 0x08,
     162,   18,   18,   18, 0x08,
     174,   18,   18,   18, 0x08,
     186,   18,   18,   18, 0x08,
     201,   18,   18,   18, 0x08,
     222,   18,   18,   18, 0x08,
     253,  247,   18,   18, 0x08,
     279,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SimuGetSceneTable[] = {
    "SimuGetSceneTable\0\0current,previous\0"
    "displayPoint(QTreeWidgetItem*,QTreeWidgetItem*)\0"
    "item,column\0treeItemChanged(QTreeWidgetItem*,int)\0"
    "text\0filterChanged(QString)\0selectAll()\0"
    "selectOff()\0selectInvert()\0"
    "pointModeChange(int)\0pointShowTypeChange(int)\0"
    "index\0pointClicked(QModelIndex)\0"
    "saveAllChange()\0"
};

const QMetaObject SimuGetSceneTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SimuGetSceneTable,
      qt_meta_data_SimuGetSceneTable, 0 }
};

const QMetaObject *SimuGetSceneTable::metaObject() const
{
    return &staticMetaObject;
}

void *SimuGetSceneTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimuGetSceneTable))
        return static_cast<void*>(const_cast< SimuGetSceneTable*>(this));
    if (!strcmp(_clname, "Ui_CSimuGetScene"))
        return static_cast< Ui_CSimuGetScene*>(const_cast< SimuGetSceneTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int SimuGetSceneTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: displayPoint((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 1: treeItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: filterChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: selectAll(); break;
        case 4: selectOff(); break;
        case 5: selectInvert(); break;
        case 6: pointModeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: pointShowTypeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: pointClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: saveAllChange(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
