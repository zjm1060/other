/****************************************************************************
** Meta object code from reading C++ file 'faultdata.h'
**
** Created: Fri Jan 23 13:44:21 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/faultdata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'faultdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CFaultDataAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      35,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   19,   19,   19, 0x0a,
      58,   19,   19,   19, 0x0a,
      66,   19,   19,   19, 0x0a,
      82,   19,   19,   19, 0x0a,
      98,   19,   19,   19, 0x0a,
     120,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CFaultDataAddTable[] = {
    "CFaultDataAddTable\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "changeType(int)\0openPointName()\0"
    "setPointName(QString)\0setLanguage(int)\0"
};

const QMetaObject CFaultDataAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CFaultDataAddTable,
      qt_meta_data_CFaultDataAddTable, 0 }
};

const QMetaObject *CFaultDataAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CFaultDataAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CFaultDataAddTable))
        return static_cast<void*>(const_cast< CFaultDataAddTable*>(this));
    if (!strcmp(_clname, "Ui_CFaultDataAddTable"))
        return static_cast< Ui_CFaultDataAddTable*>(const_cast< CFaultDataAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CFaultDataAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 4: changeType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: openPointName(); break;
        case 6: setPointName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CFaultDataAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CFaultDataAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CFaultDataTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      36,   16,   16,   16, 0x0a,
      58,   16,   16,   16, 0x0a,
      75,   73,   16,   16, 0x0a,
      95,   16,   16,   16, 0x0a,
     110,   16,   16,   16, 0x0a,
     132,   16,  128,   16, 0x0a,
     151,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CFaultDataTable[] = {
    "CFaultDataTable\0\0openFaultDataAdd()\0"
    "openFaultDataModify()\0addFaultData()\0"
    ",\0selectCell(int,int)\0delFaultData()\0"
    "modifyFaultData()\0int\0displayFaultData()\0"
    "setLanguage(int)\0"
};

const QMetaObject CFaultDataTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CFaultDataTable,
      qt_meta_data_CFaultDataTable, 0 }
};

const QMetaObject *CFaultDataTable::metaObject() const
{
    return &staticMetaObject;
}

void *CFaultDataTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CFaultDataTable))
        return static_cast<void*>(const_cast< CFaultDataTable*>(this));
    if (!strcmp(_clname, "Ui_CFaultDataTable"))
        return static_cast< Ui_CFaultDataTable*>(const_cast< CFaultDataTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CFaultDataTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openFaultDataAdd(); break;
        case 1: openFaultDataModify(); break;
        case 2: addFaultData(); break;
        case 3: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: delFaultData(); break;
        case 5: modifyFaultData(); break;
        case 6: { int _r = displayFaultData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
