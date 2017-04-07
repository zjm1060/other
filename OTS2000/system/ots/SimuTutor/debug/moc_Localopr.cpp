/****************************************************************************
** Meta object code from reading C++ file 'Localopr.h'
**
** Created: Fri Jan 23 13:44:18 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Localopr.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Localopr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CLocalOprTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      42,   38,   15,   15, 0x08,
      63,   15,   15,   15, 0x08,
      85,   15,   15,   15, 0x08,
     110,   15,   15,   15, 0x08,
     128,   15,   15,   15, 0x08,
     149,   15,   15,   15, 0x08,
     167,   15,   15,   15, 0x08,
     188,   15,   15,   15, 0x08,
     206,   15,   15,   15, 0x08,
     220,   15,   15,   15, 0x08,
     237,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CLocalOprTable[] = {
    "CLocalOprTable\0\0displayLocalOprType()\0"
    "row\0displayLocalOpr(int)\0openLocalOprTypeAdd()\0"
    "openLocalOprTypeModify()\0openLocalOprAdd()\0"
    "openLocalOprModify()\0addLocalOprType()\0"
    "modifyLocalOprType()\0delLocalOprType()\0"
    "addLocalOpr()\0modifyLocalOpr()\0"
    "delLocalOpr()\0"
};

const QMetaObject CLocalOprTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CLocalOprTable,
      qt_meta_data_CLocalOprTable, 0 }
};

const QMetaObject *CLocalOprTable::metaObject() const
{
    return &staticMetaObject;
}

void *CLocalOprTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLocalOprTable))
        return static_cast<void*>(const_cast< CLocalOprTable*>(this));
    if (!strcmp(_clname, "Ui_CLocalOprTable"))
        return static_cast< Ui_CLocalOprTable*>(const_cast< CLocalOprTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CLocalOprTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: displayLocalOprType(); break;
        case 1: displayLocalOpr((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: openLocalOprTypeAdd(); break;
        case 3: openLocalOprTypeModify(); break;
        case 4: openLocalOprAdd(); break;
        case 5: openLocalOprModify(); break;
        case 6: addLocalOprType(); break;
        case 7: modifyLocalOprType(); break;
        case 8: delLocalOprType(); break;
        case 9: addLocalOpr(); break;
        case 10: modifyLocalOpr(); break;
        case 11: delLocalOpr(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
