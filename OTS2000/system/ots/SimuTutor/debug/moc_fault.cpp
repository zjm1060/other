/****************************************************************************
** Meta object code from reading C++ file 'fault.h'
**
** Created: Fri Jan 23 13:44:22 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/fault.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fault.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CFaultAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      31,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   15,   15,   15, 0x0a,
      54,   15,   15,   15, 0x0a,
      62,   15,   15,   15, 0x0a,
      83,   79,   15,   15, 0x0a,
      99,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CFaultAddTable[] = {
    "CFaultAddTable\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "displayStation()\0stn\0displayDev(int)\0"
    "setLanguage(int)\0"
};

const QMetaObject CFaultAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CFaultAddTable,
      qt_meta_data_CFaultAddTable, 0 }
};

const QMetaObject *CFaultAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CFaultAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CFaultAddTable))
        return static_cast<void*>(const_cast< CFaultAddTable*>(this));
    if (!strcmp(_clname, "Ui_CFaultAddTable"))
        return static_cast< Ui_CFaultAddTable*>(const_cast< CFaultAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CFaultAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: insertRecord(); break;
        case 1: modifyRecord(); break;
        case 2: apply(); break;
        case 3: enter(); break;
        case 4: displayStation(); break;
        case 5: displayDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CFaultAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CFaultAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CFaultTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   12,   13,   12, 0x0a,
      34,   32,   12,   12, 0x0a,
      54,   12,   12,   12, 0x0a,
      65,   12,   12,   12, 0x0a,
      81,   12,   12,   12, 0x0a,
      96,   12,   12,   12, 0x0a,
     114,   12,   12,   12, 0x0a,
     125,   12,   12,   12, 0x0a,
     139,   12,   12,   12, 0x0a,
     152,   12,   12,   12, 0x0a,
     171,   32,   12,   12, 0x0a,
     194,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CFaultTable[] = {
    "CFaultTable\0\0int\0displayFault()\0,\0"
    "selectCell(int,int)\0delFault()\0"
    "openFaultData()\0openFaultAdd()\0"
    "openFaultModify()\0addFault()\0modifyFault()\0"
    "openStnDev()\0displayUpdate(int)\0"
    "displayUpdate(int,int)\0setLanguage(int)\0"
};

const QMetaObject CFaultTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CFaultTable,
      qt_meta_data_CFaultTable, 0 }
};

const QMetaObject *CFaultTable::metaObject() const
{
    return &staticMetaObject;
}

void *CFaultTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CFaultTable))
        return static_cast<void*>(const_cast< CFaultTable*>(this));
    if (!strcmp(_clname, "Ui_CFaultTable"))
        return static_cast< Ui_CFaultTable*>(const_cast< CFaultTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CFaultTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = displayFault();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: delFault(); break;
        case 3: openFaultData(); break;
        case 4: openFaultAdd(); break;
        case 5: openFaultModify(); break;
        case 6: addFault(); break;
        case 7: modifyFault(); break;
        case 8: openStnDev(); break;
        case 9: displayUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: displayUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
