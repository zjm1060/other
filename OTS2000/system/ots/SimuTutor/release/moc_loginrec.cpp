/****************************************************************************
** Meta object code from reading C++ file 'loginrec.h'
**
** Created: Thu Jan 15 10:27:52 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/loginrec.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loginrec.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CLoginRecTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      34,   15,   15,   15, 0x08,
      57,   15,   15,   15, 0x08,
      73,   15,   15,   15, 0x08,
      87,   15,   15,   15, 0x08,
     111,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CLoginRecTable[] = {
    "CLoginRecTable\0\0displayLoginRec()\0"
    "displayTodayLoginRec()\0exportToExcel()\0"
    "delLoginRec()\0delLoginRecFromHdb(int)\0"
    "setLanguage(int)\0"
};

const QMetaObject CLoginRecTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CLoginRecTable,
      qt_meta_data_CLoginRecTable, 0 }
};

const QMetaObject *CLoginRecTable::metaObject() const
{
    return &staticMetaObject;
}

void *CLoginRecTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLoginRecTable))
        return static_cast<void*>(const_cast< CLoginRecTable*>(this));
    if (!strcmp(_clname, "Ui_CLoginRecTable"))
        return static_cast< Ui_CLoginRecTable*>(const_cast< CLoginRecTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CLoginRecTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: displayLoginRec(); break;
        case 1: displayTodayLoginRec(); break;
        case 2: exportToExcel(); break;
        case 3: delLoginRec(); break;
        case 4: delLoginRecFromHdb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
