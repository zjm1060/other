/****************************************************************************
** Meta object code from reading C++ file 'traininfo.h'
**
** Created: Thu Jan 15 10:24:34 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/traininfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'traininfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTrainInfoAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      35,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   19,   19,   19, 0x08,
      58,   19,   19,   19, 0x08,
      66,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTrainInfoAddTable[] = {
    "CTrainInfoAddTable\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "setLanguage(int)\0"
};

const QMetaObject CTrainInfoAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CTrainInfoAddTable,
      qt_meta_data_CTrainInfoAddTable, 0 }
};

const QMetaObject *CTrainInfoAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CTrainInfoAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTrainInfoAddTable))
        return static_cast<void*>(const_cast< CTrainInfoAddTable*>(this));
    if (!strcmp(_clname, "Ui_CTrainInfoAddTable"))
        return static_cast< Ui_CTrainInfoAddTable*>(const_cast< CTrainInfoAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CTrainInfoAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 4: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CTrainInfoAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CTrainInfoAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CTrainInfoTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      21,   16,   17,   16, 0x0a,
      42,   40,   16,   16, 0x0a,
      62,   16,   16,   16, 0x0a,
      77,   16,   16,   16, 0x0a,
      96,   16,   16,   16, 0x0a,
     118,   16,   16,   16, 0x0a,
     133,   16,   16,   16, 0x0a,
     151,   16,   16,   16, 0x0a,
     165,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTrainInfoTable[] = {
    "CTrainInfoTable\0\0int\0displayTrainInfo()\0"
    ",\0selectCell(int,int)\0delTrainInfo()\0"
    "openTrainInfoAdd()\0openTrainInfoModify()\0"
    "addTrainInfo()\0modifyTrainInfo()\0"
    "exportExcel()\0setLanguage(int)\0"
};

const QMetaObject CTrainInfoTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CTrainInfoTable,
      qt_meta_data_CTrainInfoTable, 0 }
};

const QMetaObject *CTrainInfoTable::metaObject() const
{
    return &staticMetaObject;
}

void *CTrainInfoTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTrainInfoTable))
        return static_cast<void*>(const_cast< CTrainInfoTable*>(this));
    if (!strcmp(_clname, "Ui_CTrainInfoTable"))
        return static_cast< Ui_CTrainInfoTable*>(const_cast< CTrainInfoTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CTrainInfoTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = displayTrainInfo();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: delTrainInfo(); break;
        case 3: openTrainInfoAdd(); break;
        case 4: openTrainInfoModify(); break;
        case 5: addTrainInfo(); break;
        case 6: modifyTrainInfo(); break;
        case 7: exportExcel(); break;
        case 8: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
